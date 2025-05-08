#pragma once
#include "Globals.h"

/** Base class for loading and unloading assets (abstract). */
class SOLITAIRE_ENGINE_API SAssetLoader
{
public:
    /** Loads an asset from the specified file path. */
    virtual void Load(const SWString& FilePath, void* OutFile) = 0;

    /** Unloads the asset from memory. */
    virtual void UnLoad(void* OutFile) = 0;

    /** Creates a new asset of a specific type. */
    virtual SSharedPtr<SAsset> CreateAsset() = 0;
};
