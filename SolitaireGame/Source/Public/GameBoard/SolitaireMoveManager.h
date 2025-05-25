#pragma once
#include "Framework/Actor.h"
#include "GameBoard/Tableau.h"
#include "GameBoard/FoundationList.h"

/** Forward Declarations */
class SAConsolePrompt;
enum class ECardRank : SUInt8;
enum class ECardSuit : SUInt8;

/** Manages and processes player input for moving cards within the Solitaire game. */
class SSolitaireMoveManager final : public SAActor
{
public:
	/** Constructor */
	SSolitaireMoveManager(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld>  NewWorld, const SWString& TextToShow);

protected:
	/** Called when the player presses Enter after typing a line of input. */
	void OnEnterClicked(const SWString& Line);

	/** Executes the card move based on the previously selected option and typed command. */
	void ExecuteMoveCommand();

	/** Reset Selected Option and Move Command */
	void ResetInputs();

	/** Parse card */
	void ParseCard(const SWString& Command, ECardRank& OutCardRank, ECardSuit& OutCardSuit);

	/** Parses the move command into two tokens representing cards */
	void ParseMoveCommand(const SWString& MoveCommand, SWString& OutCardToken1, SWString& OutCardToken2);

	/** Searches for a specific card in the tableau based on rank and suit. */
	bool FindCardInTableau(const SATableau::TableauArray& Cards, ECardRank Rank, ECardSuit Suit, SSharedPtr<SACard>& OutCard, SUInt32& OutCardIndex, SUInt32& OutColumnIndex);

	/** Searches the card from waste pile which matches the given rank and suit. */
	bool FindCardInWastePile(const SVector<SSharedPtr<SACard>>& WastePileCards, const ECardRank RankToFind, const ECardSuit SuitToFind, SSharedPtr<SACard>& OutCard, SUInt32& OutCardIndex);

	/** Searches for a card in the foundation piles that matches the given card (Card1). */
	bool FindCardInFoundation(const SAFoundationList::FoundationPilesArray& FoundationColumns, const SSharedPtr<SACard> Card1, SSharedPtr<SACard>& OutCard2, SUInt32& OutCard2ColumnIndex);

	/** Builds a chain of cards starting from the given card. */
	SVector<SSharedPtr<SACard>> BuildCardChain(SSharedPtr<SACard> StartCard);

	/** Removes a chain of cards from a tableau column. */
	void RemoveCardChainFromTableau(SATableau::TableauArray& Cards, SUInt32 ColumnIndex, SUInt32 CardIndex, SUInt32 ChainSize);

	/** Inserts a chain of cards into a tableau column after a specific card. */
	void InsertCardChainInTableau(SATableau::TableauArray& Cards, SUInt32 ColumnIndex, SInt32 InsertAfterIndex, SVector<SSharedPtr<SACard>>& Chain);

	/** Attaches a chain of cards below a given card in the tableau, updating their positions and links. */
	void AttachCardChainBelowCard(const SVector<SSharedPtr<SACard>>& Chain, SSharedPtr<SACard> CardAbove);

	/** The option selected by the player, defining the intended action. */
	SWString SelectedOption;

	/** The detailed command input by the player, describing the specific action. */
	SWString MoveCommand;

	/** Prompt that displays available options and receives player move input via console. */
	SUniquePtr<SAConsolePrompt> MoveInputPrompt;
};