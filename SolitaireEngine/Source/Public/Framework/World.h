#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SAActor;

/** Base class representing a game world. */
class SOLITAIRE_ENGINE_API SWorld : public SIConsoleRenderable
{
	friend class SSolitaireEngine;

public:
	/** Add the actor to the Actors */
	inline void AddActor(SAActor* Actor) { Actors.push_back(Actor); }

protected:
	/** Called once when the world is created */
	virtual void Initialize() {};

	/** Called once when the world is destroyed */
	virtual void Shutdown() {};

	/** Renders the Actors at its internal grid position. */
	virtual void Write() override;

	/** Clears the entire console buffer and then draws the border. */
	virtual void ClearBuffer() override;

	/** List of all actors in this world */
	SVector<SAActor*> Actors;
};