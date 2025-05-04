#include "SolitaireGamePCH.h"
#include "Main.h"
#include "Globals.h"
#include "Core/Logger/LoggerMacros.h"
#include "Core/Logger/LogCategories.h"
#include <Core/Ini/INIFileManager.h>
#include <Core/INI/INIFile.h>

int main()
{
    Core::Paths::CreateDirectories();

    SINIFileManager::GetInstance()->LoadConfigFilesFromDisk();

    SFileLogger::GetInstance()->WaitForLoggingToFinish();
    return 0;
}