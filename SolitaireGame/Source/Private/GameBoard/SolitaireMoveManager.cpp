#include "SolitaireGamePCH.h"

SSolitaireMoveManager::SSolitaireMoveManager(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, const SWString& TextToShow) 
	: SAActor(NewGridPosition, NewWorld)
{
	// Create Move Input Prompt
	MoveInputPrompt = NewWorld->SpawnActor<SUniquePtr<SAConsolePrompt>, SAConsolePrompt>(NewGridPosition, NewWorld, FG_DARK_GRAY | SConsoleRenderer::GetInstance()->GetCurrentBackgroundColor(), TextToShow, std::bind(&SSolitaireMoveManager::OnEnterClicked, this, std::placeholders::_1));
}

void SSolitaireMoveManager::OnEnterClicked(const SWString& Line)
{
	// Check if the user hasn't selected an option yet (first input step)
	if (SelectedOption == TEXT(""))
	{
		// Store the selected option and move ownership of the input line
		SelectedOption = Line;

		// Remove white spaces
		SStringLibrary::RemoveWhiteSpaces(SelectedOption);

		// Check if the input string consists only of digit characters (e.g., "2", "10")
		if (!std::all_of(SelectedOption.begin(), SelectedOption.end(), ::isdigit) || SelectedOption == TEXT(""))
		{
			// Reset Inputs because we haven't passed a number
			ResetInputs();
			return;
		}

		// Convert the selected option string to integer representing the operation range
		SUInt32 RangeOfOperation = SStringLibrary::Convert<SUInt32>(SelectedOption);

		// Define the allowed operation range
		constexpr SUInt32 MinOperationNum = 1;
		constexpr SUInt32 MaxOperationNum = 5;

		// Validate the operation range
		if (RangeOfOperation < MinOperationNum || RangeOfOperation > MaxOperationNum)
		{
			// Log a warning if the selected operation index is out of valid bounds
			S_LOG_WARNING(LogGameBoard, TEXT("Selected operation index (%u) is out of valid range [%u, %u]"), RangeOfOperation, MinOperationNum, MaxOperationNum);

			// Reset inputs to allow the user to try again
			ResetInputs();
			return;
		}

		// Get world
		SSharedPtr<SGameBoardWorld> GameBoardWorld = GetWorld<SGameBoardWorld>();

		if (SelectedOption == TEXT("4"))
		{
			// Card pointers
			SSharedPtr<SACard> Card1 = nullptr;
			SSharedPtr<SACard> Card2 = nullptr;

			SUInt32 Card1ColumnIndex = INDEX_NONE;

			// Get the current waste pile cards
			const SVector<SSharedPtr<SACard>>& WastePileCards = GameBoardWorld->GetStockPile()->GetWastePile()->GetCards();

			if (WastePileCards.empty())
			{
				S_LOG_WARNING(LogGameBoard, TEXT("Waste pile is empty - Card1 is nullptr"));
				ResetInputs();
				return;
			}

			// Get the top card from the waste pile
			Card2 = WastePileCards.back();

			// Find card2 in the foundation list
			FindCardInFoundation(GameBoardWorld->GetFoundationList()->GetCards(), Card2, Card1, Card1ColumnIndex);

			if (GameBoardWorld->GetGameRules()->CanMoveWastePileToFoundationList(Card2, Card1))
			{
				// Use top card (remove from waste pile)
				GameBoardWorld->GetStockPile()->GetWastePile()->UseTopCard();

				// Add Card1 to the foundation list at the specified column index
				GameBoardWorld->GetFoundationList()->AddNewCardToFoundationList(Card2, Card1ColumnIndex);

				// Increment move count and reset inputs
				GameBoardWorld->IncrementMoveCount();
				ResetInputs();
			}
		}
		else if (SelectedOption == TEXT("5"))
		{
			// Use Stock Pile
			GameBoardWorld->GetStockPile()->UseStockPile();
			
			// Increment move count and reset inputs
			GameBoardWorld->IncrementMoveCount();
			ResetInputs();
		}

	}
	else if (MoveCommand == TEXT("")) // If option was already selected, check if we're waiting for the move command (second input step)
	{
		// Store the move command and move ownership of the input line
		MoveCommand = Line;

		// Change to upper
		MoveCommand = SStringLibrary::ToUpper(MoveCommand);
		
		// Perform the actual card movement
		ExecuteMoveCommand();
	}
	else // If both SelectedOption and MoveCommand are already set, but we still get input, log an warning
	{
		// Log an error because the state machine should have reset or handled the inputs already
		S_LOG_WARNING(LogGameBoard, TEXT("Unexpected input: MoveCommand and SelectedOption are already set"));

		// Reset 
		MoveCommand = TEXT("");
		SelectedOption = TEXT("");
	}
}

