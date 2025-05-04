#pragma once
#include "Globals.h"

class SINIFile;

class SOLITAIRE_ENGINE_API SINIFileManager
{
public:
	/** Returns a pointer to the global INI File Manager instance (singleton-like access) */
	static SINIFileManager* GetInstance();

	/** Retrieves the names of all configuration files located in the /Config directory. */
	void GetConfigFileNames(SVector<SPath>& OutConfigFileNames) const;

	/** This function loads config files */
	void LoadConfigFilesFromDisk();

	/** Get specified file */
	SSharedPtr<SINIFile> GetConfigFile(const SWString& FileName);

protected:
	/** This variable stores all configs loaded form the directory /Config */
	SUnorderedMap<SWString, SSharedPtr<SINIFile>> ConfigFiles;
};