#include "SolitaireEnginePCH.h"
#include "Framework/ConsolePrompt.h"
#include "Inputs/ConsoleInputHandler.h"

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow, const SConsoleLineCommitted& NewCallback)
    : SAActor(NewGridPosition), TextColor(NewTextColor), TextToShow(NewTextToShow), ConsolePromptCallback(NewCallback) {}

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow)
    : SAConsolePrompt(NewGridPosition, NewTextColor, NewTextToShow, nullptr) {}

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition) : SAConsolePrompt(NewGridPosition, FG_WHITE, TEXT("")) {}

bool SAConsolePrompt::Initialize()
{
    // Calculate the input start position: just after the prompt text
    const SGridPositionU32 ConsoleInputHandlerGridPosition = { GridPosition.first + TextToShow.size() + 1, GridPosition.second };

    // Compute maximum input length from current cursor position to screen edge (minus 2 for margin)
    const SSize MaxInputLength = static_cast<SSize>(SConsoleRenderer::GetInstance()->GetScreenWidth() - ConsoleInputHandlerGridPosition.first - 2);

    // Initialize the Console Input Handler with position and input length limit
    SConsoleInputHandler::GetInstance()->Initialize(ConsoleInputHandlerGridPosition, TextColor, std::move(ConsolePromptCallback), MaxInputLength);

    return true;
}

void SAConsolePrompt::Write()
{
    // Get the console renderer singleton
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Write the prompt text at the specified position with foreground and current background color
    ConsoleRenderer->Write(GridPosition, TextToShow, static_cast<SUInt32>(TextToShow.size()), true, TextColor | ConsoleRenderer->GetCurrentBackgroundColor());
}

void SAConsolePrompt::ClearBuffer()
{
    // Clear the prompt text from the console buffer
    SConsoleRenderer::GetInstance()->ClearBufferAt(GridPosition, TextToShow.size() + 1);

    // Clear the user's input buffer (the typed line)
    SConsoleInputHandler::GetInstance()->ClearBuffer();
}