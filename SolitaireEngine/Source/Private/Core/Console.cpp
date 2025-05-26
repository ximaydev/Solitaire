#include "SolitaireEnginePCH.h"
#include "Core/Console.h"

SBool SConsole::Initialize(SSharedPtr<SWorld> NewWorld)
{
	// Bind ConsoleCtrlHandler
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    // Initialize the Solitaire Engine
    return GSolitaireEngine->Initialize(NewWorld);
}

void SConsole::ShutDown()
{
    // Stop the engine
    GSolitaireEngine->ShutDown();

    // Log shutdown
    S_LOG(LogConsole, TEXT("Console has been shut down"));
}

BOOL WINAPI SConsole::ConsoleCtrlHandler(DWORD Signal)
{
    // Check if the received signal is a console close event
    if (Signal == CTRL_CLOSE_EVENT)
    {
        // Call ShutDown
        GConsole->ShutDown();

        // Ensure the logger finishes writing any remaining log messages
        SFileLogger::GetInstance()->WaitForLoggingToFinish();

        // Inform the system that the signal was handled.
        return TRUE;
    }

    // For all other control signals, we don't handle them here.
    return FALSE;
}