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
public:
	/** Initializes all subsystems of the Solitaire Engine. */
	bool Initialize(SSharedPtr<SWorld> NewWorld);
	
	/** Runs the main game loop. */
	void Run();

	/** Get UseConsoleInputHandler */
	inline SBool GetUseConsoleInputHandler() const { return UseConsoleInputHandler; }

	/** Set UseConsoleInputHandler */
	inline void SetUseConsoleInputHandler(SBool NewUseConsoleInputHandler) { UseConsoleInputHandler = NewUseConsoleInputHandler; }

	/** Set Current world */
	void SetCurrentWorld(const SSharedPtr<SWorld> NewWorld);

private:
	/** Shuts down the engine and cleans up resources. */
	void ShutDown();

	/** Renders output to the console. */
	void Render();

	/** Processes input events. */
	void ProcessInput();

	/** Pointer to the input system responsible for handling key input. */
	SInputSystem* InputSystem = {};

	/** Pointer to the input system responsible for handling inputs in the console. */
	SConsoleInputHandler* ConsoleInputHandler;

	/** Pointer to the console renderer responsible for rendering console. */
	SConsoleRenderer* ConsoleRenderer = {};

	/** Indicates whether the engine is currently running. */
	SBool IsRunning = {};

	/** The world currently being rendered by the engine */
	SSharedPtr<SWorld> CurrentWorld;

	/** Flag to determine whether to use this low-level Console Input Handle or the higher-level Input System. */
	SBool UseConsoleInputHandler = false;
};