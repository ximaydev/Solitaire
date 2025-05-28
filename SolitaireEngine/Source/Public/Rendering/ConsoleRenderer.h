#pragma once
#include <Windows.h>
#include "Globals.h"
#include "Core/Utility/Colors.h"
#include <cassert>

/** Represents an invalid or uninitialized 2D grid position(commonly used as a sentinel value) */
#define INVALID_GRID_POSITION SGridPositionU32((SUInt32)-1, (SUInt32)-1)

using namespace ConsoleColor;

/** Interface for objects that can be rendered in the console using a grid-based coordinate system. */
class SOLITAIRE_ENGINE_API SIConsoleRenderable
{
public:
    /** Constructors */
    SIConsoleRenderable() = default;
    SIConsoleRenderable(const SGridPositionU32& NewGridPosition) : GridPosition(NewGridPosition) {}

    /** Get Grid Position. */
    inline const SGridPositionU32& GetGridPosition() const { return GridPosition; }

    /** Set Grid Position */
    inline void SetGridPosition(const SGridPositionU32& NewGridPosition) { GridPosition = NewGridPosition; }

    /** Renders the object at its internal grid position. */
    virtual void Write() {}

    /** Clears the object from the console using its internal grid position. */
    virtual void ClearBuffer() {}

protected:
    /** Grid position used for default rendering. */
    SGridPositionU32 GridPosition = INVALID_GRID_POSITION;
};

/** Responsible for rendering output to the console window. */
class SOLITAIRE_ENGINE_API SConsoleRenderer
{
public:
    /** Constructor */
    SConsoleRenderer();

    /** Returns a singleton instance of the console renderer. */
    static SConsoleRenderer* GetInstance();

    /** Returns the screen width in characters. */
    inline const SUInt16 GetScreenWidth() const { return ScreenWidth; }

    /** Returns the screen height in characters. */
    inline const SUInt16 GetScreenHeight() const { return ScreenHeight; }

    /** Returns half of the screen width. */
    inline const SUInt16 GetHalfScreenWidth() const { return ScreenWidth / 2; }

    /** Returns half of the screen height. */
    inline const SUInt16 GetHalfScreenHeight() const { return ScreenHeight / 2; }

    /** Returns Current Background color */
    inline WORD GetCurrentBackgroundColor() const { return CurrentBackgroundColor; }

    /** Clears the screen buffer. */
    void ClearBuffer();

    /** Clears the screen buffer at the given position */
    void ClearBufferAt(const SGridPositionU32& GridPosition, SSize Size);

    /** Writes to the screen buffer. */
    void Write(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length, SBool bRespectBorder = true, WORD Color = FG_WHITE);

    /** Writes to the screen buffer. */
    template<SSize Size>
    void Write(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length, const SSpan<WORD, Size>& Colors, SBool bRespectBorder = true);

    /** Performs the main draw routine. */
    void Draw();

    /** Shows or hides the console cursor. */
    bool ShowConsolCursor(SBool bShowCursor);

    /** Draws a horizontal panel starting at the specified grid position. */
    void DrawHorizontalPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, WORD Color = FG_WHITE, const SWString& Character = TEXT(""));

    /** Draws a vertical panel starting at the specified grid position. */
    void DrawVerticalPanel(const SGridPositionU32& GridPosition, const SUInt32 Height, WORD Color = FG_WHITE, const SWString& Character = TEXT(""));

    /** Draws a filled rectangular panel. */
    void DrawPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, const SUInt32 Height, WORD Color = FG_WHITE, const SWString& WidthCharacter = TEXT(""), const SWString& HeightCharacter = TEXT(""));

    /** Draws the console window border. */
    void DrawBorder();

    /** Sets text color for a specific region in the buffer. */
    bool SetTextColor(const SGridPositionU32& GridPosition, const SSize Size, WORD Color = FG_WHITE);

    /** Sets background color for a specific region in the buffer */
    bool SetBackgroundColor(const SGridPositionU32& GridPosition, const SUInt32 BufferSize, WORD BackgroundColor = BG_BLACK);

protected:
    /** Handle to the console output. */
    HANDLE ConsoleScreenBuffer;

    /** Rectangle representing the dimensions of the console window. */
    RECT ConsoleWindowRectangle;

    /** Screen buffer used for writing characters before flushing to the console. */
    SUniquePtr<CHAR_INFO[]> ScreenBuffer;

    /** Width of the screen buffer in characters. */
    SUInt16 ScreenWidth;

    /** Height of the screen buffer in characters. */
    SUInt16 ScreenHeight;

    /** Size of the buffer */
    SUInt16 BufferSize;

    /** Current background color */
    WORD CurrentBackgroundColor;

    /** Validates that a given write operation does not go beyond the console buffer. */
    SUInt32 ValidateWriteBounds(const SGridPositionU32& GridPosition, SSize TextLength);

    /** Locks the console window size by disabling resizing and maximizing. */
    void ConfigureConsoleWindow(HWND ConsoleHandle);

    /** Writes to the screen buffer. */
    template<typename TColorProvider>
    void WriteInternal(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length, SBool bRespectBorder, TColorProvider GetColor);
};

template<SSize Size>
void SConsoleRenderer::Write(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length, const SSpan<WORD, Size>& Colors, SBool bRespectBorder)
{
    WriteInternal(GridPosition, Text, Length, bRespectBorder, [&](SUInt32 Index) { return Colors[Index]; });
}

template<typename TColorProvider>
void SConsoleRenderer::WriteInternal(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length, SBool bRespectBorder, TColorProvider GetColor)
{
    // Writes text to the screen buffer starting at the given grid position.
    // Respects left/right borders if bRespectBorder is true by constraining X between MinX and MaxX.
    SGridPositionU32 TempGridPosition = GridPosition;

    // Define horizontal boundaries
    SUInt32 MinX = 0;
    SUInt32 MaxX = ScreenWidth - 1;

    // Check if we respect the border
    if (bRespectBorder)
    {
        // Reinitialize MinX and MaxX
        MinX = 2;
        MaxX = ScreenWidth - 3;

        // Clamp initial X to the minimum border if needed
        if (TempGridPosition.first < MinX)
        {
            TempGridPosition.first = MinX;
        }
    }

    for (SUInt32 Index = 0; Index < Length; Index++)
    {
        // Abort writing if the text exceeds the screen's vertical boundary
        if (TempGridPosition.second >= ScreenHeight)
        {
            S_LOG_WARNING(LogConsoleRenderer, TEXT("Write aborted: text exceeds screen height at Y=%u"), TempGridPosition.second);
            assert(false && "Text rendering aborted: exceeds screen height");
        }

        // Wrap to next line if text reaches or exceeds right border
        if (TempGridPosition.first > MaxX)
        {
            // Reset to MinX(left margin inside border),
            TempGridPosition.first = MinX;

            // Incremented to move to the next line vertically
            TempGridPosition.second++;
        }

        // Validate the write position and write the character and color attribute
        const SUInt32 BufferIndex = ValidateWriteBounds(TempGridPosition, 1);

        // Set information at BufferIndex cell
        ScreenBuffer[BufferIndex].Char.UnicodeChar = Index < Text.size() ? Text[Index] : TEXT(' ');
        ScreenBuffer[BufferIndex].Attributes = GetColor(Index);

        // Advance to the next character position
        TempGridPosition.first++;
    }
}