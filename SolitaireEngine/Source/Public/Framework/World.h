#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SAActor;

/** Base class representing a game world. */
class SOLITAIRE_ENGINE_API SWorld : public SIConsoleRenderable, public std::enable_shared_from_this<SWorld>, public SEnable_CopyFrom<SWorld>
{
	friend class SSolitaireEngine;

public:
	/** Constructors */
	SWorld() = default;
	SWorld(SBool NewAllowUseHome);
	SWorld(const SWorld& Other);

	/** Operators */
	SWorld operator=(const SWorld& Other)
	{
		// Prevent self-assignment which could lead to logical errors or resource leaks
		if (this != &Other)
		{
			// Call CopyFrom and perform a deep copy
			CopyFrom(Other);
		}

		// Return reference to the current object
		return *this;
	}

	/** Get this world as shared ptr */
	template<typename WorldType = SWorld>
	inline SSharedPtr<WorldType> AsShared() { return std::static_pointer_cast<WorldType>(shared_from_this()); }

	/** Get Actors */
	inline const SVector<SSharedPtr<SAActor>>& GetActors() const { return Actors; }

	/** Get AllowUseHome */
	inline SBool GetAllowUseHome() const { return AllowUseHome; }

	/** Spawn an actor. */
	template <typename T, typename... Args>
	SSharedPtr<T> SpawnActor(Args&&... args);

	/** Performs a deep copy of all owned data from 'other' into this object. */
	virtual void CopyFrom(const SWorld& Other) override;

	/** Performs a deep copy of the current object using the copy constructor. */
	virtual SSharedPtr<SWorld> Clone() const override { return SSharedPtr<SWorld>(new SWorld(*this)); }

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
	SVector<SSharedPtr<SAActor>> Actors;

	/** Does the world allow to use HOME key */
	SBool AllowUseHome = true;
};

template <typename T, typename... Args>
SSharedPtr<T> SWorld::SpawnActor(Args&&... args)
{
	static_assert(std::is_base_of_v<SAActor, T>, "T must derive from SAActor");

	// Create a shared_ptr with unique ownership in this container
	SSharedPtr<T> NewActor = std::make_shared<T>(std::forward<Args>(args)...);

	// Store pointer in internal Actors container for lifetime management
	Actors.push_back(NewActor);

	// Return non-owning weak reference to the caller
	return NewActor;
}