#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include "Config/IniFileManager.h"
#include "Inputs/InputSystem.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/World.h"

bool SSolitaireEngine::Initialize(SSharedPtr<SWorld> NewWorld)
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
        assert(false && "Failed to initialize the input system.");
    }

    // Get the Console Input Handler
    ConsoleInputHandler = SConsoleInputHandler::GetInstance();

    //Get Console Renderer
    ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Create Audio Engine
    AudioEngine = std::make_unique<SAudioEngine>();

    // Set current world
    SetCurrentWorld(NewWorld);

    // Log the engine initialization completed
    S_LOG(LogTemp, TEXT("Solitaire Engine initialization completed."));
    
    return true;
}

void SSolitaireEngine::SetCurrentWorld(SSharedPtr<SWorld> NewWorld)
{
    //Set current world
    CurrentWorld = NewWorld;

    // Initialize world
    CurrentWorld->Initialize();
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

    // Outside loop
    auto LastFrameTime = std::chrono::high_resolution_clock::now();
    SDouble FPS = 0.0;

    // Inside main game loop
    while (IsRunning)
    {
        // Capture current time
        auto Now = std::chrono::high_resolution_clock::now();

        // Calculate frame time in seconds (nanosecond precision)
        auto Delta = std::chrono::duration<SDouble>(Now - LastFrameTime).count();
        LastFrameTime = Now;

        // Avoid division by zero
        if (Delta > 0.0)
        {
            FPS = 1.0 / Delta;
        }

        // Display FPS
        SWString FPSText = L"FPS: " + std::to_wstring(static_cast<int>(FPS));
        SConsoleRenderer::GetInstance()->Write(SGridPositionU32(120, 15), FPSText, 10, true, FG_BLUE | BG_GREEN);
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