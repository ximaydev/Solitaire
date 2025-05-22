#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SAConsolePrompt;

class SSolitaireMoveManager final : public SAActor
{
public:
	/** Constructor */
	SSolitaireMoveManager(const SGridPositionU32& NewGridPosition, const SWString& TextToShow);

protected:
	/** Called when the player presses Enter after typing a line of input. */
	void OnEnterClicked(SWString& Line);

	/** Executes the card move based on the previously selected option and typed command. */
	void ExecuteMoveCommand();

	/** Parse card */
	void ParseCard(const SWString& Command, ECardRank& OutCardRank, ECardSuit& OutCardSuit);

	/** Parses the move command into two tokens representing cards */
	bool ParseMoveCommand(const SWString& MoveCommand, SWString& OutCardToken1, SWString& OutCardToken2);

	/** The option selected by the player, defining the intended action. */
	SWString SelectedOption;

	/** The detailed command input by the player, describing the specific action. */
	SWString MoveCommand;

	/** Prompt that displays available options and receives player move input via console. */
	SUniquePtr<SAConsolePrompt> MoveInputPrompt;
};