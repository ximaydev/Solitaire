#include "SolitaireEnginePCH.h"
#include "Globals.h"
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace Core::Paths
{
    /** Global variable to store the project root path */
    const SWString GProjectRootPath = []
        {
            SWideChar ExePath[MAX_PATH];
            SWideChar FinalPath[MAX_PATH];
            GetModuleFileNameW(NULL, ExePath, MAX_PATH);
            PathCombineW(FinalPath, ExePath, TEXT("..\\..\\.."));
            return SWString(FinalPath);
        }();

    /** Global variable to store the content directory path */
    const SWString GProjectContentPath = GProjectRootPath + TEXT("\\Content");

    /** Global variable to store the config directory path */
    const SWString GProjectConfigPath = GProjectRootPath + TEXT("\\Config");

    /** Global variable to store the saved directory path */
    const SWString GProjectSavedPath = GProjectRootPath + TEXT("\\Saved");

    void CreateDirectories()
    {
        // Always ensure the Saved directory exists, regardless of build type.
        if (!fs::exists(GProjectSavedPath))
        {
            fs::create_directory(GProjectSavedPath);
            S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GProjectSavedPath.c_str())
        }
        
        #if DEBUG
            // In debug builds, ensure that the Content directory exists.
            // Although it should always be present, this is a safety check for clarity during development.
            if (!fs::exists(GProjectContentPath))
            {
                fs::create_directory(GProjectContentPath);
                S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GProjectContentPath.c_str())
            }

            // In debug builds, ensure that the Config directory exists.
            // This directory should also always be present; check included for development clarity.
            if (!fs::exists(GProjectConfigPath))
            {
                fs::create_directory(GProjectConfigPath);
                S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GProjectConfigPath.c_str())
            }
        #endif
    }
}