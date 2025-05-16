#include "SolitaireGamePCH.h"
#include "Card/Card.h"
#include "Rendering/ConsoleRenderer.h"

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

SACard::SACard(const SGridPositionU32& NewGridPosition, const FCardInfo& NewCardInfo) : SAActor(NewGridPosition), CardInfo(NewCardInfo) {}

void SACard::Write()
{
    // Get the singleton instance of the console renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Get string representation of card rank (e.g. "A" for Ace)
    SWString RankString;
    CardRankToString(CardInfo.CardRank, RankString);

    // Get string representation of card suit symbol (e.g. "♠" for Spades)
    SWString SuitString;
    CardSuitToString(CardInfo.CardSuit, SuitString);

    // Create array of colors for the card's ASCII representation
    // Card size is 7 x 5
    SArray<WORD, 7 * 5> Colors;
    SArray<SWString, 5> StringLines;

    // Card layout (5 lines):
    // Line 0: ███████
    // Line 1: █R    █
    // Line 2: █  S  █
    // Line 3: █    R█
    // Line 4: ███████
    //R = Rank
    //S = Suit

    // Check if the card is face up
    if (CardInfo.IsFaceUp)
    {
        // Fill the entire card area with the default color
        Colors.fill(FG_WHITE | BG_WHITE);

        // Top border
        StringLines[0] = TEXT("███████");

        // Top rank 
        StringLines[1] = TEXT("█") + RankString + SWString(5 - RankString.size(), ' ') + TEXT("█");

        // Middle suit line, centered
        StringLines[2] = TEXT("█  ") + SuitString + TEXT("  █");

        // Bottom rank
        StringLines[3] = TEXT("█") + SWString(5 - RankString.size(), ' ') + RankString + TEXT("█");

        // Bottom border
        StringLines[4] = TEXT("███████");

        // Highlight the rank and suit characters with the card's assigned color
        // Rank top-left (line 1, column 1)
        Colors[1 * 7 + 1] = CardInfo.Color | BG_WHITE;

        // Suit symbol (line 2, column 3)
        Colors[2 * 7 + 3] = CardInfo.Color | BG_WHITE;

        // Rank bottom-right (line 3, column 5)
        Colors[3 * 7 + 5] = CardInfo.Color | BG_WHITE;

        // If rank has two characters (e.g., "10"), color the second character too
        if (RankString.size() >= 2)
        {
            // Second character of top rank (line 1, column 2)
            Colors[1 * 7 + 2] = CardInfo.Color;

            // Second character of bottom rank (line 3, column 4)
            Colors[3 * 7 + 4] = CardInfo.Color;
        }
    }
    else
    {
        // Fill the entire card area with the default color 
        Colors.fill(FG_LIGHT_BLUE | BG_WHITE);

        // Face-down card decorative pattern
        StringLines[0] = TEXT("▒▓▒▓▒▓▒");
        StringLines[1] = TEXT("▓▒▓▒▓▒▓");
        StringLines[2] = TEXT("▒▓▓▓▓▓▒");
        StringLines[3] = TEXT("▓▒▓▒▓▒▓");
        StringLines[4] = TEXT("▒▓▒▓▒▓▒");
    }

    for (SUInt32 Index = 0; Index < StringLines.size(); ++Index)
    {
        // Create a span representing the 7 color values for the current line
        SSpan<WORD, 7> TempColors(Colors.data() + Index * 7, 7);

        // Write the current line of text at the appropriate vertical offset, using individual character colors from TempColors.
        ConsoleRenderer->Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index), StringLines[Index], TempColors);
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

void CardRankToString(ECardRank CardRank, SWString& OutString)
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

void CardSuitToString(ECardSuit CardSuit, SWString& OutString)
{
    // Static lookup table of card suit strings
    constexpr SWStringView CardSuitStrings[] =
    {
        TEXT(" "), TEXT("♣"), TEXT("♦"), TEXT("♥"), TEXT("♠")
    };

    // Fetch the string representation of the given card suit.
    OutString = CardSuitStrings[static_cast<SUInt8>(CardSuit)];
}