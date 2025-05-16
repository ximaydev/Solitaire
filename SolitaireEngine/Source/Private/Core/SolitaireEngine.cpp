#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include "Config/IniFileManager.h"
#include "Inputs/InputSystem.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/World.h"

bool SSolitaireEngine::Initialize(const SSharedPtr<SWorld>& NewWorld)
{
    // Log the begining of the engine initialization
    S_LOG(LogSolitaireEngine, TEXT("Initializing Solitaire Engine..."));

    // Create necessary engine directories
    Core::Paths::CreateDirectories();

    // Loads the config files from the disk
    SIniFileManager::GetInstance()->LoadConfigFilesFromDisk();
    
    // Get the input system
    InputSystem = SInputSystem::GetInstance();

    // Initialize the InputSystem
    if (!InputSystem->Initialize())
    {
        S_LOG_ERROR(LogSolitaireEngine, TEXT("Failed to initialize the input system."));
        exit(1);
    }

    // Get the Console Input Handler
    ConsoleInputHandler = SConsoleInputHandler::GetInstance();

    //Get Console Renderer
    ConsoleRenderer = SConsoleRenderer::GetInstance();

    //Set current world
    CurrentWorld = NewWorld;

    // Log the engine initialization completed
    S_LOG(LogTemp, TEXT("Solitaire Engine initialization completed."));
    
    return true;
}

void SSolitaireEngine::ShutDown()
{
    // Log the beginning of engine shutdown
    S_LOG(LogSolitaireEngine, TEXT("Shutting down the Solitaire Engine..."));

    // Log shutdown completion
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine shutdown completed."));

    // Ensure all log messages are flushed before exiting
    SFileLogger::GetInstance()->WaitForLoggingToFinish();
}

void SSolitaireEngine::Render()
{
    // Check if the current world isn't nullptr 
    if (CurrentWorld)
    {
        // Write the current world to the console buffer
        CurrentWorld->Write();
    }

    // Check if the UseConsoleInputHandler is set to true
    if (UseConsoleInputHandler)
    {
        ConsoleInputHandler->Write();
    }

    // Render current frame to the console
    ConsoleRenderer->Draw();
}

void SSolitaireEngine::ProcessInput()
{
    // Check if the UseConsoleInputHandler is set to true
    if (UseConsoleInputHandler)
    {
        // Process character input and update the current text line
        ConsoleInputHandler->UpdateLine();
    }
    else
    {
        // Update key states
        InputSystem->UpdateKeyStates();
    }
}

void SSolitaireEngine::Run()
{
    // Log the start of the main loop
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine has started running."));

    // Set the engine state to running
    IsRunning = true;

    // Start the main game loop
    while (IsRunning)
    {
        // Process Input
        ProcessInput();

        // Render the frame
        Render();
    }

    // Log the end of the main loop
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine has stopped."));

    // Shutdown the engine
    ShutDown();
}