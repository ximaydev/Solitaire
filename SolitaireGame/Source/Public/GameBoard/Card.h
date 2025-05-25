#pragma once
#include "Framework/Actor.h"

/** Macro to draw the face-up card with given rank and suit strings. */
#define DRAW_CARD_FACE_UP(LinesArray, RankString, SuitString)           \
    do {                                                   \
        LinesArray[0] = TEXT("███████");                  \
        LinesArray[1] = TEXT("█") + RankString + SWString(5 - RankString.size(), ' ') + TEXT("█"); \
        LinesArray[2] = TEXT("█  ") + SuitString + TEXT("  █"); \
        LinesArray[3] = TEXT("█") + SWString(5 - RankString.size(), ' ') + RankString + TEXT("█"); \
        LinesArray[4] = TEXT("███████");                  \
    } while (0)

/** Macro to color the face-up card's rank and suit symbols according to card color. */
#define COLOR_CARD_FACE_UP(ColorsArray, CardColor, CardSizeX, RankStringSize) \
    do {                                                           \
        ColorsArray.fill(FG_WHITE | BG_WHITE);                          \
        ColorsArray[1 * CardSizeX + 1] = CardColor | BG_WHITE;          \
        ColorsArray[2 * CardSizeX + 3] = CardColor | BG_WHITE;          \
        ColorsArray[3 * CardSizeX + 5] = CardColor | BG_WHITE;          \
        if (RankString.size() >= 2) {                              \
            ColorsArray[1 * CardSizeX + 2] = CardColor | BG_WHITE;      \
            ColorsArray[3 * CardSizeX + 4] = CardColor | BG_WHITE;      \
        }                                                          \
    } while (0)

/** Macro to draw the face - down card with decorative pattern and default colors. */
#define DRAW_CARD_FACE_DOWN(LinesArray, ColorsArray)          \
    do {                              \
        ColorsArray.fill(FG_LIGHT_BLUE | BG_WHITE); \
        LinesArray[0] = TEXT("▒▓▒▓▒▓▒");     \
        LinesArray[1] = TEXT("▓▒▓▒▓▒▓");     \
        LinesArray[2] = TEXT("▒▓▓▓▓▓▒");     \
        LinesArray[3] = TEXT("▓▒▓▒▓▒▓");     \
        LinesArray[4] = TEXT("▒▓▒▓▒▓▒");     \
    } while (0)

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

/** Static utility class for converting between card rank and suit enumerations and their string representations. */
class SCardConverter
{
public:
    /** Converts a card rank enum value to its corresponding string representation. */
    static void CardRankToString(ECardRank CardRank, SWString& OutString);

    /** Converts a card suit enum to its symbolic string representation (e.g., "♠", "♥"). */
    static void CardSuitToString(ECardSuit CardSuit, SWString& OutString);

    /** Converts a string representation of a card rank to the corresponding enum value. Accepts values like "A", "2", ..., "10", "J", "Q", "K". */
    static ECardRank StringToCardRank(const SWString& String);

    /**  Converts a string representation of a card suit to the corresponding enum. Accepts characters like "H", "D", "C", "S" (case-insensitive). */
    static ECardSuit StringToCardSuit(const SWString& String);
};

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
	SACard(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, const FCardInfo& NewCardInfo);

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
    inline void SetNextCard(SSharedPtr<SACard> NewNextCard) { NextCard = NewNextCard; }

    /** Get the next card */
    inline SSharedPtr<SACard> GetNextCard() const { return NextCard; }

    /** Checks if both cards has the same color of the card. */
    inline SBool HasSameColor(const FCardInfo& Other) const { return CardInfo.GetColor() == Other.GetColor(); }

    /** Determines if this card can be placed on the given foundation stack card. */
    SBool CanBePlacedOnFoundation(const FCardInfo& Other) const;

    /** Returns true if the card can be placed on an empty tableau column (only Kings). */
    inline SBool CanBePlacedOnEmptyTableau() const { return CardInfo.GetCardRank() == ECardRank::King; }

    /** Returns true if this card can start a foundation pile (only Aces). */
    inline SBool IsFoundationBaseCard() const { return CardInfo.GetCardRank() == ECardRank::Ace; }

    /** Sets whether the card is face up or face down. Automatically updates the card visuals. */
    void SetIsFaceUp(SBool bNewIsFaceUp);

    /** Update the card visual */
    void UpdateCardVisual();

protected:
    /** Stores card information (rank, suit, face up). */
    FCardInfo CardInfo;

    /** Link to the next card in the tableau sequence. nullptr if this is the bottom card. */
    SSharedPtr<SACard> NextCard = nullptr;

    /** Visual representation of the card using 5 lines of text */
    SArray<SWString, 5> CardVisualLines;

    /** Color buffer: stores text attributes for each character (7 columns × 5 rows) */
    SArray<WORD, 7 * 5> CardColorBuffer;
};