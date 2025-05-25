#include "SolitaireGamePCH.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"

SASolitaireRules::SASolitaireRules(SSharedPtr<SWorld> NewWorld)
	: SAActor(SGridPositionU32(0, 0), NewWorld),
	  CardsToRevealPerStockUse(SIniFileManager::GetInstance()->GetConfigFile(DefaultGameConfig)->GetValueFromKey<SUInt8>(TEXT("[GameRules]"), TEXT("CardsToRevealPerStockUse")))
{}

bool SASolitaireRules::CanMoveWastePileToFoundationList(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> FoundationListCard)
{
    return CanPlaceCardOnFoundation(WastePileCard, FoundationListCard);
}

bool SASolitaireRules::CanMoveWastePileToBoard(const SSharedPtr<SACard> WastePileCard, const SSharedPtr<SACard> BoardCard)
{
    return CanPlaceCardOnTableau(WastePileCard, BoardCard);
}

bool SASolitaireRules::CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2)
{
    // Both cards must be face up to allow move
    if (!BoardCard1 || !BoardCard1->GetCardInfo().IsFaceUp)
        return false;

    if (!BoardCard2 || !BoardCard2->GetCardInfo().IsFaceUp)
        return false;

    // Check if placing BoardCard1 on BoardCard2 is legal by game rules
    return CanPlaceCardOnTableau(BoardCard1, BoardCard2);
}

bool SASolitaireRules::CanMoveBoardToFoundationList(const SSharedPtr<SACard> BoardCard, const SSharedPtr<SACard> FoundationListCard)
{
    // BoardCard must be face up to allow move
    if (!BoardCard->GetCardInfo().IsFaceUp)
        return false;

    return CanPlaceCardOnFoundation(BoardCard, FoundationListCard);
}

bool SASolitaireRules::CanPlaceCardOnTableau(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> TargetCard)
{
    // Check if the destination tableau pile is currently empty.
    if (!TargetCard)
    {
        // Log
        S_LOG(LogTemp, TEXT("Target tableau pile is empty. Checking if card is a King..."));

        // Check if the card is a King, which is the only card allowed to be placed on an empty tableau.
        if (CardToPlace->CanBePlacedOnEmptyTableau())
        {
            // Log success.
            S_LOG(LogTemp, TEXT("Card is a King. Move to empty tableau is allowed."));
            return true; // Move is valid.
        }

        // Log failure when the card isn't a King and can't be placed.
        S_LOG(LogTemp, TEXT("Card isn't a King. Can't be placed on empty tableau."));
        return false;
    }

    // Get the FCardInfo from the existing tableau card.
    const FCardInfo& TargetCardInfo = TargetCard->GetCardInfo();

    // Check if the card can be placed.
    const SBool bCanPlace = CardToPlace->CanBePlacedOnTableau(TargetCardInfo);

    // Log the result.
    if (bCanPlace)
    {
        // Log success: the card follows tableau rules (rank -1 and opposite color).
        S_LOG(LogTemp, TEXT("Card can be placed on tableau card (rank -1, opposite color)."));
    }
    else
    {
        // Log failure: the move violates tableau placement rules.
        S_LOG(LogTemp, TEXT("Card can't be placed on tableau card (invalid rank or same color)."));
    }

    return bCanPlace;
}

bool SASolitaireRules::CanPlaceCardOnFoundation(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> FoundationCard)
{
    // Check if the foundation pile is currently empty.
    if (!FoundationCard)
    {
        // Log that the foundation pile is empty.
        S_LOG(LogTemp, TEXT("Foundation pile is empty. Checking if the card is an Ace..."));

        // If the card is Ace, it can be placed as the base of the foundation pile.
        if (CardToPlace->IsFoundationBaseCard() && CardToPlace->GetNextCard() == nullptr)
        {
            // Log successful.
            S_LOG(LogTemp, TEXT("Card is Ace. Move to empty foundation is allowed."));
            return true;
        }

        // Log failed.
        S_LOG(LogTemp, TEXT("Card isn't Ace. Can't be placed on empty foundation pile."));
        return false;
    }

    // Use CanBePlacedOnFoundation to check suit match and +1 rank rule.
    const SBool bCanPlace = CardToPlace->CanBePlacedOnFoundation(FoundationCard->GetCardInfo());

    // Log result of placement rule check.
    if (bCanPlace)
    {
        // Log valid move.
        S_LOG(LogTemp, TEXT("Card can be placed on foundation (correct rank and suit)."));
    }
    else
    {
        // Log invalid move.
        S_LOG(LogTemp, TEXT("Card can't be placed on foundation (invalid rank or suit)."));
    }

    return bCanPlace;
}