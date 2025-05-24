#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SAWastePile;
class SACard;

/** Represents the Stock Pile in the Solitaire game, managing the deck of cards from which the player draws during gameplay. */
class SAStockPile final : public SAActor
{
public:
	/** Constructors */
    SAStockPile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& InitialCards);

	/** Get Waste Pile */
	inline SAWastePile* GetWastePile() const { return WastePile.get(); }

    /** Get Cards */
    inline const SVector<SSharedPtr<SACard>>& GetCards() const { return Cards; }

    /** Returns a shared pointer to the top card of the pile */
    SSharedPtr<SACard> GetTopCard() const;

    /** Removes the specified card from the Cards */
    void RemoveCardFromPile(SSharedPtr<SACard> CardToRemove);

    /** Renders the Stock Pile. */
    void Write() override;

    /** Uses the stock pile. Moves the specified number of cards from the stock pile to the waste pile. */
    void UseStockPile();

protected:
    /** Initializes the Waste Pile at a position offset from the Stock Pile. */
    void InitializeWastePile();

    /** Fills the pile with the initial set of cards (called once at setup) */
    void FillInitialCards(SVector<SSharedPtr<SACard>>&& InitialCards);

    /** Unique pointer to the Waste Pile where drawn cards are placed */
    SUniquePtr<SAWastePile> WastePile;

    /** Container holding all cards in this stock spile */
    SVector<SSharedPtr<SACard>> Cards;
};