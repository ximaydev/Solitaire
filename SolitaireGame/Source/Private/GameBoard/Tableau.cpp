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
            // Assign grid position based on current column and row offsets
            VectorCard->SetGridPosition(SGridPositionU32(GridPosition.first + (ColumnCounter * 8), GridPosition.second + (CardsCounter * 3)));
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
    for (SUInt32 Index = 0; Index < 7; Index++)
    {
        // Move 'Index' cards from the beginning of NewCards to the current column
        Cards[Index] = SVector<SSharedPtr<SACard>>(std::make_move_iterator(NewCards.begin()), std::make_move_iterator(NewCards.begin() + Index + 1));

        // Flip the top card in the current column face-up
        Cards[Index].back()->GetCardInfo_Mutable().IsFaceUp = true;

        // Remove the moved cards from NewCards
        NewCards.erase(NewCards.begin(), NewCards.begin() + Index + 1);
    }
}