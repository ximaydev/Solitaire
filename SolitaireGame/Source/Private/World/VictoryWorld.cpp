#include "SolitaireGamePCH.h"
#include "World/VictoryWorld.h"
#include "World/MainMenuWorld.h"
#include "Framework/ConsoleSelector.h"
#include "Core/Console.h"

SBool SVictoryWorld::Initialize()
{
    // To be sure, set the use of the input system to true
    GSolitaireEngine->SetUseConsoleInputHandler(false);

    // Get Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Text message shown after player completes the game successfully
    SWString EndWorldString = TEXT("Well done! You've completed the Solitaire and won the game!");

    // Calculate horizontal position to center the string on screen
    SUInt32 PositionX = (ConsoleRenderer->GetScreenWidth() / 2) - (static_cast<SUInt32>(EndWorldString.size()) / 2);

    // Calculate vertical position to be at 75% of the screen height
    SUInt32 PositionY = static_cast<SUInt32>(ConsoleRenderer->GetScreenHeight() * 0.10);

    // Draw the message at the calculated position (centered horizontally)
    ConsoleRenderer->Write(SGridPositionU32(PositionX, PositionY), EndWorldString, static_cast<SUInt32>(EndWorldString.size()), true, FG_DARK_GRAY | ConsoleRenderer->GetCurrentBackgroundColor());

    // Define the initializer list with action name and callback
    const SInitializerList<SPair<SWStringView, SCallback>> ConsoleSelectorActions =
    {
       { TEXT("Back to Main Menu"), std::bind(&SVictoryWorld::HandleReturnToMainMenu, this) },
       { TEXT("Exit Game"), std::bind(&SVictoryWorld::HandleExitGame, this) }
    };

    // Spawn the actor with the initializer list, position, and owner, add offset to the position
    SpawnActor<SAConsoleSelector>(ConsoleSelectorActions, SGridPositionU32(PositionX - 7, PositionY + 10), AsShared());

	return true;
}

void SVictoryWorld::HandleReturnToMainMenu()
{
    // Create Main Menu Map
    GSolitaireEngine->CreateInitialMap<SMainMenuWorld>(false);
}

void SVictoryWorld::HandleExitGame()
{
    // Close the game
    GConsole->ShutDown();
}