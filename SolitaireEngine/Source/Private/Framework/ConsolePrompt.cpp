#include "SolitaireEnginePCH.h"
#include "Framework/ConsolePrompt.h"
#include "Inputs/ConsoleInputHandler.h"

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, WORD NewTextColor, const SWString& NewTextToShow, const SConsoleLineCommitted& NewCallback)
    : SAActor(NewGridPosition, NewWorld), TextColor(NewTextColor), TextToShow(NewTextToShow), ConsolePromptCallback(NewCallback) 
{
    // Calculate the input start position: just after the prompt text
    const SGridPositionU32 ConsoleInputHandlerGridPosition = { GridPosition.first + static_cast<SUInt32>(TextToShow.size()), GridPosition.second };

    // Compute maximum input length from current cursor position to screen edge (minus 2 for margin)
    const SSize MaxInputLength = static_cast<SSize>(SConsoleRenderer::GetInstance()->GetScreenWidth() - ConsoleInputHandlerGridPosition.first - 2);

    // Initialize the Console Input Handler with position and input length limit
    SConsoleInputHandler::GetInstance()->Initialize(ConsoleInputHandlerGridPosition, TextColor, std::move(ConsolePromptCallback), MaxInputLength);
}

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, WORD NewTextColor, const SWString& NewTextToShow)
    : SAConsolePrompt(NewGridPosition, NewWorld, NewTextColor, NewTextToShow, nullptr) {}

SAConsolePrompt::SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) 
    : SAConsolePrompt(NewGridPosition, NewWorld, FG_WHITE, TEXT("")) {}

SAConsolePrompt::SAConsolePrompt(const SAConsolePrompt& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
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

void SAConsolePrompt::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SGameBoardWorld pointer
    if (const SAConsolePrompt* OtherConsolePrompt = dynamic_cast<const SAConsolePrompt*>(&Other))
    {
        // Call the parent CopyFrom
        SAActor::CopyFrom(Other);

        // Copy the text content that should be displayed in the console prompt
        TextToShow = OtherConsolePrompt->TextToShow;

        // Copy the color in which the prompt text should be rendered
        TextColor = OtherConsolePrompt->TextColor;

        // Copy the callback function that will be invoked when the user submits input
        ConsolePromptCallback = OtherConsolePrompt->ConsolePromptCallback;
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SGameBoardWorld."))
    }
}

void SAConsolePrompt::SetConsolePromptCallback(SConsoleLineCommitted NewConsolePromptCallback)
{
    // Update Console Input Handler and set Console Propmpt Callback
    SConsoleInputHandler::GetInstance()->SetConsoleLineCommited(NewConsolePromptCallback);
    ConsolePromptCallback = NewConsolePromptCallback;
}