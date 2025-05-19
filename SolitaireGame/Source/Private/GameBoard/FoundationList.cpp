#include "SolitaireGamePCH.h"
#include "GameBoard/FoundationList.h"
#include "Card/Card.h"

bool SAFoundationList::AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position)
{
    // Get the foundation vector at the given position
    SVector<SSharedPtr<SACard>>& Cards = FoundationList.at(Position);

    // If the vector is empty, only an Ace can be placed
    if (Cards.empty())
    {
        if (NewCard->GetCardInfo().GetCardRank() == ECardRank::Ace)
        {
            // Push card to Cards
            Cards.push_back(NewCard);

            // Set Card Info
            SetCardInfo(NewCard.get(), Position);

            S_LOG(LogGameBoard, TEXT("Successfully added Ace to an empty foundation."));
            return true;
        }
        else
        {
            S_LOG(LogGameBoard, TEXT("Cannot add card to empty foundation. Only an Ace is allowed."));
            return false;
        }
    }

    // Get the last card on the foundation vector
    const SSharedPtr<SACard>& LastCard = Cards.back();

    // Get card info
    const FCardInfo& LastCardInfo = LastCard->GetCardInfo();
    const FCardInfo& NewCardInfo = NewCard->GetCardInfo();

    // Check if suits match
    if (LastCardInfo.GetCardSuit() == NewCardInfo.GetCardSuit())
    {
        // Check if rank is exactly one higher
        if ((static_cast<SUInt8>(LastCardInfo.GetCardRank()) + 1) == static_cast<SUInt8>(NewCardInfo.GetCardRank()))
        {
            // Push card to Cards
            Cards.push_back(NewCard);

            // Set Card Info
            SetCardInfo(NewCard.get(), Position);

            S_LOG(LogGameBoard, TEXT("Successfully added card to the foundation."));
            return true;
        }
        else
        {
            S_LOG(LogGameBoard, TEXT("Cannot add card. Rank must be exactly one higher."));
        }
    }
    else
    {
        S_LOG(LogGameBoard, TEXT("Cannot add card. Suits do not match."));
    }

    return false;
}

void SAFoundationList::SetCardInfo(SACard* Card, SUInt8 Position)
{
    // Set card position on grid
    Card->SetGridPosition(SGridPositionU32(GridPosition.first + (Position * 8), GridPosition.second));

    // Make card face-up
    Card->GetCardInfo_Mutable().IsFaceUp = true;
}

void SAFoundationList::Write()
{
    // Write card data for all top cards in foundation stacks
    for (const auto& Element : FoundationList)
    {
        Element.back()->Write();
    }
}

void SAFoundationList::ClearBuffer()
{
    // Clear render data for all top cards in foundation stacks
    for (const auto& Element : FoundationList)
    {
        Element.back()->ClearBuffer();
    }
}