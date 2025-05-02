#include "SolitaireEnginePCH.h"
#include "Globals.h"
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace Core::Paths
{
    /** Global variable to store the project root path */
    SWString GProjectRootPath;

    void IntializeProjectRootPath()
    {
        SWideChar ExePath[MAX_PATH];
        SWideChar FinalPath[MAX_PATH];

        // Get the full path of the current executable
        GetModuleFileNameW(NULL, ExePath, MAX_PATH);

        // Go up 3 directories from the executable location using a relative path
        PathCombineW(FinalPath, ExePath, TEXT("..\\..\\..\\"));

        // Convert back to string format and store in the global variable
        GProjectRootPath = SWString(FinalPath);
    }
}