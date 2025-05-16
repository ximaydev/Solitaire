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
	SAActor(const SGridPositionU32& NewGridPosition) : SIConsoleRenderable(NewGridPosition) {}

	/** Get World */
	inline SWorld* GetWorld() const { return World; }

protected:
	/** Pointer to the current world */
	SWorld* World;
};