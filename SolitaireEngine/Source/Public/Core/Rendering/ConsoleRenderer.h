#pragma once
#include "Globals.h"
#include "Core/Utility/Colors.h"
#include <Windows.h>

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

    /** Destructor */
    virtual ~SIConsoleRenderable() = default;

    /** Get Grid Position. */
    inline const SGridPositionU32& GetGridPosition() const { return GridPosition; }

    /** Set Grid Position */
    inline void SetGridPosition(const SGridPositionU32& NewGridPosition) { GridPosition = NewGridPosition; }

    /** Renders the object at its internal grid position.  */
    virtual void Write();

    /** Clears the object from the console using its internal grid position. */
    virtual void ClearBuffer();

    /** Renders the object at a specified grid position. */
    virtual void WriteAt(const SGridPositionU32& InGridPosition) = 0;

    /** Clears the object from a specified grid position.  */
    virtual void ClearBufferAt(const SGridPositionU32& InGridPosition) = 0;

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

    /** Clears the screen buffer. */
    void ClearBuffer();

    /** Clears the screen buffer at the given position */
    void ClearBufferAt(const SGridPositionU32& GridPosition, SSize Size);

    /** Writes to the screen buffer. */
    void Write(const SGridPositionU32& Position, const SWString& Text, WORD Color = WHITE);

    /** Performs the main draw routine. */
    void Draw();

    /** Shows or hides the console cursor. */
    bool ShowConsolCursor(SBool bShowCursor);

    /** Draws a horizontal panel starting at the specified grid position. */
    void DrawHorizontalPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, WORD Color = WHITE, const SWString& Character = TEXT(""));

    /** Draws a vertical panel starting at the specified grid position. */
    void DrawVerticalPanel(const SGridPositionU32& GridPosition, const SUInt32 Height, WORD Color = WHITE, const SWString& Character = TEXT(""));

    /** Draws a filled rectangular panel. */
    void DrawPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, const SUInt32 Height, WORD Color = WHITE, const SWString& WidthCharacter = TEXT(""), const SWString& HeightCharacter = TEXT(""));

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

    SUInt16 BufferSize = ScreenWidth * ScreenHeight;

    /** Validates that a given write operation does not go beyond the console buffer. */
    SUInt32 ValidateWriteBounds(const SGridPositionU32& GridPosition, SSize TextLength);

    /** Draws the console window border. */
    void DrawBorder();

    /** Sets text color for a specific region in the buffer. */
    bool SetTextColor(const SGridPositionU32& GridPosition, const SSize Size, WORD Color = WHITE);

    /** Locks the console window size by disabling resizing and maximizing. */
    void LockConsoleResize(HWND ConsoleHandle);
};