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
	extern SWString GProjectRootPath;

	/** Initializes the global project root path by determining the executable's location */
	SOLITAIRE_ENGINE_API void IntializeProjectRootPath();
}