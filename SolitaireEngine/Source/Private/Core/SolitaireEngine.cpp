#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"
#include "Inputs/InputSystem.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/World.h"
#include "Asset/AssetRegistry.h"
#include "Audio/WAVLoader.h"

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
            // Switch to typing mode � console now handles key input
            SetUseConsoleInputHandler(true && CurrentWorld->GetAllowUseHome());

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

    // Play music
    SWavFile* WAVFile = SAssetRegistry::GetInstance()->GetAsset<SWavFile>(Core::Paths::GetProjectContentPath() + TEXT("retro-game-arcade-236133.wav"));

    // Prepare the audio buffer for playback
    XAUDIO2_BUFFER AudioBuffer = {};  // Initialize all fields to zero

    // Set the size of the audio data in bytes
    AudioBuffer.AudioBytes = static_cast<SUInt32>(WAVFile->GetData().size());

    // Provide a pointer to the raw PCM audio data
    AudioBuffer.pAudioData = WAVFile->GetData().data();

    // Loop the audio infinitely
    AudioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    // Play the sound on the audio engine with the specified volume group "MusicVolume"
    AudioEngine->PlaySound(WAVFile, TEXT("Retro"), AudioBuffer, TEXT("MusicVolume"));

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

    // Get target frame time from config
    const SDouble TargetFrameTime = 1.0 / SIniFileManager::GetInstance()->GetConfigFile(DefaultEngineConfig)->GetValueFromKey<SDouble>(TEXT("[Rendering]"), TEXT("FPS"));

    // Main game loop
    while (IsRunning)
    {
        // Get time
        auto FrameStart = std::chrono::high_resolution_clock::now();

        // Process Input
        ProcessInput();

        // Render the frame
        Render();

        auto FrameEnd = std::chrono::high_resolution_clock::now();
        SDouble Elapsed = std::chrono::duration<SDouble>(FrameEnd - FrameStart).count();

        // Sleep for the remaining frame time to cap FPS
        SDouble TimeToWait = TargetFrameTime - Elapsed;
        if (TimeToWait > 0.0)
        {
            std::this_thread::sleep_for(std::chrono::duration<SDouble>(TimeToWait));
        }

        #if DEBUG
            // Calculate actual delta time after sleep
            auto AfterSleep = std::chrono::high_resolution_clock::now();
            SDouble Delta = std::chrono::duration<SDouble>(AfterSleep - FrameStart).count();
            SDouble FPS = 1.0 / Delta;

            // Display current FPS in the console
            SWString FPSText = TEXT("FPS: ") + std::to_wstring(static_cast<SUInt32>(FPS));
            SConsoleRenderer::GetInstance()->Write(SGridPositionU32(120, 15), FPSText, 10, true, FG_BLUE | BG_GREEN);
        #endif 
    }
}