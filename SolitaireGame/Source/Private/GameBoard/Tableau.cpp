#include "SolitaireGamePCH.h"
#include "GameBoard/Tableau.h"
#include "GameBoard/Card.h"

SATableau::SATableau(const SGridPositionU32& NewGridPosition, SVector<SSharedPtr<SACard>>&& NewCards)
	: SAActor(NewGridPosition)
{
    // Initialize the Tableau (Genereate Columns)
	GenerateColumns(std::move(NewCards));
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
        Cards[Index].back()->GetCardInfo_Mutable().IsFaceUp = true;

        // Remove the used cards from NewCards
        NewCards.erase(NewCards.begin(), NewCards.begin() + Index + 1);
    }
}