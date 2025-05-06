#pragma once
#include "Globals.h"

class SAsset;

/** Registry for storing and accessing loaded assets. */
class SOLITAIRE_ENGINE_API SAssetRegistry
{
public:
    /** Get instance of this class */
    static SAssetRegistry* GetInstance();

    /** Get asset of type T from registry using its path. */
    template<typename AssetType>
    inline AssetType* GetAsset(const SWString& AssetPath) { return static_cast<AssetType*>(RegisteredAssets.find(AssetPath)->second.get()); }

    /** Register/load a new asset by its path. */
    void RegisterAsset(const SWString& AssetPath);

    /** Unregister (remove) asset from registry and unload from memory by its path. */
    void UnRegisterAsset(const SWString& AssetPath);

    /** Check if an asset exists in the registry. */
    bool HasAsset(const SWString& AssetPath);

    /** Clear all registered assets. */
    void Clear();

protected:
    /** Map storing asset paths and their unique asset pointers. */
    SUnorderedMap<SWString, SSharedPtr<SAsset>> RegisteredAssets;
};
