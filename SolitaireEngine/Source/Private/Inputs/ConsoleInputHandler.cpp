#include "SolitaireEnginePCH.h"
#include "Inputs/ConsoleInputHandler.h"

SConsoleInputHandler::SConsoleInputHandler() : ConsoleInputHandle(GetStdHandle(STD_INPUT_HANDLE)) {}

SConsoleInputHandler* SConsoleInputHandler::GetInstance()
{
	// Return raw pointer to the singleton instance.
	static SUniquePtr<SConsoleInputHandler> ConsoleInputHandler = std::make_unique<SConsoleInputHandler>();
	return ConsoleInputHandler.get();
}

bool SConsoleInputHandler::Initialize(const SGridPositionU32& NewGridPosition, const WORD NewTextColor, SConsoleLineCommitted&& OnLineComitted, const SSize NewLineSize)
{
    // Set the starting position
    SetGridPosition(NewGridPosition);

    // Set the maximum allowed input line size
    LineSize = NewLineSize;

    // Set Text color
    TextColor = NewTextColor;

    // Move callback
    ConsoleLineCommited = std::move(OnLineComitted);

    return true;
}

void SConsoleInputHandler::UpdateLine()
{
    DWORD Events = 0;

    // Get the number of console input events currently in the input buffer
    GetNumberOfConsoleInputEvents(ConsoleInputHandle, &Events);

    INPUT_RECORD Record;
    DWORD Read = 0;

    // Process all available input events
    while (Events--)
    {
        // Read one input event from the console input buffer
        ReadConsoleInputW(ConsoleInputHandle, &Record, 1, &Read);

        // Skip if the event is not a keyboard event
        if (Record.EventType != KEY_EVENT) continue;

        const auto& Key = Record.Event.KeyEvent;

        // Process only key-down events (ignore key releases)
        if (!Key.bKeyDown) continue;

        // Get the Unicode character corresponding to the key press
        SWideChar Char = Key.uChar.UnicodeChar;

        switch (Key.wVirtualKeyCode)
        {
            case VK_RETURN:
            {
                // Clear buffer 
                ClearBuffer();

                // Move the current input line to a temporary result
                SWString Result = std::move(CurrentLine);

                // Reset the input buffer for the next input cycle
                CurrentLine = TEXT("");

                // Invoke the line committed callback, if bound
                if (ConsoleLineCommited)
                {
                    ConsoleLineCommited(Result);
                }

                return;
            }
            case VK_ESCAPE:
            case VK_BACK:
            {
                // Escape key pressed — remove last character from current input line if any
                if (!CurrentLine.empty())
                {
                    CurrentLine.pop_back();
                }

                break;
            }
            default:
            {
                // For all other keys, append printable characters (ASCII 32 and above) to current line and check if the Current lize is < LineSize
                if (Char >= 32 && CurrentLine.size() < LineSize)
                {
                    CurrentLine += Char;
                }

                break;
            }
        }
    }
}

void SConsoleInputHandler::Write()
{
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();
    ConsoleRenderer->Write(GridPosition, CurrentLine, static_cast<SUInt32>(LineSize), true, TextColor | ConsoleRenderer->GetCurrentBackgroundColor());
}

void SConsoleInputHandler::ClearBuffer()
{
    SConsoleRenderer::GetInstance()->ClearBufferAt(GridPosition, CurrentLine.size());
}