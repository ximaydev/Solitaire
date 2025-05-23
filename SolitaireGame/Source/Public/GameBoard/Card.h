#pragma once
#include "Framework/Actor.h"

/**
 * Enum representing the rank (value) of a playing card.
 * Includes numbers 2-10 and face cards (Jack, Queen, King) plus Ace.
 */
enum class ECardRank : SUInt8
{
    None = 0,
	Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
};

/**
 * Enum representing the suit (color) of a playing card.
 * Standard suits: Clubs, Diamonds, Hearts, Spades.
 */
enum class ECardSuit : SUInt8
{
    None = 0,
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

/** Converts a card rank enum value to its corresponding string representation. */
void CardRankToString(ECardRank CardRank, SWString& OutString);

/** Converts a card suit enum to its symbolic string representation (e.g., "♠", "♥"). */
void CardSuitToString(ECardSuit CardSuit, SWString& OutString);

/** Converts a string representation of a card rank to the corresponding enum value. Accepts values like "A", "2", ..., "10", "J", "Q", "K". */
ECardRank StringToCardRank(const SWString& String);

/**  Converts a string representation of a card suit to the corresponding enum. Accepts characters like "H", "D", "C", "S" (case-insensitive). */
ECardSuit StringToCardSuit(const SWString& String);

/**
 * Structure representing a single playing card.
 * Combines rank and suit, plus a flag indicating if the card is revealed.
 */
struct FCardInfo
{
public:
    /** Constructors */
    FCardInfo(ECardRank InRank, ECardSuit InSuit, SBool bIsFaceUp) : CardRank(InRank), CardSuit(InSuit), IsFaceUp(bIsFaceUp) { SetCardColor(); }

    /** Get CardRank */
    inline ECardRank GetCardRank() const { return CardRank; }
    
    /** Get CardSuit */
    inline ECardSuit GetCardSuit() const { return CardSuit; }

    /** Get Color */
    inline WORD GetColor() const { return Color; }

    /** Card Sizes */
    const SUInt32 CardSizeX = 7;
    const SUInt32 CardSizeY = 5;

    /** True if the card is face up (visible). */
    SBool IsFaceUp;

protected:
    /** Sets the color of the card based on its suit. */
    void SetCardColor();

    /** Rank (value) of the card. */
    ECardRank CardRank;

    /** Suit (color) of the card. */
    ECardSuit CardSuit;

    /** Console display color attribute for the card. */
    WORD Color;
};

/** Class representing a card with rendering capabilities. */
class SACard final : public SAActor
{
public:
	/** Constructors */
	SACard(const SGridPositionU32& NewGridPosition, const FCardInfo& NewCardInfo);

    /** Renders the card at a specified grid position. */
    void Write() override;

    /** Clears the card from the console using its internal grid position. */
    void ClearBuffer() override;

    /** Get Card Info */
    FCardInfo& GetCardInfo_Mutable() { return CardInfo; }

    /** Get Card Info */
    const FCardInfo& GetCardInfo() const { return CardInfo; }

    /** Determines if this card can be placed on another card in the tableau. */
    SBool CanBePlacedOnTableau(const FCardInfo& Other) const;

    /** Sets the next card in a sequence and updates its grid position. */
    void SetNextCard(const SGridPositionU32& NextCardGridPosition, SSharedPtr<SACard> NewNextCard);

    /** Sets the next card in a sequence */
    void SetNextCard(SSharedPtr<SACard> NewNextCard) { NextCard = NewNextCard; }

    /** Checks if both cards belong to the same suit. */
    inline SBool IsSameSuit(const FCardInfo& Other) const { return CardInfo.GetCardSuit() == Other.GetCardSuit(); }

    /** Determines if this card can be placed on the given foundation stack card. */
    inline SBool CanBePlacedOnFoundation(const FCardInfo& Other) const { return CardInfo.GetCardSuit() == Other.GetCardSuit() && static_cast<SUInt8>(CardInfo.GetCardRank()) == static_cast<SUInt8>(Other.GetCardRank()) + 1; }

    /** Returns true if the card can be placed on an empty tableau column (only Kings). */
    inline SBool CanBePlacedOnEmptyTableau() const { return CardInfo.GetCardRank() == ECardRank::King; }

    /** Returns true if this card can start a foundation pile (only Aces). */
    inline SBool IsFoundationBaseCard() const { return CardInfo.GetCardRank() == ECardRank::Ace; }

private:
    /** Stores card information (rank, suit, face up). */
    FCardInfo CardInfo;

    /** Link to the next card in the tableau sequence. nullptr if this is the bottom card. */
    SSharedPtr<SACard> NextCard = nullptr;
};