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

    /** Determines whether a card from the Waste Pile can legally be moved onto the given Foundation pile. */
    bool CanMoveWastePileToFoundationList(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> FoundationListCard);

    /** Determines whether a card from the Waste Pile can legally be placed onto a card on the Tableau. */
    bool CanMoveWastePileToBoard(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> BoardCard);

    /** Determines whether a card can legally be moved from one Tableau column to another. */
    bool CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2);

    /** Determines whether a card from the Tableau can legally be moved onto the Foundation pile. */
    bool CanMoveBoardToFoundationList(const SSharedPtr<SACard> BoardCard, const SSharedPtr<SACard> FoundationListCard);

    /** Number of cards to reveal each time Stock Pile is used. */
    const SUInt8 CardsToRevealPerStockUse = 0;

protected:
    /** Determines whether a given card can be placed on a tableau pile according to standard Solitaire rules. */
    bool CanPlaceCardOnTableau(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> TargetCard);

    /** Checks if a card can be placed on the foundation pile according to solitaire rules. */
    bool CanPlaceCardOnFoundation(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> FoundationCard);
};