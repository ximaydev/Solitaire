#pragma once
#include "Globals.h"
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** SASolitaireRules defines the rule set and move validation logic for a Solitaire game instance. */
class SASolitaireRules : public SAActor
{
public:
    /** Constructor */
    SASolitaireRules(SSharedPtr<SWorld> NewWorld);
    SASolitaireRules(const SASolitaireRules& Other);

    /** Operators */
    SASolitaireRules operator=(const SASolitaireRules& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }
        return *this;
    }

    /** Performs a deep copy of all owned data from 'other' into this object. */
    virtual void CopyFrom(const SAActor& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SASolitaireRules(*this)); }

    /** Get CardsToRevealPerStockUse */
    inline SUInt8 GetCardsToRevealPerStockUse() const { return CardsToRevealPerStockUse; }

    /** Determines whether a card from the Waste Pile can legally be moved onto the given Foundation pile. */
    bool CanMoveWastePileToFoundationList(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> FoundationListCard);

    /** Determines whether a card from the Waste Pile can legally be placed onto a card on the Tableau. */
    bool CanMoveWastePileToBoard(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> BoardCard);

    /** Determines whether a card can legally be moved from one Tableau column to another. */
    bool CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2);
    bool CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2, SBool IsKingCard2);

    /** Determines whether a card from the Tableau can legally be moved onto the Foundation pile. */
    bool CanMoveBoardToFoundationList(const SSharedPtr<SACard> BoardCard, const SSharedPtr<SACard> FoundationListCard);

    /** Determines if card can be placed on other card in the tableau. */
    SBool CanBePlacedOnTableau(const SSharedPtr<SACard> Card, const SSharedPtr<SACard> OtherCard) const;

    /** Determines if this card can be placed on the given foundation stack card. */
    SBool CanBePlacedOnFoundation(const SSharedPtr<SACard> Card, const SSharedPtr<SACard> OtherCard) const;

    /** Returns true if the card can be placed on an empty tableau column (only Kings). */
    inline SBool CanBePlacedOnEmptyTableau(const SSharedPtr<SACard> Card) const { return Card->GetCardInfo().GetCardRank() == ECardRank::King; }

    /** Returns true if this card can start a foundation pile (only Aces). */
    inline SBool IsFoundationBaseCard(const SSharedPtr<SACard> Card) const { return Card->GetCardInfo().GetCardRank() == ECardRank::Ace; }

protected:
    /** Number of cards to reveal each time Stock Pile is used. */
    SUInt8 CardsToRevealPerStockUse = 0;

    /** Determines whether a given card can be placed on a tableau pile according to standard Solitaire rules. */
    bool CanPlaceCardOnTableau(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> TargetCard);

    /** Checks if a card can be placed on the foundation pile according to solitaire rules. */
    bool CanPlaceCardOnFoundation(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> FoundationCard);
};