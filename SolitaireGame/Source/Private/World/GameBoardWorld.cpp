#include "SolitaireGamePCH.h"
#include "Rules/SolitaireRules.h"
#include <random>

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

    // Number of cards to assign to the tableau
    constexpr SUInt8 TableauCardNum = 28;

    // Initialize Tableau with the first 28 cards, move them into the tableau vector
    Tableau = SpawnActor<SUniquePtr<SATableau>, SATableau>(SGridPositionU32(5, 20), AsShared<SGameBoardWorld>(), SVector<SSharedPtr<SACard>>(std::make_move_iterator(Cards.begin()), std::make_move_iterator(Cards.begin() + TableauCardNum)));

    // Remove the 28 cards moved to Tableau from the main card vector
    Cards.erase(Cards.begin(), Cards.begin() + TableauCardNum);

    // Create StockPile with the remaining cards
    StockPile = SpawnActor<SUniquePtr<SAStockPile>, SAStockPile>(SGridPositionU32(70, 10), AsShared<SGameBoardWorld>(), std::move(Cards));

    // Create solitaire game rules
    GameRules = SpawnActor<SUniquePtr<SASolitaireRules>, SASolitaireRules>(AsShared<SGameBoardWorld>());

    // Create FoundationList
    FoundationList = SpawnActor<SUniquePtr<SAFoundationList>, SAFoundationList>(AsShared<SGameBoardWorld>());

    // Create Move Manager
    MoveManager = SpawnActor<SUniquePtr<SSolitaireMoveManager>, SSolitaireMoveManager>(SGridPositionU32(100, 45), AsShared<SGameBoardWorld>(), TEXT("Type text :"));

    // Log
    S_LOG(LogGameBoard, TEXT("GameBoardWorld initialized."));

    // Initialization succeeded
    return true;
}