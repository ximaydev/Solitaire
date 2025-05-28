#include "SolitaireGamePCH.h"

SAFoundationList::SAFoundationList(SSharedPtr<SWorld> NewWorld, const SGridPositionU32& NewGridPosition) : SAActor(NewGridPosition, NewWorld) {}

SAFoundationList::SAFoundationList(const SAFoundationList& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

bool SAFoundationList::AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position)
{
    // Get the foundation vector at the given position
    SVector<SSharedPtr<SACard>>& Cards = FoundationList.at(Position);

    // Set Is Face Up to true
    NewCard->SetIsFaceUp(true);

    // If the vector is empty, only an Ace can be placed
    if (Cards.empty())
    {
        if (NewCard->GetCardInfo().GetCardRank() == ECardRank::Ace)
        {
            // Push card to Cards
            Cards.push_back(NewCard);

            // Set Grid Position
            NewCard->SetGridPosition(SGridPositionU32(GridPosition.first + (Position * 8), GridPosition.second));

            S_LOG(LogGameBoard, TEXT("Successfully added Ace to an empty foundation."));
            return true;
        }
        else
        {
            S_LOG(LogGameBoard, TEXT("Cannot add card to empty foundation. Only an Ace is allowed."));
            return false;
        }
    }

    // Set Next card for the penultimate card
    Cards.back()->SetNextCard(SGridPositionU32(GridPosition.first + (Position * 8), GridPosition.second), NewCard);

    // Push card to Cards
    Cards.push_back(NewCard);

    return false;
}

void SAFoundationList::Write()
{
    // Write card data for all top cards in foundation stacks
    for (const auto& Element : FoundationList)
    {
        // Check if the vector is greater than zero
        if (Element.size() > 0)
        {
            Element.back()->Write();
        }
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

void SAFoundationList::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SAFoundationList pointer
    if (const SAFoundationList* OtherFoundationList = dynamic_cast<const SAFoundationList*>(&Other))
    {
        // Call the parent CopyFrom
        SAActor::CopyFrom(Other);

        // Deep-copy all card piles
        for (SUInt32 Index = 0; Index < OtherFoundationList->FoundationList.size(); Index++)
        {
            // Create a new card pile
            SVector<SSharedPtr<SACard>> TargetCardPile;

            // Get source pile
            const SVector<SSharedPtr<SACard>>& SourceCardPile = OtherFoundationList->FoundationList[Index];

            // Preallocate memory
            TargetCardPile.reserve(SourceCardPile.size());

            // Deep copy each card
            for (SUInt32 CardIndex = 0; CardIndex < SourceCardPile.size(); CardIndex++)
            {
                SSharedPtr<SACard> ClonedCard = std::make_shared<SACard>(*SourceCardPile[CardIndex]);
                TargetCardPile.push_back(std::move(ClonedCard));
            }

            // Save the new pile
            FoundationList[Index] = std::move(TargetCardPile);
        }

        // Link 'NextCard' pointers for each foundation column
        for (SUInt32 Index = 0; Index < FoundationList.size(); Index++)
        {
            // Reference to the current column (pile of cards)
            SVector<SSharedPtr<SACard>>& TargetCardPile = FoundationList[Index];

            // Traverse each card in the pile
            for (SUInt32 CardIndex = 0; CardIndex < TargetCardPile.size(); CardIndex++)
            {
                // If there is a next card, link it
                if (CardIndex + 1 < TargetCardPile.size())
                {
                    TargetCardPile[CardIndex]->SetNextCard(TargetCardPile[CardIndex + 1]);
                }
                else
                {
                    // Last card in the pile — no next card
                    TargetCardPile[CardIndex]->SetNextCard(nullptr);
                }
            }
        }
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SAFoundationList."))
    }
}