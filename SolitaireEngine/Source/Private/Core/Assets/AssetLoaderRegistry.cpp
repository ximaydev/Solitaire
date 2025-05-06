#include "SolitaireEnginePCH.h"
#include "Core/Assets/AssetLoaderRegistry.h"

SAssetLoaderRegistry* SAssetLoaderRegistry::GetInstance()
{
    static SUniquePtr<SAssetLoaderRegistry> AssetLoaderRegistry = std::make_unique<SAssetLoaderRegistry>();
    return AssetLoaderRegistry.get();
}
