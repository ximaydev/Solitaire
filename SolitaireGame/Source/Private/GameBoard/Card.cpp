#include "SolitaireGamePCH.h"
#include "Rendering/ConsoleRenderer.h"

void SCardConverter::CardRankToString(ECardRank CardRank, SWString& OutString)
{
    // Static lookup table of card rank strings
    constexpr SWStringView CardRankStrings[] =
    {
    TEXT(" "), TEXT("A"), TEXT("2"), TEXT("3"), TEXT("4"),
    TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8"), TEXT("9"),
    TEXT("10"),TEXT("J"), TEXT("Q"), TEXT("K")
    };

    // Fetch the string representation of the given card rank.
    OutString = CardRankStrings[static_cast<SUInt8>(CardRank)];
}

void SCardConverter::CardSuitToString(ECardSuit CardSuit, SWString& OutString)
{
    // Static lookup table of card suit strings
    constexpr SWStringView CardSuitStrings[] =
    {
        TEXT(" "), TEXT("♣"), TEXT("♦"), TEXT("♥"), TEXT("♠")
    };

    // Fetch the string representation of the given card suit.
    OutString = CardSuitStrings[static_cast<SUInt8>(CardSuit)];
}

ECardRank SCardConverter::StringToCardRank(const SWString& String)
{
    // Check if the input string represents a face card or Ace, and return the corresponding enum.
    if (String == TEXT("A")) return ECardRank::Ace;
    else if (String == TEXT("J")) return ECardRank::Jack;
    else if (String == TEXT("Q")) return ECardRank::Queen;
    else if (String == TEXT("K")) return ECardRank::King;
    else
    {
        // Check if the input string consists only of digit characters (e.g., "2", "10")
        if (std::all_of(String.begin(), String.end(), ::isdigit))
        {
            // Convert the numeric string to an unsigned integer
            SUInt32 Value = static_cast<SUInt32>(std::stoi(String));

            // Check if the numeric value is within the valid range of non-face card ranks (2–10)
            if (Value >= 2 && Value <= 10)
            {
                // Cast the integer to the corresponding ECardRank enum and return it
                return static_cast<ECardRank>(Value);
            }
        }
    }

    // If none of the cases matched, return a special value indicating an invalid or unrecognized rank.
    return ECardRank::None;
}

ECardSuit SCardConverter::StringToCardSuit(const SWString& String)
{
    // Check if the input string represents a valid card suit, and return the corresponding ECardSuit enum value.
    if (String == TEXT("C")) return ECardSuit::Clubs;
    else if (String == TEXT("D")) return ECardSuit::Diamonds;
    else if (String == TEXT("H")) return ECardSuit::Hearts;
    else if (String == TEXT("S")) return ECardSuit::Spades;
    else    return ECardSuit::None;
}

void FCardInfo::SetCardColor()
{
    // Check if the CardSuit is a heart or a diamond
    if (CardSuit == ECardSuit::Hearts || CardSuit == ECardSuit::Diamonds)
    {
        // Red color for Hearts and Diamonds
        Color = FG_RED;
    }
    else
    {
        // Black color for Clubs and Spades
        Color = FG_BLACK;
    }
}

SACard::SACard(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, const FCardInfo& NewCardInfo) 
    : SAActor(NewGridPosition, NewWorld), CardInfo(NewCardInfo)
{
    // Create card
    UpdateCardVisual();
}

void SACard::Write()
{
    // Get the singleton instance of the console renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Render card
    for (SUInt32 Index = 0; Index < CardVisualLines.size(); ++Index)
    {
        // Create a span representing the 7 color values for the current line
        SSpan<WORD, 7> TempColors(CardColorBuffer.data() + Index * 7, 7);

        // Write the current line of text at the appropriate vertical offset, using individual character colors from TempColors.
        const SWString& Line = CardVisualLines[Index];
        ConsoleRenderer->Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index), Line, static_cast<SUInt32>(Line.size()), TempColors, true);
    }
}

void SACard::ClearBuffer()
{
    // Get instance of the console renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Clear each row of the card's area in the buffer
    for (SUInt32 Index = 0; Index < CardInfo.CardSizeY; Index++)
    {
        // Clear one horizontal row at a time at the card's position
        ConsoleRenderer->ClearBufferAt(SGridPositionU32(GridPosition.first, GridPosition.second + Index), CardInfo.CardSizeX);
    }
}

SBool SACard::CanBePlacedOnTableau(const FCardInfo& Other) const
{
    // Check if this card's rank is one lower than the target card (descending order rule)
    SBool bIsRankCorrect = static_cast<SUInt8>(CardInfo.GetCardRank()) + 1 == static_cast<SUInt8>(Other.GetCardRank());

    // Check if the suits are of opposite colors
    SBool bIsColorOpposite = !HasSameColor(Other);

    // The card can be placed if both rules are satisfied
    return bIsRankCorrect && bIsColorOpposite;
}

void SACard::SetNextCard(const SGridPositionU32& NextCardGridPosition, SSharedPtr<SACard> NewNextCard)
{
    // Check if the NewNextCard isn't nullptr
    if (NewNextCard)
    {
        // Store a shared pointer to the next card in the sequence
        NextCard = NewNextCard;

        // Update the grid position of the new next card
        NewNextCard->SetGridPosition(NextCardGridPosition);
    }
}

SBool SACard::CanBePlacedOnFoundation(const FCardInfo& Other) const
{
    // If there is no next card (i.e., this is the bottom card in the stack)
    if (GetNextCard() == nullptr)
    {
        // Check if this card's suit matches the other card's suit and if this card's rank is exactly one higher than the other card's rank
        return CardInfo.GetCardSuit() == Other.GetCardSuit() && static_cast<SUInt8>(CardInfo.GetCardRank()) == static_cast<SUInt8>(Other.GetCardRank()) + 1;
    }

    return false;
}

void SACard::SetIsFaceUp(SBool NewIsFaceUp)
{
    // Set Is Face Up
    CardInfo.IsFaceUp = NewIsFaceUp;

    // Refresh card 
    UpdateCardVisual();
}

void SACard::UpdateCardVisual()
{
    // Get string representation of card rank
    SWString RankString;
    SCardConverter::CardRankToString(CardInfo.GetCardRank(), RankString);

    // Get string representation of card suit symbol
    SWString SuitString;
    SCardConverter::CardSuitToString(CardInfo.GetCardSuit(), SuitString);

    // Check if the card is face up
    if (CardInfo.IsFaceUp)
    {
        // Create card
        DRAW_CARD_FACE_UP(CardVisualLines, RankString, SuitString);
        COLOR_CARD_FACE_UP(CardColorBuffer, CardInfo.GetColor(), CardInfo.CardSizeX, RankString.size());
    }
    else
    {
        // Create card
        DRAW_CARD_FACE_DOWN(CardVisualLines, CardColorBuffer);
    }
}
