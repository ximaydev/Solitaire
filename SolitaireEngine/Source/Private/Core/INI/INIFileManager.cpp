#include "SolitaireEnginePCH.h"
#include "Core/INI/INIFileManager.h"

SIniFileManager* SIniFileManager::GetInstance()
{
	// Create the singleton instance of SIniFileManager if it doesn't already exist.
	static SUniquePtr<SIniFileManager> INIFileManager = std::make_unique<SIniFileManager>();
	return INIFileManager.get();
}

void SIniFileManager::GetConfigFileNames(SVector<SPath>& OutConfigFileNames) const
{
	// Counter to track the number of configuration files found
	SUInt32 Counter = 0;

	// First, count the number of regular files in the config directory (recursively)
	for (const auto& Entry : fs::directory_iterator(Core::Paths::GetProjectConfigPath()))
	{
		// If the entry is a regular file (not a directory), increment the counter
		if (fs::is_regular_file(Entry))
			Counter++;
	}

	// Reserve memory in the output vector to avoid reallocations during the file name collection
	OutConfigFileNames.reserve(Counter);

	// Iterate through all files in the configuration path (non-recursively)
	for (const auto& Path : fs::directory_iterator(Core::Paths::GetProjectConfigPath()))
	{
		// Check if the current path is a regular file (not a directory or symlink)
		if (Path.is_regular_file())
		{
			// Get File Name
			const SWString& FileName = Path.path().filename().wstring();

			// Get extension and check if the file is .ini
			SWString Extension;
			SStringLibrary::GetFileExtension(FileName, Extension);

			if (Extension == TEXT(".ini"))
			{
				// Add the filename (without full path) to the output vector
				OutConfigFileNames.push_back(FileName);

				// Log the found file path
				S_LOG(LogConfig, TEXT("Found config file: %s"), FileName.c_str());
			}
			else
			{
				// Log ignored file with its extension
				S_LOG_WARNING(LogConfig, TEXT("Ignored file (not .ini): %s (Extension: %s)"), FileName.c_str(), Extension.c_str());
			}
		}
	}

	// Shrink the vector to fit the number of elements it contains to optimize memory usage
	OutConfigFileNames.shrink_to_fit();
}

void SIniFileManager::LoadConfigFilesFromDisk()
{
	// Starting to load configuration files from disk into memory
	S_LOG(LogConfig, TEXT("Starting to load config files from disk into memory"));

	// Get config file names
	SVector<SPath> ConfigFileNames;
	GetConfigFileNames(ConfigFileNames);

	// Loop through each config file and load it into memory
	for (SWString ConfigFileName : ConfigFileNames)
	{
		// Create a shared pointer for the new INIFile
		SSharedPtr<SIniFile> INIFile = std::make_shared<SIniFile>();

		// Load the configuration file into the INIFile
		SIniReader::LoadFromFile(ConfigFileName, INIFile->ConfigFileMap);

		// Add the loaded INIFile to the map of config files
		ConfigFiles.emplace(ConfigFileName, INIFile);
	}

	// Finished loading configuration files from disk into memory
	S_LOG(LogConfig, TEXT("Finished loading config files from disk into memory"));
}

SSharedPtr<SIniFile> SIniFileManager::GetConfigFile(const SWString& FileName)
{
	return ConfigFiles.at(FileName);
}

