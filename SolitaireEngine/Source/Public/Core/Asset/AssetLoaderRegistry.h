#pragma once
#include "Globals.h"

/** Registry for managing asset loaders by file extension. */
class SOLITAIRE_ENGINE_API SAssetLoaderRegistry
{
public:
    /** Get instance of this class */
    static SAssetLoaderRegistry* GetInstance();

    /** Registers a loader for a specific file extension. */
    inline void RegisterLoader(const SWString& Extension, SSharedPtr<SAssetLoader> Loader) { RegisteredLoaders.try_emplace(Extension, Loader); }

    /** Get the loader for a specific file extension. */
    inline SAssetLoader* GetLoader(SWString& Extension) const { return RegisteredLoaders.find(Extension)->second.get(); }

protected:
    /** Map storing file extensions and their corresponding asset loaders. */
    SUnorderedMap<SWString, SSharedPtr<SAssetLoader>> RegisteredLoaders;
};