#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Represents an actor in the game world with a position on the grid. */
class SOLITAIRE_ENGINE_API SAActor : public SIConsoleRenderable
{
public:
	/** Constructors */
	SAActor(const SGridPositionU32& NewGridPosition) : SIConsoleRenderable(NewGridPosition) {}
};