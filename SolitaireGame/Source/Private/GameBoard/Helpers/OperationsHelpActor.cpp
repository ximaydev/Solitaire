#include "SolitaireGamePCH.h"
#include "GameBoard/Helpers/OperationsHelpActor.h"

SAOperationsHelpActor::SAOperationsHelpActor(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SAActor(NewGridPosition, NewWorld)
{
    // Reserve memory for the operations vector to avoid reallocations
    Operations.reserve(5);

    // Initialize the list of operations to display
    Operations.push_back(TEXT("1 - Move a card from tableau to another tableau column"));
    Operations.push_back(TEXT("2 - Move a card from tableau to foundation"));
    Operations.push_back(TEXT("3 - Move a card from the waste pile to tableau"));
    Operations.push_back(TEXT("4 - Move a card from the waste pile to foundation"));
    Operations.push_back(TEXT("5 - Draw the next set of cards from the stock pile"));
    Operations.push_back(TEXT("6 - Abort move"));
}

SAOperationsHelpActor::SAOperationsHelpActor(const SAOperationsHelpActor& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

void SAOperationsHelpActor::Write()
{
    // Get Console Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Write each help line to a fixed screen position
    ConsoleRenderer->Write(SGridPositionU32(65, 36), Operations[0], static_cast<SUInt32>(Operations[0].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
    ConsoleRenderer->Write(SGridPositionU32(65, 37), Operations[1], static_cast<SUInt32>(Operations[1].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
    ConsoleRenderer->Write(SGridPositionU32(65, 38), Operations[2], static_cast<SUInt32>(Operations[2].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
    ConsoleRenderer->Write(SGridPositionU32(65, 39), Operations[3], static_cast<SUInt32>(Operations[3].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
    ConsoleRenderer->Write(SGridPositionU32(65, 40), Operations[4], static_cast<SUInt32>(Operations[4].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
    ConsoleRenderer->Write(SGridPositionU32(65, 41), Operations[5], static_cast<SUInt32>(Operations[5].size()), true, FG_BLACK | ConsoleRenderer->GetCurrentBackgroundColor());
}

void SAOperationsHelpActor::ClearBuffer()
{
    // Get Console Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Get starting position of the help text block
    const SInt32 StartX = GridPosition.first;
    const SInt32 StartY = GridPosition.second;

    // Clear each line where help operations were previously rendered
    for (SUInt32 Index = 0; Index < Operations.size(); Index++)
    {
        ConsoleRenderer->ClearBufferAt(SGridPositionU32(StartX, StartY + Index), Operations[Index].size());
    }
}

void SAOperationsHelpActor::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SAOperationsHelpActor pointer
    if (const SAOperationsHelpActor* OtherOperationsHelpActor = dynamic_cast<const SAOperationsHelpActor*>(&Other))
    {
        // Copy the operations text from the source actor
        Operations = OtherOperationsHelpActor->Operations;
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SAOperationsHelpActor."))
    }
}