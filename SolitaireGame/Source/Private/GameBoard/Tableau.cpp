#include "SolitaireGamePCH.h"

SATableau::SATableau(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& NewCards) \
    : SAActor(NewGridPosition, NewWorld)
{
    // Initialize the Tableau (Genereate Columns)
	GenerateColumns(std::move(NewCards));
}

SATableau::SATableau(const SATableau& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

void SATableau::Write()
{
    // Counters for positioning cards in a grid layout
    SUInt32 CardsCounter = 0;
    SUInt32 ColumnCounter = 0;

    // Iterate through each group of cards
    for (const auto& Card : Cards)
    {
        // Reset vertical position counter for each new column
        CardsCounter = 0;

        // Iterate through each card in the current group
        for (const SSharedPtr<SACard>& VectorCard : Card)
        {
            // Write
            VectorCard->Write();

            // Move to the next row
            CardsCounter++;
        }

        // Move to the next column
        ColumnCounter++;
    }
}

void SATableau::ClearBuffer()
{
    // Iterate through each group of cards
    for (const auto& Card : Cards)
    {
        // Iterate through each card in the current group
        for (const SSharedPtr<SACard>& VectorCard : Card)
        {
            // Clear buffer
            VectorCard->ClearBuffer();
        }
    }
}

void SATableau::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SAFoundationList pointer
    if (const SATableau* OtherFoundationList = dynamic_cast<const SATableau*>(&Other))
    {
        // Callt the parent CopyFrom
        SAActor::CopyFrom(Other);

        // Loop through each column index
        for (SUInt32 Index = 0; Index < ColumnCount; Index++)
        {
            // Reference to the current vector of cards in 'this'
            SVector<SSharedPtr<SACard>>& TargetVector = Cards[Index];

            // Reference to the corresponding vector of cards in 'Other'
            const SVector<SSharedPtr<SACard>>& SourceVector = OtherFoundationList->Cards[Index];

            // Clear the current target vector before copying
            TargetVector.clear();

            // Pre-allocate memory to avoid reallocations
            TargetVector.reserve(SourceVector.size());

            // Deep copy each card from 'Other' to 'this'
            for (const auto& Card : SourceVector)
            {
                // Make sure the pointer is valid before copying
                if (Card)
                {
                    // Create a new card using copy constructor and add it to the target vector
                    TargetVector.push_back(std::make_shared<SACard>(*Card));
                }
            }
        }

        // Loop through each column in the card layout
        for (SUInt32 Index = 0; Index < ColumnCount; Index++)
        {
            // Get a reference to the current column of cards
            const SVector<SSharedPtr<SACard>>& Column = Cards[Index];

            // Traverse each card within the current column
            for (SUInt32 CardIndex = 0; CardIndex < Column.size(); CardIndex++)
            {
                // If there is a next card, link it to the current one
                if (CardIndex + 1 < Column.size())
                {
                    Column[CardIndex]->SetNextCard(Column[CardIndex + 1]);
                }
                else
                {
                    // If this is the last card in the column, set its next pointer to null
                    Column[CardIndex]->SetNextCard(nullptr);
                }
            }
        }
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SATableau."))
    }
}

void SATableau::GenerateColumns(SVector<SSharedPtr<SACard>>&& NewCards)
{
    // TODO debug it
    for (SUInt32 Index = 0; Index < 7; Index++)
    {
        // Create a temporary array to hold the cards for this column
        SVector<SSharedPtr<SACard>> TempCards;
        TempCards.reserve(Index + 1);

        for (SUInt32 IndexJ = 0; IndexJ <= Index; IndexJ++)
        {
            // Move card from NewCards to TempCards
            SSharedPtr<SACard> Card = std::move(NewCards.at(IndexJ));
            TempCards.push_back(Card);

            // Link the previous card to the current one (skip first row and first card)
            if (IndexJ > 0)
            {
                TempCards[IndexJ - 1]->SetNextCard(SGridPositionU32(GridPosition.first + (Index * 8), GridPosition.second + (IndexJ * 3)), TempCards[IndexJ]);
            }
            else
            {
                // Set grid position, IndexJ is 0
                TempCards[IndexJ]->SetGridPosition(SGridPositionU32(GridPosition.first + (Index * 8), GridPosition.second + (IndexJ * 3)));
            }
        }

        // To be sure set to nullptr
        TempCards.back()->SetNextCard(nullptr);

        // Move the built column into the tableau
        Cards[Index] = std::move(TempCards);

        // Flip the top card face-up (last in column)
        Cards[Index].back()->SetIsFaceUp(true);

        // Remove the used cards from NewCards
        NewCards.erase(NewCards.begin(), NewCards.begin() + Index + 1);
    }
}