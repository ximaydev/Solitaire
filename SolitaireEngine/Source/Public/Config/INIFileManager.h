#pragma once
#include "Globals.h"

/** // SIniFileManager class handles loading, accessing, and managing INI configuration files from the /Config directory, providing singleton access to the loaded files. */
class SOLITAIRE_ENGINE_API SIniFileManager
{
public:
	/** Returns a pointer to the global INI File Manager instance (singleton-like access) */
	static SIniFileManager* GetInstance();

	/** Retrieves the names of all configuration files located in the /Config directory. */
	void GetConfigFileNames(SVector<SPath>& OutConfigFileNames) const;

	/** This function loads config files */
	void LoadConfigFilesFromDisk();

	/** Get specified file */
	SSharedPtr<SIniFile> GetConfigFile(const SWString& FileName);

protected:
	/** This variable stores all configs loaded form the directory /Config */
	SUnorderedMap<SWString, SSharedPtr<SIniFile>> ConfigFiles;
};