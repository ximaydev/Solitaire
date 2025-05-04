#include "SolitaireEnginePCH.h"
#include "Core/INI/INIFileManager.h"

SINIFileManager* SINIFileManager::GetInstance()
{
	static SUniquePtr<SINIFileManager> INIFileManager = std::make_unique<SINIFileManager>();
	return INIFileManager.get();
}

void SINIFileManager::GetConfigFileNames(SVector<SPath>& OutConfigFileNames) const
{
	//Reserve memory
	OutConfigFileNames.reserve(5);

	// Iterate through all files and directories in the configuration path
	for (const auto& Path : fs::directory_iterator(Core::Paths::GProjectConfigPath))
	{
		// Check if the current path is a regular file (not a directory or symlink)
		if (Path.is_regular_file())
		{
			// Add the full file path (as wide string) to the output vector
			OutConfigFileNames.push_back(Path.path().filename());

			// Log the found file path for debugging or informational purposes
			S_LOG(LogConfig, TEXT("We found %s "), Path.path().wstring().c_str());
		}
	}

	//Shrink
	OutConfigFileNames.shrink_to_fit();
}

void SINIFileManager::LoadConfigFilesFromDisk()
{
	// Log: Starting to load configuration files from disk into memory
	S_LOG(LogConfig, TEXT("Starting to load config files from disk into memory"));

	// Get config file names
	SVector<SPath> ConfigFileNames;
	GetConfigFileNames(ConfigFileNames);

	// Loop through each config file and load it into memory
	for (SWString ConfigFileName : ConfigFileNames)
	{
		// Create a shared pointer for the new INIFile
		SSharedPtr<SINIFile> INIFile = std::make_shared<SINIFile>();

		// Load the configuration file into the INIFile
		SINIFileReader::LoadFromFile(ConfigFileName, INIFile->ConfigFileMap);

		// Add the loaded INIFile to the map of config files
		ConfigFiles.emplace(ConfigFileName, INIFile);
	}

	// Log: Finished loading configuration files from disk into memory
	S_LOG(LogConfig, TEXT("Finished loading config files from disk into memory"));
}

SSharedPtr<SINIFile> SINIFileManager::GetConfigFile(const SWString& FileName)
{
	return ConfigFiles.at(FileName + TEXT(".ini"));
}

