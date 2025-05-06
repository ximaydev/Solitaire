#pragma once
#include "Globals.h"

/** Forward Declaratoin */
class SWAVFile;

class SOLITAIRE_ENGINE_API SAssetLoader
{
public:
	virtual void Load(const SWString& FilePath, void* OutFile) = 0;
	virtual void UnLoad(void* OutFile) = 0;
	virtual SSharedPtr<SAsset> CreateAsset() = 0;
};