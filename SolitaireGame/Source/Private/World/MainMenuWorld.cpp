#include "SolitaireGamePCH.h"
#include "World/MainMenuWorld.h"
#include "Framework/ConsoleSelector.h"
#include "Core/Console.h"
#include "Utils/ScoreManager.h"

SMainMenuWorld::SMainMenuWorld() : SWorld(false) {}

SBool SMainMenuWorld::Initialize()
{
	// Get console renderer instance
	SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

	// Message displayed after the player completes the game
	SWString MainMenuHeaderString = TEXT("Welcome to the Solitaire Game!");

	// Calculate horizontal position to center the text
	SUInt32 PositionX = (ConsoleRenderer->GetScreenWidth() / 2) - (static_cast<SUInt32>(MainMenuHeaderString.size()) / 2);

	// Position text at 10% from the top of the screen
	SUInt32 PositionY = static_cast<SUInt32>(ConsoleRenderer->GetScreenHeight() * 0.05);

	// Draw the message at the calculated position (centered horizontally)
	ConsoleRenderer->Write(SGridPositionU32(PositionX, PositionY), MainMenuHeaderString, static_cast<SUInt32>(MainMenuHeaderString.size()), true, FG_DARK_GRAY | ConsoleRenderer->GetCurrentBackgroundColor());

	// Define the initializer list with action name and callback
	const SInitializerList<SPair<SWStringView, SCallback>> ConsoleSelectorActions =
	{
	   { TEXT("Exit Game"), std::bind(&SMainMenuWorld::HandleExitGame, this) },
	   { TEXT("Start Game"), std::bind(&SMainMenuWorld::HandleCreateNewMatch, this) }
	};

	// Spawn Console Selector
	SpawnActor<SAConsoleSelector>(ConsoleSelectorActions, SGridPositionU32(PositionX + 8, PositionY * 2), AsShared());

	// Spawn Score Manager
	SpawnActor<SAScoreManager>(SGridPositionU32(5, 10), AsShared());

	return true;
}

void SMainMenuWorld::HandleCreateNewMatch()
{
	// Create new game
	GSolitaireEngine->CreateInitialMap<SGameBoardWorld>(false);
}

void SMainMenuWorld::HandleExitGame()
{
	// Close the game
	GConsole->ShutDown();
}