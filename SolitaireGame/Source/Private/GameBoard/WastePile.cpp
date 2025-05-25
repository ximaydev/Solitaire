#include "SolitaireGamePCH.h"

SAWastePile::SAWastePile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SAActor(NewGridPosition, NewWorld) {}

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

SSharedPtr<SACard> SAWastePile::UseTopCard()
{
	// Check if Cards is empty
	if (Cards.empty())
		return nullptr;

	// Get the card at the front (top) of the pile
	SSharedPtr<SACard> TopCard = Cards.back();

	// Clear buffer 
	TopCard->ClearBuffer();

	// Remove the last card from the vector (the one we just took)
	Cards.pop_back();

	//TODO Przetasowac je¿eli Cards jest juz pusty
	
	// Return the card that was removed
	return TopCard;
}

void SAWastePile::MoveCardsToWastePile(SVector<SSharedPtr<SACard>>& StockPileCards)
{
	// Move all cards from the waste pile back to the stock pile
	for (auto Iterator = Cards.rbegin(); Iterator != Cards.rend(); Iterator++)
	{
		// Get Card
		SSharedPtr<SACard>& Card = *Iterator;

		// Set Grid Position. Last element is always in the stock pile position
		Card->SetGridPosition(StockPileCards.back()->GetGridPosition());

		// Flip card face down before moving
		Card->SetIsFaceUp(false);

		// Clear buffer
		Card->ClearBuffer();

		// Emplace
		StockPileCards.emplace(StockPileCards.begin(), std::move(Card));
	}

	// Clear the waste pile after transferring the cards
	Cards.clear();

	// Get CardsToRevealPerStockUse
	SUInt32 Amount = GetWorld<SGameBoardWorld>()->GetGameRules()->CardsToRevealPerStockUse;

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
		Card->SetIsFaceUp(true);
	}
}	