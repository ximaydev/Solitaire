#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

class SAWastePile final : public SAActor
{
	friend class SAStockPile;
public:
	/** Constructor */
	SAWastePile(const SGridPositionU32& NewGridPosition);

	/** Returns true if there are cards available in the Waste Pile to interact with */
	inline bool HasUsableCard() const { return RevealedCardCount > 0; }

	/** Use the top card in the pile (the first card in the vector). */
	SSharedPtr<SACard> UseTopCard();

	/** Renders the Waste sPile. */
	void Write() override;

protected:
	/** Transfer new cards to the waste pile. If the vector 'StockPileCards' isn't empty, move the cards back to the original stock pile. */
	void MoveCardsToWastePile(SVector<SSharedPtr<SACard>>& StockPileCards);

	/** Number of cards currently revealed (face-up) in the Waste Pile */
	SUInt8 RevealedCardCount = 0;

	/** Container holding all cards in this waste spile */
	SVector<SSharedPtr<SACard>> Cards;
};