void SSolitaireMoveManager::ExecuteMoveCommand()
{
	// Get world
	SSharedPtr<SGameBoardWorld> GameBoardWorld = GetWorld<SGameBoardWorld>();

	// Parse move command
	SWString Token1, Token2;
	ParseMoveCommand(MoveCommand, Token1, Token2);

	SUInt32 Card1ColumnIndex = INDEX_NONE;
	SUInt32 Card1CardIndex = INDEX_NONE;

	SUInt32 Card2ColumnIndex = INDEX_NONE;
	SUInt32 Card2CardIndex = INDEX_NONE;

	// Parse cards from tokens
	ECardRank CardRank1 = ECardRank::None, CardRank2 = ECardRank::None;
	ECardSuit CardSuit1 = ECardSuit::None, CardSuit2 = ECardSuit::None;

	ParseCard(Token1, CardRank1, CardSuit1);
	ParseCard(Token2, CardRank2, CardSuit2);

	// Card pointers
	SSharedPtr<SACard> Card1 = nullptr;
	SSharedPtr<SACard> Card2 = nullptr;

	bool bFoundCard1 = false;
	bool bFoundCard2 = false;

	// Get Cards
	SATableau::TableauArray& Cards = const_cast<SATableau::TableauArray&>(GameBoardWorld->GetTableau()->GetCards());

	if (SelectedOption == TEXT("1"))
	{
		// Find the first card in the tableau matching given rank and suit
		bFoundCard1 = FindCardInTableau(Cards, CardRank1, CardSuit1, Card1, Card1CardIndex, Card1ColumnIndex);

		// Find the second card in the tableau matching given rank and suit
		bFoundCard2 = FindCardInTableau(Cards, CardRank2, CardSuit2, Card2, Card2CardIndex, Card2ColumnIndex);

		// Check if card1 was successfully found
		if (!bFoundCard1)
		{
			// Log warning and reset input if either card was not found
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 is nullptr"));
			ResetInputs();
			return;
		}

		// Check if the move is valid according to game rules and cards are in different columns
		if (GameBoardWorld->GetGameRules()->CanMoveBoardToBoard(Card1, Card2) && Card1ColumnIndex != Card2ColumnIndex)
		{
			// Build a chain of linked cards starting from Card1
			SVector<SSharedPtr<SACard>> Chain = BuildCardChain(Card1);

			// Check if the Card1 is king and Card2 is empty (contain information about column)
			if (!Card2)
			{
				// Parse the target column number from the token and clamp it to valid range [1,7], then convert to 0-based index
				SUInt32 ColumnNum = SStringLibrary::Convert<SUInt32>(Token2);
				ColumnNum = std::clamp<SUInt32>(ColumnNum, 1, 7) - 1;

				// Remove the chain of cards from its original location in the tableau
				RemoveCardChainFromTableau(Cards, Card1ColumnIndex, Card1CardIndex, Chain.size());

				// Reposition the first card in the chain according to the target column's grid position
				const SGridPositionU32& TableauGridPositon = GameBoardWorld->GetTableau()->GetGridPosition();
				Card1->ClearBuffer();
				Card1->SetGridPosition(SGridPositionU32(TableauGridPositon.first + (ColumnNum * 8), TableauGridPositon.second));

				// Insert the entire card chain at the beginning of the new column in the tableau
				Cards[ColumnNum].insert(Cards[ColumnNum].begin(), Chain.begin(), Chain.end());
			}
			else
			{
				// Attach the entire card chain below Card2, updating positions and links accordingly
				AttachCardChainBelowCard(Chain, Card2);

				// Remove the card chain from its original position in the tableau
				RemoveCardChainFromTableau(Cards, Card1ColumnIndex, Card1CardIndex, Chain.size());

				// Insert the card chain into the tableau after Card2's position
				InsertCardChainInTableau(Cards, Card2ColumnIndex, Card2CardIndex, Chain);
			}
				// Reveal Card
				if (Cards[Card1ColumnIndex].size() > 0)
					Cards[Card1ColumnIndex].back()->SetIsFaceUp(true);
		}
		else
		{
			// Log
			S_LOG(LogGameBoard, TEXT("Move rejected: Card1 can't be placed on Card2 due to game rules"));
		}
	}
	else if (SelectedOption == TEXT("2"))
	{
		// Try to find the card in the tableau using its rank and suit.
		bFoundCard1 = FindCardInTableau(Cards, CardRank1, CardSuit1, Card1, Card1CardIndex, Card1ColumnIndex);

		// Check if the card was NOT found
		if (!bFoundCard1)
		{
			// Log an error indicating that Card1 is null, so the operation cannot proceed
			S_LOG_ERROR(LogGameBoard, TEXT("Card1 is nullptr we can't continue"));

			// Reset any inputs or state related to the card selection
			ResetInputs();
			return;
		}
		
		// Try to find the card in the foundation list using its rank and suit.
		bFoundCard2 = FindCardInFoundation(GameBoardWorld->GetFoundationList()->GetCards(), Card1, Card2, Card2ColumnIndex);

		// If move is valid according to game rules, proceed
		if (GameBoardWorld->GetGameRules()->CanMoveBoardToFoundationList(Card1, Card2))
		{
			// Clear buffer
			Card1->ClearBuffer();

			// Remove from tableau
			Cards[Card1ColumnIndex].pop_back();

			// Set last card's next card to nullptr
			if (Cards[Card1ColumnIndex].size() > 0)
				Cards[Card1ColumnIndex].back()->SetNextCard(nullptr);

			// Add Card1 to the foundation list at the specified column index
			GameBoardWorld->GetFoundationList()->AddNewCardToFoundationList(Card1, Card2ColumnIndex);

			// Reveal Card
			if (Cards[Card1ColumnIndex].size() > 0)
				Cards[Card1ColumnIndex].back()->SetIsFaceUp(true);
		}

	}
	else if (SelectedOption == TEXT("3"))
	{
		// Get the current waste pile cards
		const SVector<SSharedPtr<SACard>>& WastePileCards = GameBoardWorld->GetStockPile()->GetWastePile()->GetCards();

		if (WastePileCards.empty())
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Waste pile is empty - Card1 is nullptr"));
			ResetInputs();
			return;
		}

		// Get the top card from the waste pile
		Card1 = WastePileCards.back();

		// Try to find the destination card in the tableau
		bFoundCard1 = FindCardInTableau(GameBoardWorld->GetTableau()->GetCards(), CardRank1, CardSuit1, Card2, Card1CardIndex, Card1ColumnIndex);

		// Special handling for Kings: they can only be placed in empty columns
		if (Card1->GetCardInfo().GetCardRank() == ECardRank::King && !bFoundCard1)
		{
			// Parse and clamp the column number to a valid range [0,6]
			SUInt32 ColumnNum = SStringLibrary::Convert<SUInt32>(Token1);
			ColumnNum = std::clamp(ColumnNum, 1U, 7U) - 1;

			// Use top card (remove from waste pile)
			GameBoardWorld->GetStockPile()->GetWastePile()->UseTopCard();

			// Set the card's new grid position on an empty slot
			const SGridPositionU32& TableauGridPosition = GameBoardWorld->GetTableau()->GetGridPosition();
			Card1->SetGridPosition(SGridPositionU32(TableauGridPosition.first + (ColumnNum * 8), TableauGridPosition.second));

			// Place the King card into the appropriate tableau column
			Cards[ColumnNum].push_back(Card1);

			// Increment move count and reset inputs
			GameBoardWorld->IncrementMoveCount();
			ResetInputs();

			return;
		}

		// If Card1 was not found in the tableau and it's not a King, exit
		if (!bFoundCard1)
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 not found in tableau"));
			ResetInputs();
			return;
		}

		// If the move from the waste pile to the tableau is valid, perform it
		if (GameBoardWorld->GetGameRules()->CanMoveWastePileToBoard(Card1, Card2))
		{
			// Use top card (remove from waste pile)
			GameBoardWorld->GetStockPile()->GetWastePile()->UseTopCard();

			// Set Next Card
			const SGridPositionU32& Card2GridPosition = Card2->GetGridPosition();
			Card2->SetNextCard(SGridPositionU32(Card2GridPosition.first, Card2GridPosition.second + 3), Card1);

			// Add Card1 to the Cards
			Cards[Card1ColumnIndex].push_back(Card1);
		}
	}
	else
	{
		S_LOG_WARNING(LogGameBoard, TEXT("Unknown command: %s"), SelectedOption.data());
	}

	// Reset inputs and increase move counter
	GameBoardWorld->IncrementMoveCount();
	ResetInputs();
}

