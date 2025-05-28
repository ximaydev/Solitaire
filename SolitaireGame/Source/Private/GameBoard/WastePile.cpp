#include "SolitaireGamePCH.h"

SAWastePile::SAWastePile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SAActor(NewGridPosition, NewWorld) {}

SAWastePile::SAWastePile(const SAWastePile& Other)
{
	// Call CopyFrom and perform a deep copy
	CopyFrom(Other);
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

void SAWastePile::CopyFrom(const SAActor& Other)
{
	// Attempt to cast the base class reference to a SAWastePile pointer
	if (const SAWastePile* OtherWastePile = dynamic_cast<const SAWastePile*>(&Other))
	{
		// Call the parent CopyFrom
		SAActor::CopyFrom(Other);

		// Copy the count of revealed cards from the other WastePile instance
		RevealedCardCount = OtherWastePile->RevealedCardCount;

		// Clear current Cards container to prepare for copying new cards
		Cards.clear();

		// Reserve memory upfront to avoid reallocations during copying
		Cards.reserve(OtherWastePile->Cards.size());

		// Deep copy each card from the other WastePile's Cards vector
		for (const auto& Card : OtherWastePile->Cards)
		{
			// Create a new shared pointer with a copy of the card and move it into this container
			Cards.push_back(std::move(std::make_shared<SACard>(*Card)));
		}

	}
	else
	{
		S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SAWastePile."))
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
	SUInt32 Amount = GetWorld<SGameBoardWorld>()->GetGameRules()->GetCardsToRevealPerStockUse();

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