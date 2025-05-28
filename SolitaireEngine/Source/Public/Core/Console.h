#pragma once
#include "Globals.h"

/** Console class for the console apps. */
class SOLITAIRE_ENGINE_API SConsole
{
public:
	/** This function sets up the console. Must be called once before using any console-related features. */
	virtual SBool Initialize();

	/** Cleans up any allocated resources or state related to the console. Should be called before application exits or when console is no longer needed. */
	virtual void ShutDown();

	/** Handles system console control signals (e.g. closing console window). */
	static BOOL WINAPI ConsoleCtrlHandler(DWORD Signal);
};