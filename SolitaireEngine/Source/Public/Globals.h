#pragma once
#include "Core/CoreTypes.h"

#ifdef SOLITAIRE_ENGINE_BUILD
	#define SOLITAIRE_ENGINE_API __declspec(dllexport)
#else
	#define SOLITAIRE_ENGINE_API __declspec(dllimport)
#endif

// Disable assert() in release builds by defining NDEBUG
#ifdef RELEASE
	#define NDEBUG
#endif

// Defines the name of the engine configuration file, located in the Config folder.
// Must always maintain a consistent name.
#define DefaultEngineConfig TEXT("DefaultEngine.ini")

// Defines the name of the game configuration file, located in the Config folder.
// Must always maintain a consistent name.
#define DefaultGameConfig   TEXT("DefaultGame.ini")

// Defines the name of the audio configuration file, located in the Config folder.
// Must always maintain a consistent name.
#define DefaultAudioConfig  TEXT("DefaultAudio.ini")

namespace Core::Paths
{
	/** Global variable to store the project root path */
	const extern SWString GProjectRootPath;

	/** Global variable to store the content directory path */
	const extern SWString GProjectContentPath;

	/** Global variable to store the config directory path */
	const extern SWString GProjectConfigPath;

	/** Global variable to store the saved directory path */
	const extern SWString GProjectSavedPath;

	/** Global variable to store the generated directory path */
	const extern SWString GProjectGeneratedPath;

	/** Returns absolute path to project root directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectRootPath() { return GProjectRootPath; }

	/** Returns path to Content directory (relative to root). */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectContentPath() { return GProjectContentPath; }

	/** Returns path to Config directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectConfigPath() { return GProjectConfigPath; }

	/** Returns path to Saved directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectSavedPath() { return GProjectSavedPath; }

	/** Returns path to Generated directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectGeneratedPath() { return GProjectGeneratedPath; }

	/** Creates necessary project directories if they do not already exist. */
	void CreateDirectories();
}