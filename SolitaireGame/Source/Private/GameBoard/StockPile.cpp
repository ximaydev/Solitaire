#include "SolitaireGamePCH.h"
#include "GameBoard/StockPile.h"
#include "GameBoard/WastePile.h"
#include "Card/Card.h"
#include "Framework/World.h"

SAStockPile::SAStockPile(const SGridPositionU32& NewGridPosition) : SAActor(NewGridPosition) {}

SAStockPile::SAStockPile(const SGridPositionU32& NewGridPosition, SVector<SSharedPtr<SACard>>& InitialCards) : SAActor(NewGridPosition)
{
    FillInitialCards(InitialCards);
}

bool SAStockPile::Initialize()
{
    return InitializeWastePile();
}

SSharedPtr<SACard> SAStockPile::GetTopCard() const
{
    // Check if there are any cards in the pile
    if (!Cards.empty())
    {
        // Return the first card in the collection (the visible top card)
        return Cards.back();
    }

    // Return nullptr if the pile is empty (no visible card)
    return nullptr;
}

void SAStockPile::RemoveCardFromPile(SSharedPtr<SACard> CardToRemove)
{
    // Remove the specified card from the Cards vector:
    Cards.erase(std::remove(Cards.begin(), Cards.end(), CardToRemove), Cards.end());
}

void SAStockPile::Write()
{
    // Check if there is at least one card in the pile
    if (!Cards.empty())
    {
        // Write (display) the top card
        Cards.back()->Write();
    }
}

void SAStockPile::UseStockPile()
{
    /** Moves cards to the WastePile */
    WastePile->MoveCardsToWastePile(Cards);
}

bool SAStockPile::InitializeWastePile()
{
    // Check if the WastePile isn't nullptr
    if (WastePile)
    {
        S_LOG_WARNING(LogCard, TEXT("WastePile is already initialized."));
        return false;
    }

    // Create a new WastePile positioned 10 units to the right of the StockPile
    const SGridPositionU32& StockPileGridPosition = GetGridPosition();
    WastePile = GetWorld()->SpawnActor<SUniquePtr<SAWastePile>, SAWastePile>(SGridPositionU32(StockPileGridPosition.first + 10, StockPileGridPosition.second));
    return true;
}

void SAStockPile::FillInitialCards(SVector<SSharedPtr<SACard>>& InitialCards)
{
    // Get the grid position of the stock pile (used as the position for all incoming cards)
    const SGridPositionU32& StockPileGridPosition = GetGridPosition();

    for (SSharedPtr<SACard>& Card : InitialCards)
    {
        // Assign the stock pile's grid position to the card
        Card->SetGridPosition(StockPileGridPosition);

        // Ensure the card is face-down when placed in the stock pile
        Card->GetCardInfo_Mutable().IsFaceUp = false;
    }

    // Reserve enough space in the Cards vector to avoid reallocations
    Cards.reserve(Cards.size() + InitialCards.size());

    // Insert the initial cards at the beginning of the pile using move semantics (this ensures efficient transfer without unnecessary copies)
    Cards.insert(Cards.begin(), std::make_move_iterator(InitialCards.begin()), std::make_move_iterator(InitialCards.end()));

    // Remove the moved cards
    InitialCards.erase(InitialCards.begin(), InitialCards.end());
}