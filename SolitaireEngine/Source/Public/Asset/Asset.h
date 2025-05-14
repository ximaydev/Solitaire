#pragma once
#include "Globals.h"

/** Base class for all assets (abstract). */
class SOLITAIRE_ENGINE_API SIAsset
{
protected:
	/** Clear the asset */
	virtual void ClearAsset() = 0;
};