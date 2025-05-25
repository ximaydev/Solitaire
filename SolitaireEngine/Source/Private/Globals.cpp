#include "SolitaireEnginePCH.h"
#include "Globals.h"
#include <Shlwapi.h>
#include "Core/SolitaireEngine.h"

#pragma comment(lib, "Shlwapi.lib")

/** Represents an invalid or uninitialized index. Typically used to indicate that a valid index was not found. */
const SUInt32 INDEX_NONE = -1;

namespace Core::Paths
{
    /** Global variable to store the project root path */
    const SWString GProjectRootPath = []
        {
            SWideChar ExePath[MAX_PATH];
            SWideChar FinalPath[MAX_PATH];
            GetModuleFileNameW(NULL, ExePath, MAX_PATH);
            PathCombineW(FinalPath, ExePath, TEXT("..\\..\\..\\"));
            return SWString(FinalPath);
        }();

    /** Global variable to store the content directory path */
    const SWString GProjectContentPath = GProjectRootPath + TEXT("Content\\");

    /** Global variable to store the config directory path */
    const SWString GProjectConfigPath = GProjectRootPath + TEXT("Config\\");

    /** Global variable to store the saved directory path */
    const SWString GProjectSavedPath = GProjectRootPath + TEXT("Saved\\");

    /** Global variable to store the generated directory path */
    const SWString GProjectGeneratedPath = GProjectRootPath + TEXT("Generated\\");

    void CreateDirectories()
    {
        // Always ensure the Saved directory exists, regardless of build type.
        if (!fs::exists(GetProjectSavedPath()))
        {
            fs::create_directory(GetProjectSavedPath());
            S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GetProjectSavedPath().c_str())
        }

        #if DEBUG
            // In debug builds, ensure that the Content directory exists.
            // Although it should always be present, this is a safety check for clarity during development.
            if (!fs::exists(GetProjectContentPath()))
            {
                fs::create_directory(GetProjectContentPath());
                S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GetProjectContentPath().c_str())
            }

            // In debug builds, ensure that the Config directory exists.
            // This directory should also always be present; check included for development clarity.
            if (!fs::exists(GetProjectConfigPath()))
            {
                fs::create_directory(GetProjectConfigPath());
                S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GetProjectConfigPath().c_str())
            }

            // In debug builds, ensure that the Generated directory exists.
            // This directory should also always be present; check included for development clarity.
            if (!fs::exists(GetProjectGeneratedPath()))
            {
                fs::create_directory(GetProjectGeneratedPath());
                S_LOG(LogFileSystem, TEXT("Directory %s did not exist so it will be created"), GetProjectGeneratedPath().c_str())
            }

        #endif
    }
}

/** Global unique pointer instance managing the Solitaire Engine. */
SUniquePtr<SSolitaireEngine> GSolitaireEngine = std::make_unique<SSolitaireEngine>();