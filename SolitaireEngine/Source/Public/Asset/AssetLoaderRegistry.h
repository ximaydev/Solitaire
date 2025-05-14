#pragma once
#include "Globals.h"
#include "Asset/AssetLoader.h"

/** Registry for managing asset loaders by file extension. */
class SOLITAIRE_ENGINE_API SAssetLoaderRegistry
{
public:
    /** Get instance of this class */
    static SAssetLoaderRegistry* GetInstance();

    /** Registers a loader for a specific file extension. */
    inline void RegisterLoader(const SWString& Extension, SSharedPtr<SIAssetLoader> Loader) { RegisteredLoaders.try_emplace(Extension, Loader); }

    /** Get the loader for a specific file extension. */
    inline SIAssetLoader* GetLoader(SWString& Extension) const { return RegisteredLoaders.find(Extension)->second.get(); }

protected:
    /** Map storing file extensions and their corresponding asset loaders. */
    SUnorderedMap<SWString, SSharedPtr<SIAssetLoader>> RegisteredLoaders;
};