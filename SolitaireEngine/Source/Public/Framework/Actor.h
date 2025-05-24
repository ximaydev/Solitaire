#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SWorld;

/** Represents an actor in the game world with a position on the grid. */
class SOLITAIRE_ENGINE_API SAActor : public SIConsoleRenderable
{
	friend SWorld;
public:
	/** Constructors */
	SAActor(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SIConsoleRenderable(NewGridPosition), World(NewWorld) {}

	/** Get World */
	template<typename WorldType = SWorld>
	inline SSharedPtr<WorldType> GetWorld() const { return std::static_pointer_cast<WorldType>(World); }

	/** Set World */
	inline void SetWorld(SSharedPtr<SWorld> NewWorld) { World = NewWorld; }

protected:
	/** Pointer to the current world */
	SSharedPtr<SWorld> World = nullptr;
};