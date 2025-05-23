#pragma once
#include "Globals.h"
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

class SASolitaireRules : public SAActor
{
public:
    /** Constructor */
    SASolitaireRules();

    /** Determines whether a card from the Waste Pile can legally be moved onto the given Foundation pile. */
    bool CanMoveWastePileToFoundationList(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> FoundationListCard);

    /** Move card from Waste Pile to Board. */
    bool CanMoveWastePileToBoard(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> BoardCard);

    /** Move card within Board. */
    bool CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2);

    /** Move card from Board to Foundation Lists. */
    bool CanMoveBoardToFoundationList(const SSharedPtr<SACard> BoardCard, const SSharedPtr<SACard> FoundationListCard);

    /** Number of cards to reveal each time Stock Pile is used. */
    const SUInt8 CardsToRevealPerStockUse = {};

protected:
    /** Determines whether a given card can be placed on a tableau pile according to standard Solitaire rules. */
    bool CanPlaceCardOnTableau(const SSharedPtr<SACard>& CardToPlace, const SSharedPtr<SACard>& TargetCard);

    /** Checks if a card can be placed on the foundation pile according to solitaire rules. */
    bool CanPlaceCardOnFoundation(const SSharedPtr<SACard>& CardToPlace, const SSharedPtr<SACard>& FoundationCard);
};