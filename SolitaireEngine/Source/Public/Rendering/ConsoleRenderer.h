#pragma once
#include <Windows.h>
#include "Globals.h"
#include "Core/Utility/Colors.h"

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
    inline const SUInt8 GetScreenWidth() const { return ScreenWidth; }

    /** Returns the screen height in characters. */
    inline const SUInt8 GetScreenHeight() const { return ScreenHeight; }

    /** Returns half of the screen width. */
    inline const SUInt8 GetHalfScreenWidth() const { return ScreenWidth / 2; }

    /** Returns half of the screen height. */
    inline const SUInt8 GetHalfScreenHeight() const { return ScreenHeight / 2; }

    /** Returns Current Background color */
    inline WORD GetCurrentBackgroundColor() const { return CurrentBackgroundColor; }

    /** Clears the screen buffer. */
    void ClearBuffer();

    /** Clears the screen buffer at the given position */
    void ClearBufferAt(const SGridPositionU32& GridPosition, SSize Size);

    /** Writes to the screen buffer. */
    void Write(const SGridPositionU32& GridPosition, const SWString& Text, WORD Color = FG_WHITE);

    /** Writes to the screen buffer. */
    template<SSize Size>
    void Write(const SGridPositionU32& GridPosition, const SWString& Text, const SSpan<WORD, Size>& Colors);

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
    SUniquePtr<SWideChar[]> ScreenBuffer;

    /** Width of the screen buffer in characters. */
    SUInt8 ScreenWidth = 128;

    /** Height of the screen buffer in characters. */
    SUInt8 ScreenHeight = 48;

    /** Size of the buffer */
    SUInt16 BufferSize = ScreenWidth * ScreenHeight;

    /** Current background color */
    WORD CurrentBackgroundColor;

    /** Validates that a given write operation does not go beyond the console buffer. */
    SUInt32 ValidateWriteBounds(const SGridPositionU32& GridPosition, SSize TextLength);

    /** Locks the console window size by disabling resizing and maximizing. */
    void ConfigureConsoleWindow(HWND ConsoleHandle);
};

template<SSize Size>
void SConsoleRenderer::Write(const SGridPositionU32& GridPosition, const SWString& Text, const SSpan<WORD, Size>& Colors)
{
    // Check if the sizes are the same
    if (Text.size() != Colors.size())
    {
        S_LOG_WARNING(LogConsoleRenderer, TEXT("Text size - %d isn't equal to Colors size - %d"), Text.size(), Colors.size());
    }

    // Characters considered invalid for coloring
    SArray<SWideChar, 4> InvalidCharacters = { TEXT(' '), TEXT('\n'), TEXT('\r'), TEXT('\t') };

    // Lambda to check if a given character matches the first character of Text
    auto IsAnyOf = [&Text](SWideChar Character)
        {
            return Character == Text[0];
        };

    //Get the size of the text
    SSize TextSize = Text.size();

    // Apply color only if the text is not empty, is longer than one character,
    // and the first character is not considered invalid
    if (TextSize > 1 && !Text.empty())
    {
        if (std::find_if(InvalidCharacters.begin(), InvalidCharacters.end(), IsAnyOf) == InvalidCharacters.end())
        {
            // Iterate through each character
            for (SSize Index = 0; Index < Colors.size(); Index++)
            {
                // Set the text color at the current character's grid position
                SetTextColor(SGridPositionU32(GridPosition.first + static_cast<SUInt32>(Index), GridPosition.second), 1, Colors[Index]);
            }
        }
        else
        {
            S_LOG(LogConsoleRenderer, TEXT("Skipping SetTextColor: character '%c' is invalid."), Text[0]);
        }
    }

    // Write the text to the screen buffer
    swprintf(&ScreenBuffer[ValidateWriteBounds(GridPosition, TextSize)], BufferSize, Text.c_str());
}
