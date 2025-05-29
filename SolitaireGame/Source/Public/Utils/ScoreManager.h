#pragma once
#include "Framework/Actor.h"

/** Manages player scores in the game */
class SAScoreManager : public SAActor
{
public:
	/** Maximum number of score entries */
	static constexpr SUInt8 MaxScoreEntry = 10;

	/** Constructor */
	SAScoreManager(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld);
};