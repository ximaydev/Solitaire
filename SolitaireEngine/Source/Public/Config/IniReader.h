#pragma once
#include "Globals.h"

/**
 * SIniReader is a static INI configuration file reader that parses files with [Section] headers and key=value pairs,
 * storing them in a nested map structure: Section -> (Key -> Value). It allows reading and managing configuration data
*/
class SOLITAIRE_ENGINE_API SIniReader
{
public:
	/** Loads and parses the INI file from disk into memory. */
	static void LoadFromFile(const SWString& FileName, SIniConfigMap& ConfigFileMap);
};