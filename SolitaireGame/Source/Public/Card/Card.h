#pragma once
#include "Globals.h"
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
    None,
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

/** Converts a card rank enum value to its corresponding string representation. */
void CardRankToString(ECardRank CardRank, SWString& OutString);

/** Converts a card suit enum to its symbolic string representation (e.g., "♠", "♥"). */
void CardSuitToString(ECardSuit CardSuit, SWString& OutString);

/**
 * Structure representing a single playing card.
 * Combines rank and suit, plus a flag indicating if the card is revealed.
 */
struct FCardInfo
{
public:
    /** Constructors */
    FCardInfo() = delete;
    FCardInfo(ECardRank InRank, ECardSuit InSuit, SBool bIsFaceUp) : CardRank(InRank), CardSuit(InSuit), IsFaceUp(bIsFaceUp) { SetCardColor(); }

    /** Sets the color of the card based on its suit. */
    void SetCardColor();

    /** Rank (value) of the card. */
    ECardRank CardRank;

    /** Suit (color) of the card. */
    ECardSuit CardSuit;

    /** True if the card is face up (visible). */
    SBool IsFaceUp;

    /** Console display color attribute for the card. */
    WORD Color;

    /** Card Sizes */
    const SUInt32 CardSizeX = 7;
    const SUInt32 CardSizeY = 5;
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

private:
    /** Stores card information (rank, suit, face up). */
    FCardInfo CardInfo;
};