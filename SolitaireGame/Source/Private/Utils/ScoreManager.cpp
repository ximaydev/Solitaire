#include "SolitaireGamePCH.h"
#include "Utils/ScoreManager.h"
#include "Config//IniFileManager.h"
#include "Config/IniFile.h"
#include "Config/IniReader.h"

SAScoreManager::SAScoreManager(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SAActor(NewGridPosition, NewWorld)
{
    // Get player score file
    SSharedPtr<SIniFile> PlayerScoreFile = SIniFileManager::GetInstance()->GetConfigFile(PlayerScoresConfig);

    // Reload the config file and update the internal config map with the latest data from disk
    SIniReader::LoadFromFile(PlayerScoresConfig, PlayerScoreFile->GetConfigFileMap_Mutable());

    // Get all section names (each represents a score entry)
    SVector<SWString> Sections;
    PlayerScoreFile->GetAllSections(Sections);

    // Determine how many entries to display
    const SSize Count = std::min<SSize>(MaxScoreEntry, Sections.size());

    // Get Console Renderer 
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Message to display as the header for the scores list
    SWString HeaderString = TEXT("Most Recent 10 Player Scores:");

    // Write the header string to the console
    ConsoleRenderer->Write(GridPosition, HeaderString, static_cast<SUInt32>(HeaderString.size()), true, FG_LIGHT_MAGENTA | ConsoleRenderer->GetCurrentBackgroundColor());

    for (SSize Index = 0; Index < Count; Index++)
    {
        // Access the N-th most recent score section
        const SWString& Section = Sections[Sections.size() - Count + Index];

        // Retrieve player name and move count from the section
        SWString Name = PlayerScoreFile->GetValueFromKey<SWString>(Section, TEXT("PlayerName"));
        SUInt32 Moves = PlayerScoreFile->GetValueFromKey<SUInt32>(Section, TEXT("MoveCount"));

        // Prepare a wide string stream to build the display string
        std::wstringstream ss;

        // Append player name, separator and move count to the stream
        ss << Name << TEXT(" - ") << Moves << TEXT(" moves");

        // Convert stream content to SWString for display
        SWString Display = ss.str();

        // Render score entry on screen
        constexpr SUInt32 TopOffset = 2;
        ConsoleRenderer->Write(SGridPositionU32(GridPosition.first, GridPosition.second + static_cast<SUInt32>(Index) + TopOffset), Display, static_cast<SUInt32>(Display.size()), true, FG_LIGHT_MAGENTA | ConsoleRenderer->GetCurrentBackgroundColor());
    }
}