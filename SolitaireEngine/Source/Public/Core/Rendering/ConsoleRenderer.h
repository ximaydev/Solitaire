#pragma once
#include "Globals.h"
#include "Core/Utility/Colors.h"
#include <Windows.h>

using namespace ConsoleColor;

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

    /** Writes to the screen buffer. */
    void Write(const SGridPosition<SUInt32, SUInt32>& Position, const SWString& Text, WORD Color = WHITE);

    /** Performs the main draw routine. */
    void Draw();

    /** Shows or hides the console cursor. */
    bool ShowConsolCursor(SBool bShowCursor);

    /** Draws a horizontal panel starting at the specified grid position. */
    void DrawHorizontalPanel(const SGridPosition<SUInt32, SUInt32>& GridPosition, const SUInt32 Width, WORD Color = WHITE, const SWString& Character = TEXT(""));

    /** Draws a vertical panel starting at the specified grid position. */
    void DrawVerticalPanel(const SGridPosition<SUInt32, SUInt32>& GridPosition, const SUInt32 Height, WORD Color = WHITE, const SWString& Character = TEXT(""));

    /** Draws a filled rectangular panel. */
    void DrawPanel(const SGridPosition<SUInt32, SUInt32>& GridPosition, const SUInt32 Width, const SUInt32 Height, WORD Color = WHITE, const SWString& Character = TEXT(""), const SWString& HeightCharacter = TEXT(""));

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

    /** Draws the console window border. */
    void DrawBorder();

    /** Sets text color for a specific region in the buffer. */
    bool SetTextColor(const SGridPosition<SInt32, SInt32>& GridPosition, const SUInt32 Size, WORD Color = WHITE);

    /** Locks the console window size by disabling resizing and maximizing. */
    void LockConsoleResize(HWND ConsoleHandle);
};