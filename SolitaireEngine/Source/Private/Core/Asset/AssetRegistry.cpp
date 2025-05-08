#include "SolitaireEnginePCH.h"
#include "Core/Asset/AssetRegistry.h"

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
	SAssetLoader* AssetLoader = SAssetLoaderRegistry::GetInstance()->GetLoader(Extension);

	// Create and load the asset using the loader
	SSharedPtr<SAsset> Asset = AssetLoader->CreateAsset();
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