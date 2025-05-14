#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SAActor;

/** Base class representing a game world. */
class SOLITAIRE_ENGINE_API SWorld : public SIConsoleRenderable
{
	friend class SSolitaireEngine;

protected:
	/** Called once when the world is created */
	virtual void Initialize() = 0;

	/** Called once when the world is destroyed */
	virtual void Shutdown() = 0;

	/** Renders the Actors at its internal grid position. */
	virtual void Write() override;

	/** Clears the entire console buffer and then draws the border. */
	virtual void ClearBuffer() override;

	/** List of all actors in this world */
	SVector<SAActor*> Actors;
};