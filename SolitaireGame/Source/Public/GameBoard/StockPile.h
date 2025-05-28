#pragma once
#include "Framework/Actor.h"
#include "Globals.h"

/** Forward Declarations */
class SAWastePile;
class SACard;

/** Represents the Stock Pile in the Solitaire game, managing the deck of cards from which the player draws during gameplay. */
class SAStockPile final : public SAActor
{
public:
	/** Constructors */
    SAStockPile(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& InitialCards);
    SAStockPile(const SAStockPile& Other);

    /** Operators */
    SAStockPile operator=(const SAStockPile& Other)
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

    /** Get Cards Mutable */
    inline SVector<SSharedPtr<SACard>>& GetCards_Mutable() { return Cards; }

    /** Returns a shared pointer to the top card of the pile */
    SSharedPtr<SACard> GetTopCard() const;

    /** Removes the specified card from the Cards */
    void RemoveCardFromPile(SSharedPtr<SACard> CardToRemove);

    /** Renders the Stock Pile. */
    void Write() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    void CopyFrom(const SAActor& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAStockPile(*this)); }

protected:
    /** Fills the pile with the initial set of cards (called once at setup) */
    void FillInitialCards(SVector<SSharedPtr<SACard>>&& InitialCards);

    /** Container holding all cards in this stock spile */
    SVector<SSharedPtr<SACard>> Cards;
};