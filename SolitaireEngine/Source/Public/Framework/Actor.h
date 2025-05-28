#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Forward Declarations */
class SWorld;

/** Represents an actor in the game world with a position on the grid. */
class SOLITAIRE_ENGINE_API SAActor : public SIConsoleRenderable, public SEnable_CopyFrom<SAActor>
{
	friend SWorld;
public:
	/** Constructors */
	SAActor() = default;
	SAActor(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld) : SIConsoleRenderable(NewGridPosition), World(NewWorld) {}
	SAActor(const SAActor& Other);

	/** Operators */
	SAActor operator=(const SAActor& Other)
	{
		if (this != &Other)
		{
			// Call CopyFrom and perform a deep copy
			CopyFrom(Other);
		}
		return *this;
	}

	/** Get World */
	template<typename WorldType = SWorld>
	inline SSharedPtr<WorldType> GetWorld() const { return std::static_pointer_cast<WorldType>(World); }

	/** Set World */
	inline void SetWorld(SSharedPtr<SWorld> NewWorld) { World = NewWorld; }

	/** Performs a deep copy of all owned data from 'other' into this object. */
	virtual void CopyFrom(const SAActor& Other) override;

	/** Performs a deep copy of the current object using the copy constructor. */
	virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAActor(*this)); }

protected:
	/** Pointer to the current world */
	SSharedPtr<SWorld> World = nullptr;
};