#include "SolitaireGamePCH.h"

SAStockPile::SAStockPile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& InitialCards) 
    : SAActor(NewGridPosition, NewWorld)
{
    // Fill the Stock Pile with cards
    FillInitialCards(std::move(InitialCards));
}

SAStockPile::SAStockPile(const SAStockPile& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
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

void SAStockPile::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SAStockPile pointer
    if (const SAStockPile* OtherStockPile = dynamic_cast<const SAStockPile*>(&Other))
    {
        // Call the parent CopyFrom
        SAActor::CopyFrom(Other);

        // Clear current Cards container to prepare for copying new cards
        Cards.clear();

        // Reserve memory upfront to avoid reallocations during copying
        Cards.reserve(OtherStockPile->Cards.size());

        // Deep copy each card from the other WastePile's Cards vector
        for (const auto& Card : OtherStockPile->Cards)
        {
            // Create a new shared pointer with a copy of the card and move it into this container
            Cards.push_back(std::move(std::make_shared<SACard>(*Card)));
        }
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SAStockPile."))
    }
}

void SAStockPile::FillInitialCards(SVector<SSharedPtr<SACard>>&& InitialCards)
{
    // Get the grid position of the stock pile (used as the position for all incoming cards)
    const SGridPositionU32& StockPileGridPosition = GetGridPosition();

    for (SSharedPtr<SACard>& Card : InitialCards)
    {
        // assign the stock pile's grid position to the card
        Card->SetGridPosition(StockPileGridPosition);

        // Ensure the card is face-down when placed in the stock pile
        Card->SetIsFaceUp(false);
    }

    // Reserve enough space in the Cards vector to avoid reallocations
    Cards.reserve(InitialCards.size());

    // Insert the initial cards at the beginning of the pile using move semantics (this ensures efficient transfer without unnecessary copies)
    Cards.insert(Cards.begin(), std::make_move_iterator(InitialCards.begin()), std::make_move_iterator(InitialCards.end()));

    // Remove the moved cards
    InitialCards.erase(InitialCards.begin(), InitialCards.end());
}