#pragma once
#include "Globals.h"

/** Forward Declarations */
class SAudioEngine;
class SInputSystem;
class SConsoleRenderer;

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
	bool Initialize();

	/** Runs the main game loop. */
	void Run();

private:
	/** Shuts down the engine and cleans up resources. */
	void ShutDown();

	/** Renders output to the console. */
	void Render();

	/** Unique pointer to the audio engine responsible for playing sounds. */
	SUniquePtr<SAudioEngine> AudioEngine;

	/** Unique pointer to the input system responsible for handling key input. */
	SUniquePtr<SInputSystem> InputSystem;

	/** Pointer to the console renderer responsible for rendering console. */
	SConsoleRenderer* ConsoleRenderer;

	/** Indicates whether the engine is currently running. */
	SBool IsRunning = false;
};