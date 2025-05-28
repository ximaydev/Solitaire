#include "SolitaireGamePCH.h"
#include "Rules/SolitaireRules.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"
#include <random>
#include "Globals.h"
#include "Framework/ConsoleSelector.h"
#include "Utils/UndoManager.h"
#include "GameBoard/Helpers/OperationsHelpActor.h"

SGameBoardWorld::SGameBoardWorld(const SGameBoardWorld& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

SBool SGameBoardWorld::Initialize()
{
    // Create a temporary vector to hold all 52 cards for the deck
    SVector<SSharedPtr<SACard>> Cards;
    Cards.reserve(52);

    // Loop through all suits (1 to 4)
    for (SUInt8 IndexSuit = 1; IndexSuit <= 4; IndexSuit++)
    {
        // Loop through all ranks (1 to 13)
        for (SUInt8 IndexRank = 1; IndexRank <= 13; IndexRank++)
        {
            // Create a card with default grid position (0,0), face down
            SSharedPtr<SACard> Card = std::make_shared<SACard>(SGridPositionU32(0, 0), AsShared<SGameBoardWorld>(), FCardInfo(static_cast<ECardRank>(IndexRank), static_cast<ECardSuit>(IndexSuit), false));
            Cards.push_back(std::move(Card));
        }
    }

    // Log
    S_LOG(LogGameBoard, TEXT("Generated total of %d cards."), Cards.size());

    // Initialize random number generator for shuffling
    std::random_device RandomDevice;
    std::mt19937 Generator(RandomDevice());

    // Shuffle the cards randomly
    std::shuffle(Cards.begin(), Cards.end(), Generator);
    S_LOG(LogGameBoard, TEXT("Shuffled the card deck."));

    // Define the initializer list with action name and callback
    const SInitializerList<SPair<SWStringView, SCallback>> ConsoleSelectorActions =
    {
       { TEXT("Undo Move"), std::bind(&SGameBoardWorld::HandleUndoMove, this) }
    };

    // Spawn the actor with the initializer list, position, and owner
    SpawnActor<SAConsoleSelector>(ConsoleSelectorActions, SGridPositionU32(30, 45), AsShared());

    // Spawn actor which displays the options for user
    SpawnActor<SAOperationsHelpActor>(SGridPositionU32(65, 36), AsShared());

    // Create solitaire game rules
    GameRules = SpawnActor<SASolitaireRules>(AsShared());

    // Create Move Manager
    MoveManager = SpawnActor<SSolitaireMoveManager>(SGridPositionU32(65, 43), AsShared());
    SpawnActor<SAConsolePrompt>(SGridPositionU32(65, 42), AsShared(), FG_DARK_GRAY | SConsoleRenderer::GetInstance()->GetCurrentBackgroundColor(), TEXT("Choose an operation:"), std::bind(&SSolitaireMoveManager::OnEnterClicked, MoveManager, std::placeholders::_1));

    // Number of cards to assign to the tableau
    constexpr SUInt8 TableauCardNum = 28;

    // Initialize Tableau with the first 28 cards, move them into the tableau vector
    Tableau = SpawnActor<SATableau>(SGridPositionU32(5, 4), AsShared(), SVector<SSharedPtr<SACard>>(std::make_move_iterator(Cards.begin()), std::make_move_iterator(Cards.begin() + TableauCardNum)));

    // Remove the 28 cards moved to Tableau from the main card vector
    Cards.erase(Cards.begin(), Cards.begin() + TableauCardNum);

    // Create StockPile with the remaining cards
    StockPile = SpawnActor<SAStockPile>(SGridPositionU32(100, 5), AsShared(), std::move(Cards));

    // Create WastePile
    WastePile = SpawnActor<SAWastePile>(SGridPositionU32(StockPile->GetGridPosition().first + 10, StockPile->GetGridPosition().second), AsShared());

    // Create FoundationList
    FoundationList = SpawnActor<SAFoundationList>(AsShared(), SGridPositionU32(100, 15));
    
    // Log
    S_LOG(LogGameBoard, TEXT("GameBoardWorld initialized."));

    // Initialization succeeded
    return true;
}

void SGameBoardWorld::Write()
{
    // Call parent function
    SWorld::Write();

    // Get Console Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Format and render the move count string
    SWString MovesString = std::format(TEXT("Moves: {}"), MoveCount);
    ConsoleRenderer->Write(SGridPositionU32(115, 45), MovesString, static_cast<SUInt32>(MovesString.size()), true, FG_BLUE | ConsoleRenderer->GetCurrentBackgroundColor());
}

void SGameBoardWorld::CopyFrom(const SWorld& Other)
{
    // Attempt to cast the base class reference to a SGameBoardWorld pointer
    if (const SGameBoardWorld* OtherGameBoardWorld = dynamic_cast<const SGameBoardWorld*>(&Other))
    {
        // Clear the current list of actors to prepare for deep copy
        Actors.clear();

        // Reserve memory upfront based on the size of the source Actors vector
        Actors.reserve(OtherGameBoardWorld->Actors.size());

        // Iterate over each actor in the source object's Actors list
        for (const auto& ActorPtr : OtherGameBoardWorld->Actors)
        {
            // Create a deep copy of the actor by calling the copy constructor
            SSharedPtr<SAActor> NewActor = ActorPtr->Clone();

            // Add the newly copied actor to the current object's Actors list
            Actors.push_back(NewActor);

            // For specific members, perform type-safe casting and assign NewActor
            if (ActorPtr.get() == OtherGameBoardWorld->Tableau.get())
            {
                Tableau = std::static_pointer_cast<SATableau>(NewActor);
            }
            else if (ActorPtr.get() == OtherGameBoardWorld->FoundationList.get())
            {
                FoundationList = std::static_pointer_cast<SAFoundationList>(NewActor);
            }
            else if (ActorPtr.get() == OtherGameBoardWorld->StockPile.get())
            {
                StockPile = std::static_pointer_cast<SAStockPile>(NewActor);
            }
            else if (ActorPtr.get() == OtherGameBoardWorld->WastePile.get())
            {
                WastePile = std::static_pointer_cast<SAWastePile>(NewActor);
            }
            else if (ActorPtr.get() == OtherGameBoardWorld->MoveManager.get())
            {
                MoveManager = std::static_pointer_cast<SSolitaireMoveManager>(NewActor);
            }
            else if (ActorPtr.get() == OtherGameBoardWorld->GameRules.get())
            {
                GameRules = std::static_pointer_cast<SASolitaireRules>(NewActor);
            }
        }

        // Copy the MoveCount value from the source object
        MoveCount = OtherGameBoardWorld->MoveCount;
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SGameBoardWorld."))
    }
}

void SGameBoardWorld::HandleUndoMove()
{
    // Undo Move
    if (SSharedPtr<SGameBoardWorld> OldGameBoardWorld = SUndoManager<SGameBoardWorld>::GetInstance()->Undo())
    {
        GSolitaireEngine->SetCurrentWorld(std::move(OldGameBoardWorld));
    }
}