#include "SolitaireGamePCH.h"
#include "GameBoard/WastePile.h"
#include "Card/Card.h"

SAWastePile::SAWastePile(const SGridPositionU32& NewGridPosition) : SAActor(NewGridPosition) {}

SSharedPtr<SACard> SAWastePile::UseTopCard()
{
	// Check if Cards is empty
	if (Cards.empty())
		return nullptr;

	// Get the card at the front (top) of the pile
	SSharedPtr<SACard> TopCard = Cards.back();

	// Remove the last card from the vector (the one we just took)
	Cards.erase(Cards.end());

	// Return the card that was removed
	return TopCard;
}

void SAWastePile::Write()
{
	// Check if there is at least one card in the pile
	if (!Cards.empty())
	{
		// Iterate over all cards in the waste pile and write their state
		for (const auto& Card : Cards)
		{
			Card->Write();
		}
	}
}

void SAWastePile::MoveCardsToWastePile(SVector<SSharedPtr<SACard>>& StockPileCards)
{
	// If the waste pile has cards, move them back to the stock pile
	if (!Cards.empty())
	{
		StockPileCards.insert(StockPileCards.begin(), std::make_move_iterator(Cards.begin()), std::make_move_iterator(Cards.end()));
	}

	// TODO: Get the number of cards to draw from the stock pile based on game rules (usually 1 or 3)
	SUInt32 Amount = 3;

	// Move the last 'Amount' cards from the stock pile to the waste pile (to the front)
	auto MoveStartIterator = StockPileCards.end() - Amount;
	Cards.insert(Cards.begin(), std::make_move_iterator(MoveStartIterator), std::make_move_iterator(StockPileCards.end()));

	// Remove the moved cards from the stock pile
	StockPileCards.erase(MoveStartIterator, StockPileCards.end());

	// Calculate the total horizontal shift needed for positioning all cards
	SUInt32 TotalShift = (Amount - 1) * 4;

	for (SUInt32 Index = 0; Index < Amount; Index++)
	{
		// Access the card from the end of the pile (starting from the last card)
		SSharedPtr<SACard> Card = Cards[Cards.size() - Index - 1];

		// Get the current grid position of the pile
		const SGridPositionU32& GridPosition = GetGridPosition();

		// Calculate the X position reversed: start from the rightmost position and move leftwards
		SUInt32 PositionX = GridPosition.first + TotalShift - (Index * 4);

		// Set the calculated grid position to the card
		Card->SetGridPosition(SGridPositionU32(PositionX, GridPosition.second));

		// Flip the card face-up
		FCardInfo& CardInfo = Card->GetCardInfo_Mutable();
		CardInfo.IsFaceUp = true;
	}
}	