#include "SolitaireGamePCH.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"

SASolitaireRules::SASolitaireRules(SSharedPtr<SWorld> NewWorld)
	: SAActor(SGridPositionU32(0, 0), NewWorld),
	  CardsToRevealPerStockUse(SIniFileManager::GetInstance()->GetConfigFile(DefaultGameConfig)->GetValueFromKey<SUInt8>(TEXT("[GameRules]"), TEXT("CardsToRevealPerStockUse")))
{}

SASolitaireRules::SASolitaireRules(const SASolitaireRules & Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

void SASolitaireRules::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SASolitaireRules pointer
    if (const SASolitaireRules* OtherSolitaireRules = dynamic_cast<const SASolitaireRules*>(&Other))
    {
        SAActor::CopyFrom(Other);
        CardsToRevealPerStockUse = OtherSolitaireRules->CardsToRevealPerStockUse;
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SASolitaireRules."))
    }
}

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

bool SASolitaireRules::CanMoveBoardToBoard(const SSharedPtr<SACard> BoardCard1, const SSharedPtr<SACard> BoardCard2, SBool IsKingCard2)
{
    // Both cards must be face up to allow move
    if (!BoardCard1 || !BoardCard1->GetCardInfo().IsFaceUp)
        return false;

    // We don't have to check if the Card2 is face up becuse we will move king into the empty column
    if (!IsKingCard2)
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

SBool SASolitaireRules::CanBePlacedOnTableau(const SSharedPtr<SACard> Card, const SSharedPtr<SACard> OtherCard) const
{
    // Check if this card's rank is one lower than the target card (descending order rule)
    SBool bIsRankCorrect = static_cast<SUInt8>(Card->GetCardInfo().GetCardRank()) + 1 == static_cast<SUInt8>(OtherCard->GetCardInfo().GetCardRank());

    // Check if the suits are of opposite colors
    SBool bIsColorOpposite = !Card->HasSameColor(OtherCard->GetCardInfo());

    // The card can be placed if both rules are satisfied
    return bIsRankCorrect && bIsColorOpposite;
}

SBool SASolitaireRules::CanBePlacedOnFoundation(const SSharedPtr<SACard> Card, const SSharedPtr<SACard> OtherCard) const
{
    // If there is no next card (i.e., this is the bottom card in the stack)
    if (Card->GetNextCard() == nullptr)
    {
        // Check if this card's suit matches the other card's suit and if this card's rank is exactly one higher than the other card's rank
        return Card->GetCardInfo().GetCardSuit() == OtherCard->GetCardInfo().GetCardSuit() && 
        static_cast<SUInt8>(Card->GetCardInfo().GetCardRank()) == static_cast<SUInt8>(OtherCard->GetCardInfo().GetCardRank()) + 1;
    }

    return false;
}

bool SASolitaireRules::CanPlaceCardOnTableau(const SSharedPtr<SACard> CardToPlace, const SSharedPtr<SACard> TargetCard)
{
    // Check if the destination tableau pile is currently empty.
    if (!TargetCard)
    {
        // Log
        S_LOG(LogTemp, TEXT("Target tableau pile is empty. Checking if card is a King..."));

        // Check if the card is a King, which is the only card allowed to be placed on an empty tableau.
        if (CanBePlacedOnEmptyTableau(CardToPlace))
        {
            // Log success.
            S_LOG(LogTemp, TEXT("Card is a King. Move to empty tableau is allowed."));
            return true; // Move is valid.
        }

        // Log failure when the card isn't a King and can't be placed.
        S_LOG(LogTemp, TEXT("Card isn't a King. Can't be placed on empty tableau."));
        return false;
    }

    // Check if the card can be placed.
    const SBool bCanPlace = CanBePlacedOnTableau(CardToPlace, TargetCard);

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
        if (IsFoundationBaseCard(CardToPlace) && CardToPlace->GetNextCard() == nullptr)
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
    const SBool bCanPlace = CanBePlacedOnFoundation(CardToPlace, FoundationCard);

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