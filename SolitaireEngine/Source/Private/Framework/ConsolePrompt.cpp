#include "SolitaireEnginePCH.h"
#include "Framework/ConsolePrompt.h"
#include "Inputs/ConsoleInputHandler.h"

SConsolePrompt::SConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow, const SConsoleLineCommitted& NewCallback)
    : SAActor(NewGridPosition), TextColor(NewTextColor), TextToShow(NewTextToShow), ConsolePromptCallback(NewCallback) {}

SConsolePrompt::SConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow)
    : SConsolePrompt(NewGridPosition, NewTextColor, NewTextToShow, nullptr) {}

SConsolePrompt::SConsolePrompt(const SGridPositionU32& NewGridPosition) : SConsolePrompt(NewGridPosition, FG_WHITE, TEXT("")) {}

bool SConsolePrompt::Initialize()
{
    // Calculate the input start position: just after the prompt text
    const SGridPositionU32 ConsoleInputHandlerGridPosition = { GridPosition.first + TextToShow.size() + 1, GridPosition.second };

    // Compute maximum input length from current cursor position to screen edge (minus 2 for margin)
    const SSize MaxInputLength = static_cast<SSize>(SConsoleRenderer::GetInstance()->GetScreenWidth() - ConsoleInputHandlerGridPosition.first - 2);

    // Initialize the Console Input Handler with position and input length limit
    SConsoleInputHandler::GetInstance()->Initialize(ConsoleInputHandlerGridPosition, TextColor, std::move(ConsolePromptCallback), MaxInputLength);

    return true;
}

void SConsolePrompt::Write()
{
    // Get the console renderer singleton
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Write the prompt text at the specified position with foreground and current background color
    ConsoleRenderer->Write(GridPosition, TextToShow, static_cast<SUInt32>(TextToShow.size()), true, TextColor | ConsoleRenderer->GetCurrentBackgroundColor());
}

void SConsolePrompt::ClearBuffer()
{
    // Clear the prompt text from the console buffer
    SConsoleRenderer::GetInstance()->ClearBufferAt(GridPosition, TextToShow.size() + 1);

    // Clear the user's input buffer (the typed line)
    SConsoleInputHandler::GetInstance()->ClearBuffer();
}