void SSolitaireMoveManager::ResetInputs()
{
	// Reset the SelectedOption and MoveCommand
	SelectedOption = TEXT("");
	MoveCommand = TEXT("");
}

void SSolitaireMoveManager::ParseCard(const SWString& Command, ECardRank& OutCardRank, ECardSuit& OutCardSuit)
{
	// Check if the input command is at least 2 characters long (suit + rank)
	if (Command.size() < 2)
	{
		S_LOG_WARNING(LogGameBoard, TEXT("Command is too short"));
		return;
	}	

	// Extract the first character from Command as a substring representing the suit
	SWString SuitString = Command.substr(0, 1);

	// Extract the rest of the string after the first character as rank substring
	SWString RankString = Command.substr(1);

	// Convert the suit substring to the ECardSuit enum
	OutCardSuit = SCardConverter::StringToCardSuit(SuitString);

	// Convert the rank substring to the ECardRank enum
	OutCardRank = SCardConverter::StringToCardRank(RankString);
}

void SSolitaireMoveManager::ParseMoveCommand(const SWString& MoveCommand, SWString& OutCardToken1, SWString& OutCardToken2)
{
	// Create a stream from the input command string to tokenize it
	SWIStringStream StringStream(MoveCommand);

	// Temporary container for the extracted tokens
	SVector<SWString> Tokens;

	// Temporary string to hold each extracted token
	SWString Temp;

	// Read up to 2 tokens from the stream
	while (StringStream >> Temp)
	{
		// Move the extracted token into the tokens vector to avoid unnecessary copying
		Tokens.push_back(Temp);
	}

	// Assign the parsed tokens to the output parameters using move semantics
	OutCardToken1 = Tokens.size() > 0 ? std::move(Tokens[0]) : TEXT("");
	OutCardToken2 = Tokens.size() > 1 ? std::move(Tokens[1]) : TEXT("");
}

