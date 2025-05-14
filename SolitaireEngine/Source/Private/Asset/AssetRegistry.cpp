#include "SolitaireEnginePCH.h"
#include "Asset/AssetRegistry.h"
#include "Asset/AssetLoader.h"
#include "Asset/AssetLoaderRegistry.h"
#include "Asset/Asset.h"
#include "Config/IniFile.h"
#include "Config/IniFileManager.h"

SAssetRegistry::SAssetRegistry()
{
    LoadAssetsFromAssetList();
}

SAssetRegistry* SAssetRegistry::GetInstance()
{
	// Return the singleton instance of the asset registry
	static SUniquePtr<SAssetRegistry> AssetRegistry = std::make_unique<SAssetRegistry>();
	return AssetRegistry.get();
}

void SAssetRegistry::RegisterAsset(const SWString& AssetPath)
{
	// Extract the file extension from the path
	SWString Extension;
	SStringLibrary::GetFileExtension(AssetPath, Extension);

	// Get the appropriate loader for the file extension
	SIAssetLoader* AssetLoader = SAssetLoaderRegistry::GetInstance()->GetLoader(Extension);

	// Create and load the asset using the loader
	SSharedPtr<SIAsset> Asset = AssetLoader->CreateAsset();
	AssetLoader->Load(AssetPath, Asset.get());

	// Store the loaded asset in the registry
	RegisteredAssets.emplace(AssetPath, Asset);
}

void SAssetRegistry::UnRegisterAsset(const SWString& AssetPath)
{
	// Remove the asset from the registry
	RegisteredAssets.erase(AssetPath);
}

bool SAssetRegistry::HasAsset(const SWString& AssetPath)
{
	// Check if the asset exists in the registry
	return RegisteredAssets.contains(AssetPath);
}

void SAssetRegistry::Clear()
{
	// Remove all assets from the registry
	RegisteredAssets.clear();
	// TODO: Check if the map's capacity was reduced
}

void SAssetRegistry::LoadAssetsFromAssetList()
{
    // Get Engine Config file
    SSharedPtr<SIniFile> EngineConfig = SIniFileManager::GetInstance()->GetConfigFile(DefaultEngineConfig);

    // Get the name of the AssetList.txt file from the engine config
    const SWString& AssetListName = EngineConfig->GetValueFromKey<SWString>(SWString(TEXT("[Paths]")), SWString(TEXT("AssetListName")));

    SWString Line;

    // Open AssetList.txt for reading
    SWFileReader AssetListReader(Core::Paths::GetProjectGeneratedPath() + AssetListName, std::ios::in);
    if (!AssetListReader.is_open())
    {
        // Log an error if the AssetList file could not be opened
        S_LOG_ERROR(LogFileSystem, TEXT("Failed to open %s file"), AssetListName.c_str());
        return;
    }

    // Log the successful opening of AssetList.txt
    S_LOG(LogFileSystem, TEXT("Successfully opened %s file"), AssetListName.c_str());

    // Read each line in AssetList.txt
    while (std::getline(AssetListReader, Line))
    {
        // Register the asset found in the line
        S_LOG(LogFileSystem, TEXT("Registering asset from path: %s"), (Core::Paths::GetProjectContentPath() + Line).c_str());
        RegisterAsset(Core::Paths::GetProjectContentPath() + Line);
    }

    // Log the completion of asset loading
    S_LOG(LogFileSystem, TEXT("Finished loading assets from %s"), AssetListName.c_str());
}
