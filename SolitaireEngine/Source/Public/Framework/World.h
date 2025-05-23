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
	void AddActor(SAActor* Actor);

	/** Spawn an actor. */
	template <typename PtrType, typename T, typename... Args>
	PtrType SpawnActor(Args&&... args);

protected:
	/** Called once when the world is created */
	virtual SBool Initialize() { return true; };

	/** Called once when the world is destroyed */
	virtual void Shutdown() {};

	/** Renders the Actors at its internal grid position. */
	virtual void Write() override;

	/** Clears the entire console buffer and then draws the border. */
	virtual void ClearBuffer() override;

	/** List of all actors in this world */
	SVector<SAActor*> Actors;
};

template <typename PtrType, typename T, typename... Args>
PtrType SWorld::SpawnActor(Args&&... args)
{
	// Create a new actor instance wrapped in PtrType using raw pointer constructor
	PtrType Actor = PtrType(new T(std::forward<Args>(args)...));

	// Set the actor's world pointer to this world instance
	Actor->World = this;

	// Store raw pointer in internal Actors container for lifetime management
	Actors.push_back(Actor.get());

	// Return the smart pointer to the caller
	return Actor;
}