bool SSolitaireMoveManager::FindCardInTableau(const SATableau::TableauArray& Cards, ECardRank Rank, ECardSuit Suit, SSharedPtr<SACard>& OutCard, SUInt32& OutCardIndex, SUInt32& OutColumnIndex)
{
	// Loop through each column in the tableau.
	for (SUInt32 ColumnIndex = 0; ColumnIndex < Cards.size(); ColumnIndex++)
	{
		// Get a reference to the current column.
		const auto& Column = Cards[ColumnIndex];

		// Loop through each card in the current column.
		for (SUInt32 Index = 0; Index < Column.size(); ++Index)
		{
			// Get a reference to the current card.
			const auto& Card = Column[Index];

			// Retrieve the card's information (rank and suit).
			const auto& Info = Card->GetCardInfo();

			// Check if the current card matches the desired rank and suit.
			if (Info.GetCardRank() == Rank && Info.GetCardSuit() == Suit)
			{
				// Output the found card and its indices.
				OutCard = Card;
				OutCardIndex = Index;
				OutColumnIndex = ColumnIndex;

				// Return true indicating the card was found.
				return true;
			}
		}
	}

	// Return false if no matching card was found in the entire tableau.
	return false;
}

bool SSolitaireMoveManager::FindCardInWastePile(const SVector<SSharedPtr<SACard>>& WastePileCards, const ECardRank RankToFind, const ECardSuit SuitToFind, SSharedPtr<SACard>& OutCard, SUInt32& OutCardIndex)
{
	// Iterate over all cards in the waste pile
	for (SUInt32 Index = 0; Index < WastePileCards.size(); Index++)
	{
		const auto& Card = WastePileCards[Index];

		// Check if this card matches the specified rank and suit
		if (Card->GetCardInfo().GetCardRank() == RankToFind && Card->GetCardInfo().GetCardSuit() == SuitToFind)
		{
			// Store the matching card and its index
			OutCard = Card;
			OutCardIndex = Index;

			// Indicate that a match was found
			return true;
		}
	}

	// No matching card found in the waste pile
	return false;
}

