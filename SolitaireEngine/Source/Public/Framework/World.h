#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SAActor;

/** Base class representing a game world. */
class SOLITAIRE_ENGINE_API SWorld : public SIConsoleRenderable, public std::enable_shared_from_this<SWorld>
{
	friend class SSolitaireEngine;

public:
	/** Get this world as shared ptr */
	template<typename WorldType = SWorld>
	inline SSharedPtr<WorldType> AsShared() { return std::static_pointer_cast<WorldType>(shared_from_this()); }

	/** Add the actor to the Actors */
	void RegisterActor(SSharedPtr<SAActor> Actor);
	void RegisterActor(SAActor* Actor);

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

	// Store raw pointer in internal Actors container for lifetime management
	RegisterActor(Actor.get());

	// Return the smart pointer to the caller
	return Actor;
}