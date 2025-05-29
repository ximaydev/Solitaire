#pragma once
#include "Globals.h"
#include "Audio/AudioEngine.h"
#include "Audio/AudioMaster.h"

/** Forward Declarations */
class SAudioEngine;
class SInputSystem;
class SConsoleRenderer;
class SWorld;
class SConsoleInputHandler;

/**
 * SSolitaireEngine is the core engine class responsible for managing and running
 * the Solitaire game. It handles initialization, shutdown, the main loop, rendering,
 * and integrates with subsystems such as audio. Designed to support modular states
 * like menus, gameplay, and settings using a clean architecture in the Windows console.
 */
class SOLITAIRE_ENGINE_API SSolitaireEngine
{
	friend class SConsole;

public:
	/** Initializes all subsystems of the Solitaire Engine. */
	SBool Initialize();

	/** Set Current world */
	void SetCurrentWorld(const SSharedPtr<SWorld> NewWorld);

	/** Get UseConsoleInputHandler */
	inline SBool GetUseConsoleInputHandler() const { return UseConsoleInputHandler; }

	/** Get GetInputSystem */
	inline SInputSystem* GetInputSystem() const { return InputSystem; }

	/** Get GetConsoleInputHandler */
	inline SConsoleInputHandler* GetConsoleInputHandler() const { return ConsoleInputHandler; }

	/** Get GetConsoleRenderer */
	inline SConsoleRenderer* GetConsoleRenderer() const { return ConsoleRenderer; }

	/** Get IsEngineRunning */
	inline SBool IsEngineRunning() const { return IsRunning; }

	/** Get Current World */
	inline SSharedPtr<SWorld> GetCurrentWorld() const { return CurrentWorld; }

	/** Get Audio Engine */
	inline SAudioEngine* GetAudioEngine() const { return AudioEngine.get(); }

	/** Set UseConsoleInputHandler */
	inline void SetUseConsoleInputHandler(SBool NewUseConsoleInputHandler) { UseConsoleInputHandler = NewUseConsoleInputHandler; }

	/** Creates initial map instance */
	template<typename MapType>
	inline void CreateInitialMap(SBool bRunEngine = true);

private:
	/** Shuts down the engine and cleans up resources. */
	void ShutDown();

	/** Renders output to the console. */
	void Render();

	/** Processes input events. */
	void ProcessInput();

	/** Runs the main game loop. */
	void Run();

	/** Holds the persistent callback for the StartTyping key action. */
	SCallback StartTypingCallback;

	/** Pointer to the input system responsible for handling key input. */
	SInputSystem* InputSystem = nullptr;

	/** Pointer to the input system responsible for handling inputs in the console. */
	SConsoleInputHandler* ConsoleInputHandler = nullptr;

	/** Pointer to the console renderer responsible for rendering console. */
	SConsoleRenderer* ConsoleRenderer = nullptr;

	/** Pointer to the audio engine resposible for audio */
	SUniquePtr<SAudioEngine> AudioEngine = nullptr;

	/** Indicates whether the engine is currently running. */
	SBool IsRunning = false;

	/** The world currently being rendered by the engine */
	SSharedPtr<SWorld> CurrentWorld = nullptr;

	/** Flag to determine whether to use this low-level Console Input Handle or the higher-level Input System. */
	SBool UseConsoleInputHandler = false;
};

template<typename MapType>
inline void SSolitaireEngine::CreateInitialMap(SBool bRunEngine)
{
	static_assert(std::is_base_of_v<SWorld, MapType>, "MapType must derive from SWorld");

	// Set current world
	SSharedPtr<MapType> World = std::make_shared<MapType>();
	SetCurrentWorld(World);

	// Initialize map
	World->Initialize();

	// Run Engine
	if(bRunEngine)
		Run();
}