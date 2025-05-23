#include "SolitaireGamePCH.h"
#include "GameBoard/SolitaireMoveManager.h"
#include "GameBoard/FoundationList.h"
#include "GameBoard/StockPile.h"
#include "GameBoard/Tableau.h"
#include "GameBoard/WastePile.h"
#include "GameBoard/Card.h"
#include "World/GameBoardWorld.h"
#include "Rules/SolitaireRules.h"
#include "Framework/ConsolePrompt.h"

SSolitaireMoveManager::SSolitaireMoveManager(SWorld* NewWorld, const SGridPositionU32& NewGridPosition, const SWString& TextToShow) : SAActor(NewGridPosition)
{
	// Create Move Input Prompt
	MoveInputPrompt = NewWorld->SpawnActor<SUniquePtr<SAConsolePrompt>, SAConsolePrompt>(NewGridPosition, FG_DARK_GRAY | SConsoleRenderer::GetInstance()->GetCurrentBackgroundColor(), TextToShow, std::bind(&SSolitaireMoveManager::OnEnterClicked, this, std::placeholders::_1));
	MoveInputPrompt->Initialize();
}

void SSolitaireMoveManager::OnEnterClicked(const SWString& Line)
{
	// Check if the user hasn't selected an option yet (first input step)
	if (SelectedOption == TEXT(""))
	{
		// Store the selected option and move ownership of the input line
		SelectedOption = Line;
	}
	else if (MoveCommand == TEXT("")) // If option was already selected, check if we're waiting for the move command (second input step)
	{
		// Store the move command and move ownership of the input line
		MoveCommand = Line;

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
	SGameBoardWorld* GameBoardWorld = GetWorld<SGameBoardWorld>();

	// Ensure the move command contains at least two cards (each 2 characters long)
	if (MoveCommand.size() < 4)
	{
		S_LOG_WARNING(LogGameBoard, TEXT("Move Command is invalid"));
		ResetInputs();
		return;
	}

	// Parse move command
	SWString Token1, Token2;
	if (!ParseMoveCommand(MoveCommand, Token1, Token2))
	{
		ResetInputs();
		return;
	}

	// Parse cards from tokens
	ECardRank CardRank1, CardRank2;
	ECardSuit CardSuit1, CardSuit2;

	ParseCard(Token1, CardRank1, CardSuit1);
	ParseCard(Token2, CardRank2, CardSuit2);

	// Card pointers
	SSharedPtr<SACard> Card1 = nullptr;
	SSharedPtr<SACard> Card2 = nullptr;

	bool bFoundCard1 = false;
	bool bFoundCard2 = false;

	if (SelectedOption == TEXT("1"))
	{
		// Iterate over all columns in the tableau
		for (const auto& Column : GameBoardWorld->GetTableau()->GetCards())
		{
			// Iterate over all cards in the current column
			for (const auto& Card : Column)
			{
				// Get the card information (rank and suit)
				const auto& Info = Card->GetCardInfo();

				// Check if this card matches the first card to find
				if (!bFoundCard1 && Info.GetCardRank() == CardRank1 && Info.GetCardSuit() == CardSuit1)
				{
					Card1 = Card;
					bFoundCard1 = true;
				}
				// Check if this card matches the second card to find
				else if (!bFoundCard2 && Info.GetCardRank() == CardRank2 && Info.GetCardSuit() == CardSuit2)
				{
					Card2 = Card;
					bFoundCard2 = true;
				}

				// If both cards found, break inner loop
				if (bFoundCard1 && bFoundCard2)
					break;
			}

			// If both cards found, break outer loop
			if (bFoundCard1 && bFoundCard2)
				break;
		}

		// Check if both cards were found
		if (!bFoundCard1 || !bFoundCard2)
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 or Card2 is nullptr"));
			ResetInputs();
			return;
		}

		// Attempt to move card if allowed by game rules
		if (GameBoardWorld->GetGameRules()->CanMoveBoardToBoard(Card1, Card2))
		{
			// TODO: Implement the move logic here
		}
	}
	else if (SelectedOption == TEXT("2"))
	{
		// Search for the first card in the tableau
		for (const auto& Column : GameBoardWorld->GetTableau()->GetCards())
		{
			// Iterate through all cards in the current tableau column
			for (const auto& Card : Column)
			{
				// Check if this card matches the first card (source in the move)
				if (Card->GetCardInfo().GetCardRank() == CardRank1 && Card->GetCardInfo().GetCardSuit() == CardSuit1)
				{
					Card1 = Card;
					bFoundCard1 = true;
					break;
				}
			}

			// If we have found the card we leave from it
			if (bFoundCard1)
				break;
		}

		// Search for the second card in the foundation pile
		for (const auto& Column : GameBoardWorld->GetFoundationList()->GetCards())
		{
			// Iterate through all cards in the current foundation column
			for (const auto& Card : Column)
			{
				// Check if this card matches the second card (target in the move)
				if (Card->GetCardInfo().GetCardRank() == CardRank2 && Card->GetCardInfo().GetCardSuit() == CardSuit2)
				{
					Card2 = Card;
					bFoundCard2 = true;
					break;
				}
			}

			// If we have found the card we leave from it
			if (bFoundCard1)
				break;
		}

		// Validate that both cards were found
		if (!bFoundCard1 || !bFoundCard2)
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 or Card2 is nullptr"));
			ResetInputs();
			return;
		}

		// If move is valid according to game rules, proceed
		if (GameBoardWorld->GetGameRules()->CanMoveBoardToFoundationList(Card1, Card2))
		{
			// TODO: Implement move logic
		}

	}
	else if (SelectedOption == TEXT("3"))
	{
		// Search for the first card in the waste pile
		for (const auto& Card : GameBoardWorld->GetStockPile()->GetWastePile()->GetCards())
		{
			// Check if this card matches the first card
			if (Card->GetCardInfo().GetCardRank() == CardRank1 && Card->GetCardInfo().GetCardSuit() == CardSuit1)
			{
				Card1 = Card;
				bFoundCard1 = true;
				break;
			}

			// If we have found the card we leave from it
			if (bFoundCard1)
				break;
		}

		// Search for the second card in the tableau
		for (const auto& Column : GameBoardWorld->GetTableau()->GetCards())
		{
			// Iterate through all cards in the current tableau column
			for (const auto& Card : Column)
			{
				// Check if this card matches the second card
				if (Card->GetCardInfo().GetCardRank() == CardRank2 && Card->GetCardInfo().GetCardSuit() == CardSuit2)
				{
					Card2 = Card;
					bFoundCard2 = true;
					break;
				}
			}

			// If we have found the card we leave from it
			if (bFoundCard2)
				break;
		}


		// Validate that both cards were found
		if (!bFoundCard1 || !bFoundCard2)
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 or Card2 is nullptr"));
			ResetInputs();
			return;
		}

		if (GameBoardWorld->GetGameRules()->CanMoveWastePileToBoard(Card1, Card2))
		{
			// TODO: Implement move logic
		}
	}
	else if (SelectedOption == TEXT("4"))
	{
		// Search for the first card in the waste pile
		for (const auto& Card : GameBoardWorld->GetStockPile()->GetWastePile()->GetCards())
		{
			// Check if this card matches the first card
			if (Card->GetCardInfo().GetCardRank() == CardRank1 && Card->GetCardInfo().GetCardSuit() == CardSuit1)
			{
				Card1 = Card;
				bFoundCard1 = true;
				break;
			}

			// If we have found the card we leave from it
			if (bFoundCard1)
				break;
		}

		// Search for the second card in the foundation list
		for (const auto& Column : GameBoardWorld->GetFoundationList()->GetCards())
		{
			// Iterate through all cards in the current foundation list column
			for (const auto& Card : Column)
			{
				// Check if this card matches the second card
				if (Card->GetCardInfo().GetCardRank() == CardRank2 && Card->GetCardInfo().GetCardSuit() == CardSuit2)
				{
					Card2 = Card;
					bFoundCard2 = true;
					break;
				}
			}

			// If we have found the card we leave from it
			if (bFoundCard2)
				break;
		}


		// Validate that both cards were found
		if (!bFoundCard1 || !bFoundCard2)
		{
			S_LOG_WARNING(LogGameBoard, TEXT("Card1 or Card2 is nullptr"));
			ResetInputs();
			return;
		}

		if (GameBoardWorld->GetGameRules()->CanMoveWastePileToFoundationList(Card1, Card2))
		{
			// TODO: Implement move logic
		}
	}
	else
	{
		S_LOG_WARNING(LogGameBoard, TEXT("Unknown command: %s"), SelectedOption.data());
	}

	ResetInputs();
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
	OutCardSuit = StringToCardSuit(SuitString);

	// Convert the rank substring to the ECardRank enum
	OutCardRank = StringToCardRank(RankString);
}

bool SSolitaireMoveManager::ParseMoveCommand(const SWString& MoveCommand, SWString& OutCardToken1, SWString& OutCardToken2)
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

	// If we didn't get exactly 2 tokens, the command is invalid
	if (Tokens.size() != 2)
	{
		// Log a warning to help diagnose issues with malformed commands
		S_LOG_WARNING(LogGameBoard, TEXT("Move Command should have 2 cards"));
		return false;
	}

	// Assign the parsed tokens to the output parameters using move semantics
	OutCardToken1 = std::move(Tokens[0]);
	OutCardToken2 = std::move(Tokens[1]);

	return true;
}

void SSolitaireMoveManager::ResetInputs()
{
	// Reset the SelectedOption and MoveCommand
	SelectedOption = TEXT("");
	MoveCommand = TEXT("");
}