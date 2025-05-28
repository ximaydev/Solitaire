#pragma once
#include "Core/CoreTypes.h"

/** Forward Declarations */
class SSolitaireEngine;
class SConsole;

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

/** Represents an invalid or uninitialized index. Typically used to indicate that a valid index was not found. */
SOLITAIRE_ENGINE_API const extern SUInt32 INDEX_NONE;

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

/** Global unique pointer instance managing the Console. */
extern SOLITAIRE_ENGINE_API SUniquePtr<SConsole> GConsole;

/** Global unique pointer instance managing the Solitaire Engine. */
extern SOLITAIRE_ENGINE_API SUniquePtr<SSolitaireEngine> GSolitaireEngine;

/** Attempts to promote a weak pointer to a shared pointer. */
template<typename T>
inline SSharedPtr<T> WeakToSharedPtr(const SWeakPtr<T>& WeakPtr)
{
	return !WeakPtr.expired() ? WeakPtr.lock() : nullptr;
}

/** Template base class that enables the CopyFrom functionality. */
template<typename T>
class SOLITAIRE_ENGINE_API SEnable_CopyFrom
{
protected:
	/** Pure virtual method to copy data from another object of type T. */
	virtual void CopyFrom(const T& Other) = 0;

	// Virtual clone method returns pointer to the copy
	virtual SSharedPtr<T> Clone() const { return nullptr; };
};