bool SSolitaireMoveManager::FindCardInFoundation(const SAFoundationList::FoundationPilesArray& FoundationColumns, const SSharedPtr<SACard> Card1, SSharedPtr<SACard>& OutCard2, SUInt32& OutCard2ColumnIndex)
{
	// Initialize the output column index for the found card
	OutCard2ColumnIndex = 0;

	// Iterate over each foundation column
	for (const auto& Column : FoundationColumns)
	{
		// If the foundation column is empty, treat this as a valid spot to place the card
		if (Column.size() == 0)
		{
			// Check if the Card1 is Ace and return true otherwise return false
			if (Card1->GetCardInfo().GetCardRank() == ECardRank::Ace)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		// Get last card (valid only)
		SSharedPtr<SACard> Card = Column.back();

		// Skip rank comparison if Card1 is an Ace, since Aces start the foundation piles
		if (Card1->GetCardInfo().GetCardRank() != ECardRank::Ace)
		{
			// Calculate the expected rank that should come after the current card in the foundation sequence
			ECardRank ExpectedRank = static_cast<ECardRank>(static_cast<SUInt8>(Card->GetCardInfo().GetCardRank()) + 1);

			// Check if Card1 matches the expected rank and suit required to be placed on top of this card in foundation
			if (ExpectedRank == Card1->GetCardInfo().GetCardRank() && Card->GetCardInfo().GetCardSuit() == Card1->GetCardInfo().GetCardSuit())
			{
				// Assign the found card in foundation to output parameter
				OutCard2 = Card;

				// Indicate success in finding a matching card
				return true;
			}
		}

		// Increment the column index since Card1 was not found in this column
		++OutCard2ColumnIndex;
	}

	// Return false if no matching card was found in any foundation column
	return false;

}

SVector<SSharedPtr<SACard>> SSolitaireMoveManager::BuildCardChain(SSharedPtr<SACard> StartCard)
{
	// Create a vector to hold the chain of cards.
	SVector<SSharedPtr<SACard>> Chain;

	// Preallocate space for up to 13 cards (max possible chain).
	Chain.reserve(13);

	// Set the current card to the start of the chain.
	SSharedPtr<SACard> Current = StartCard;

	// Traverse the chain until there are no more linked cards.
	while (Current)
	{
		// Add the current card to the chain.
		Chain.push_back(Current);

		// Move to the next card in the chain.
		Current = Current->GetNextCard();
	}

	// Reduce memory usage.
	Chain.shrink_to_fit();

	// Return the completed card chain.
	return Chain;
}

void SSolitaireMoveManager::RemoveCardChainFromTableau(SATableau::TableauArray& Cards, SUInt32 ColumnIndex, SUInt32 CardIndex, SUInt32 ChainSize)
{
	// Erase the range of cards from the column.
	Cards[ColumnIndex].erase(Cards[ColumnIndex].begin() + CardIndex, Cards[ColumnIndex].begin() + CardIndex + ChainSize);

	// Set to nullpt last element in the previous column
	if (Cards[ColumnIndex].size() > 0)
		Cards[ColumnIndex].back()->SetNextCard(nullptr);
}

void SSolitaireMoveManager::InsertCardChainInTableau(SATableau::TableauArray& Cards, SUInt32 ColumnIndex, SInt32 InsertAfterIndex, SVector<SSharedPtr<SACard>>& Chain)
{
	// Insert the cards into the column at the desired position using move semantics for efficiency.
	Cards[ColumnIndex].insert(Cards[ColumnIndex].begin() + InsertAfterIndex + 1, std::make_move_iterator(Chain.begin()), std::make_move_iterator(Chain.end()));
}

void SSolitaireMoveManager::AttachCardChainBelowCard(const SVector<SSharedPtr<SACard>>& Chain, SSharedPtr<SACard> CardAbove)
{
	// Do nothing if the chain is empty.
	if (Chain.empty()) return;

	// Get the grid position of the card above (to calculate new positions for the chain).
	const SGridPositionU32& CardAboveGridPosition = CardAbove->GetGridPosition();

	// Start new row position for the first card in the chain (3 rows below the card above).
	SUInt32 Row = CardAboveGridPosition.second + 3;

	// Clear buffer of the first card in the chain.
	Chain[0]->ClearBuffer();

	// Set the first card of the chain as the next card of CardAbove.
	CardAbove->SetNextCard(SGridPositionU32(CardAboveGridPosition.first, Row), Chain[0]);

	// Attach remaining cards in the chain one by one.
	for (SUInt32 Index = 1; Index < Chain.size(); Index++)
	{
		// Each card goes 3 rows below the previous one.
		Row += 3;

		// Clear any buffered state of the card before re-linking.
		Chain[Index]->ClearBuffer();

		// Set the current card as the next card of the previous one.
		Chain[Index - 1]->SetNextCard({ CardAboveGridPosition.first, Row }, Chain[Index]);
	}

	// Last card in the chain should not link to any further card.
	Chain.back()->SetNextCard(nullptr);
}