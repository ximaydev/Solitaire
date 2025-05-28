#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include "Config/IniFileManager.h"
#include "Inputs/InputSystem.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/World.h"

SBool SSolitaireEngine::Initialize()
{
    // Create necessary engine directories
    Core::Paths::CreateDirectories();

    // Log the begining of the engine initialization
    S_LOG(LogSolitaireEngine, TEXT("Initializing Solitaire Engine..."));

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

    // Bind unique action to the Input System which allows us to come back to typing mode
    StartTypingCallback = [this]()
        {
            // Switch to typing mode — console now handles key input
            SetUseConsoleInputHandler(true);

            // Clear any leftover events in the console buffer
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        };

    InputSystem->AddCallback(EKeyAction::StartTyping, StartTypingCallback);

    //Get Console Renderer
    ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Create Audio Engine
    AudioEngine = std::make_unique<SAudioEngine>();

    // Log the engine initialization completed
    S_LOG(LogTemp, TEXT("Solitaire Engine initialization completed."));

    return true;
}

void SSolitaireEngine::SetCurrentWorld(SSharedPtr<SWorld> NewWorld)
{
    // If we are not in the middle of a copy operation (i.e. both CurrentWorld and NewWorld are valid but different),
    // we can safely assign the world pointer to all actors.
    // During a deep copy of the world, assigning the world inside actor constructors is unsafe,
    // so we defer it until this point.
    SBool bSetWorld = CurrentWorld != nullptr && NewWorld != nullptr;

    //Set current world
    CurrentWorld = NewWorld;

    // Check if we can assing CurrentWorld
    if (bSetWorld)
    {
        // Iterate throught the actors
        for (auto& Actor : CurrentWorld->Actors)
        {
            // Set Current World
            Actor->SetWorld(CurrentWorld);
        }
    }

    // Rewrite the whole console to see the changes
    SConsoleRenderer::GetInstance()->ClearBuffer();

    // Log
    S_LOG(LogSolitaireEngine, TEXT("New world has been set"));
}

void SSolitaireEngine::ShutDown()
{
    // Log the beginning of engine shutdown
    S_LOG(LogSolitaireEngine, TEXT("Shutting down the Solitaire Engine..."));

    // Set IsRunning to false
    IsRunning = false;

    // Log shutdown completion
    S_LOG(LogSolitaireEngine, TEXT("Solitaire Engine shutdown completed."));
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
}