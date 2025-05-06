#pragma once
#include "Core/CoreTypes.h"

#ifdef SOLITAIRE_ENGINE_BUILD
	#define SOLITAIRE_ENGINE_API __declspec(dllexport)
#else
	#define SOLITAIRE_ENGINE_API __declspec(dllimport)
#endif

/** Alias for the std::filesystem namespace for cleaner code */
namespace fs = std::filesystem;

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

	/** Returns absolute path to project root directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectRootPath() { return GProjectRootPath; }

	/** Returns path to Content directory (relative to root). */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectContentPath() { return GProjectContentPath; }

	/** Returns path to Config directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectConfigPath() { return GProjectConfigPath; }

	/** Returns path to Saved directory. */
	SOLITAIRE_ENGINE_API inline const SWString& GetProjectSavedPath() { return GProjectSavedPath; }

	/** Creates necessary project directories if they do not already exist. */
	//TODO: Delete SOLITAIRE_ENGINE_API, we don't want to export it
	SOLITAIRE_ENGINE_API void CreateDirectories();
}