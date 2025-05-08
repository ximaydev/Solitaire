#include "SolitaireEnginePCH.h"
#include "Core/Asset/AssetLoaderRegistry.h"

SAssetLoaderRegistry* SAssetLoaderRegistry::GetInstance()
{
    // Creates and returns a unique instance of SAssetLoaderRegistry.
    static SUniquePtr<SAssetLoaderRegistry> AssetLoaderRegistry = std::make_unique<SAssetLoaderRegistry>();
    return AssetLoaderRegistry.get();
}
