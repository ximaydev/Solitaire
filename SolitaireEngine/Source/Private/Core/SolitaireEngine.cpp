#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include "Config/IniFileManager.h"
#include "Inputs/InputSystem.h"

bool SSolitaireEngine::Initialize()
{
    // Log the begining of the engine initialization
    S_LOG(LogSolitaireEngine, TEXT("Initializing Solitaire Engine..."));

    // Create necessary engine directories
    Core::Paths::CreateDirectories();

    // Loads the config files from the disk
    SIniFileManager::GetInstance()->LoadConfigFilesFromDisk();

    // Create the audio engine
    AudioEngine = std::make_unique<SAudioEngine>();

    // Get the input system
    InputSystem = SInputSystem::GetInstance();

    // Initialize the InputSystem
    if (!InputSystem->Initialize())
    {
        S_LOG_ERROR(LogSolitaireEngine, TEXT("Failed to initialize the input system."));
        exit(1);
    }

    //Get Console Renderer
    ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Log the engine initialization completed
    S_LOG(LogTemp, TEXT("Solitaire Engine initialization completed."));
    
    return true;
}

void SSolitaireEngine::ShutDown()
{
    // Log the beginning of engine shutdown
    S_LOG(LogSolitaireEngine, TEXT("Shutting down the Solitaire Engine..."));

    // Deinitialize the audio engine
    AudioEngine->Shutdown();

    // Log shutdown completion
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine shutdown completed."));

    // Ensure all log messages are flushed before exiting
    SFileLogger::GetInstance()->WaitForLoggingToFinish();
}

void SSolitaireEngine::Render()
{
    // Render current frame to the console
    ConsoleRenderer->Draw();
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
        // Update input system
        InputSystem->UpdateKeyStates();

        // Render the frame
        Render();
    }

    // Log the end of the main loop
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine has stopped."));

    // Shutdown the engine
    ShutDown();
}