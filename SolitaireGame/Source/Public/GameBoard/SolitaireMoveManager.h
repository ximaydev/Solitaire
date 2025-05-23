#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SAConsolePrompt;
enum class ECardRank : SUInt8;
enum class ECardSuit : SUInt8;

class SSolitaireMoveManager final : public SAActor
{
public:
	/** Constructor */
	SSolitaireMoveManager(SWorld* NewWorld, const SGridPositionU32& NewGridPosition, const SWString& TextToShow);

protected:
	/** Called when the player presses Enter after typing a line of input. */
	void OnEnterClicked(const SWString& Line);

	/** Executes the card move based on the previously selected option and typed command. */
	void ExecuteMoveCommand();

	/** Parse card */
	void ParseCard(const SWString& Command, ECardRank& OutCardRank, ECardSuit& OutCardSuit);

	/** Parses the move command into two tokens representing cards */
	bool ParseMoveCommand(const SWString& MoveCommand, SWString& OutCardToken1, SWString& OutCardToken2);

	/** Reset Selected Option and Move Command */
	void ResetInputs();

	/** The option selected by the player, defining the intended action. */
	SWString SelectedOption;

	/** The detailed command input by the player, describing the specific action. */
	SWString MoveCommand;

	/** Prompt that displays available options and receives player move input via console. */
	SUniquePtr<SAConsolePrompt> MoveInputPrompt;
};