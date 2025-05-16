#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Handles player keyboard input in the custom console and stores line - by - line commands */
class SOLITAIRE_ENGINE_API SConsoleInputHandler : public SIConsoleRenderable
{
public:
    /** Constructor */
    SConsoleInputHandler();

    /** Returns the singleton instance of the Console Input Handler. */
    static SConsoleInputHandler* GetInstance();

    /** Initialize Console Input Handler */
    bool Initialize(const SGridPositionU32& NewGridPosition, const WORD TextColor, SConsoleLineCommitted&& OnLineComitted, const SSize NewLineSize = 0);

    /** Reads all available characters from the input buffer and updates the current line. If the Enter key is pressed, the line is marked as completed and ready to be consumed. */
    void UpdateLine();

    /** Get Currente Line */
    inline const SWString& GetCurrentLine() const { return CurrentLine; }

    /** Renders the line at its internal grid position. */
    virtual void Write() override;

    /** Clears the line from the console using its internal grid position. */
    virtual void ClearBuffer() override;

protected:

    /** Stores the characters typed by the user until Enter is pressed. */
    SWString CurrentLine;

    /** Maximum number of characters allowed in the current line */
    SSize LineSize = 10;

    /** Color of the text. It should be only FG color */
    WORD TextColor = FG_BLACK;

    /** Handle to the Windows console input buffer. */
    HANDLE ConsoleInputHandle = nullptr;

    /** Function called when user press enter */
    SConsoleLineCommitted ConsoleLineCommited;
};