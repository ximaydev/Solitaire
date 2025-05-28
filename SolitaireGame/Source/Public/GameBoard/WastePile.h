#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** Represents the Waste Pile in Solitaire, which holds cards drawn from the Stock Pile. */
class SAWastePile final : public SAActor
{
public:
	/** Constructor */
	SAWastePile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld);
	SAWastePile(const SAWastePile& Other);

	/** Operators */
	SAWastePile operator=(const SAWastePile& Other)
	{
		if (this != &Other)
		{
			// Call CopyFrom and perform a deep copy
			CopyFrom(Other);
		}
		return *this;
	}

	/** Get Cards */
	inline const SVector<SSharedPtr<SACard>>& GetCards() const { return Cards; }

	/** Returns true if there are cards available in the Waste Pile to interact with */
	inline bool HasUsableCard() const { return RevealedCardCount > 0; }

	/** Renders the Waste sPile. */
	void Write() override;

	/** Performs a deep copy of all owned data from 'other' into this object. */
	void CopyFrom(const SAActor& Other) override;

	/** Performs a deep copy of the current object using the copy constructor. */
	virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAWastePile(*this)); }

	/** Use the top card in the pile (the first card in the vector). */
	SSharedPtr<SACard> UseTopCard();

	/** Transfer new cards to the waste pile. If the vector 'StockPileCards' isn't empty, move the cards back to the original stock pile. */
	void MoveCardsToWastePile(SVector<SSharedPtr<SACard>>& StockPileCards);

protected:

	/** Number of cards currently revealed (face-up) in the Waste Pile */
	SUInt8 RevealedCardCount = 0;

	/** Container holding all cards in this waste spile */
	SVector<SSharedPtr<SACard>> Cards;
};