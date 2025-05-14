#pragma once
#include "Globals.h"
#include "Asset/Asset.h"

#define REGISTER_ASSET_LOADER(CLASS_NAME)                           \
static struct F##CLASS_NAME##AutoRegister                           \
{                                                                   \
    F##CLASS_NAME##AutoRegister()                                   \
    {                                                               \
        auto Loader = std::make_shared<CLASS_NAME>();               \
        SAssetLoaderRegistry::GetInstance()->RegisterLoader(        \
            Loader->GetSupportedExtension(), Loader);               \
    }                                                               \
} CLASS_NAME##AutoRegisterInstance;


/** Base class for loading and unloading assets (abstract). */
class SOLITAIRE_ENGINE_API SIAssetLoader
{
public:
    /** Loads an asset from the specified file path. */
    virtual void Load(const SWString& FilePath, SIAsset* OutFile) = 0;

    /** Unloads the asset from memory. */
    virtual void UnLoad(SIAsset* OutFile) = 0;

    /** Creates a new asset of a specific type. */
    virtual SSharedPtr<SIAsset> CreateAsset() = 0;

    /** Returns the file extension supported by this loader (e.g., ".wav", ".ogg"). */
    virtual SWString GetSupportedExtension() = 0;
};
