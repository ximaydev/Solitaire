#include "SolitaireGamePCH.h"
#include "World/VictoryWorld.h"
#include "World/MainMenuWorld.h"
#include "Framework/ConsoleSelector.h"
#include "Core/Console.h"
#include "Config/IniFile.h"
#include "Config/IniFileManager.h"
#include "Config/IniWriter.h"

SBool SVictoryWorld::Initialize()
{
    // To be sure, set the use of the input system to true
    GSolitaireEngine->SetUseConsoleInputHandler(false);

    // Get Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Text message shown after player completes the game successfully
    SWString EndWorldString = TEXT("Well done! You've completed the Solitaire and won the game!");

    // Calculate horizontal position to center the string on screen
    SUInt32 PositionX = (static_cast<SUInt32>(ConsoleRenderer->GetScreenWidth() / 2)) - (static_cast<SUInt32>(EndWorldString.size()) / 2);

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

    // Spawn Console prompt
    SWString TypeNameString = TEXT("Type your name:");
    SConsoleLineCommitted Callback = std::bind(&SVictoryWorld::HandleNameCommitted, this, std::placeholders::_1);
    SpawnActor<SAConsolePrompt>(SGridPositionU32(PositionX, PositionY + 3), AsShared(), FG_DARK_GRAY | ConsoleRenderer->GetCurrentBackgroundColor(), TypeNameString, Callback);

    // Spawn the actor with the initializer list, position, and owner, add offset to the position
    SpawnActor<SAConsoleSelector>(ConsoleSelectorActions, SGridPositionU32(PositionX, PositionY + 5), AsShared());

	return true;
}

void SVictoryWorld::HandleReturnToMainMenu()
{
    // Check if the player typed his name
    if (NameTyped)
    {
        // Create Main Menu Map
        GSolitaireEngine->CreateInitialMap<SMainMenuWorld>(false);
    }
}

void SVictoryWorld::HandleExitGame()
{
    // Check if the player typed his name
    if (NameTyped)
    {
        // Close the game
        GConsole->ShutDown();
    }
}

void SVictoryWorld::HandleNameCommitted(const SWString& Line)
{
    // Remove whitespace from the input line
    SStringLibrary::RemoveWhiteSpaces(const_cast<SWString&>(Line));

    // If the cleaned line is not empty, proceed to save the new score entry
    if (Line.size() != 0 && !NameTyped)
    {
        // Get the last section name from the score file (e.g., "Score_0015")
        SWString LastSection;
        SIniFileManager::GetInstance()->GetConfigFile(PlayerScoresConfig)->GetLastSection(LastSection);

        // If no last section was found (empty string), initialize it to the first score section "Score_0" (it will convert to Score_1)
        if (LastSection.empty())
        {
            LastSection = TEXT("Score_0");
        }

        // Define the prefix used for score sections
        const SWString Prefix = TEXT("Score_");

        // Extract the numeric part from the last section name (e.g., "15")
        SWString NumberPart = LastSection.substr(Prefix.size() + 1);

        // Convert the numeric part from string to integer
        SUInt32 Index = SStringLibrary::Convert<SUInt32>(NumberPart);

        // Create the new section name by appending the current index (should increment by 1)
        SWString NewSectionName = Prefix + std::to_wstring(++Index);

        // Initialize INI writer to add a new section and key-value pairs
        SIniWriter IniWriter(PlayerScoresConfig);

        // Add new section with the generated name (e.g., "Score_0016")
        IniWriter.AddSection(SStringLibrary::WideStringToString(NewSectionName));

        // Add player's name under the new section
        IniWriter.AddPairKeyValue("PlayerName", SStringLibrary::WideStringToString(Line));

        // Add move count initialized to zero
        IniWriter.AddPairKeyValue("MoveCount", std::to_string(PlayerMoves));

        // Save changes to the INI file
        IniWriter.SaveToFile();

        // Mark that the name was successfully typed and saved
        NameTyped = true;
    }
}