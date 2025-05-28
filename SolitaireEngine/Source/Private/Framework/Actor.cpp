#include "SolitaireEnginePCH.h"
#include "Framework/Actor.h"

SAActor::SAActor(const SAActor& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

void SAActor::CopyFrom(const SAActor& Other)
{
    // Copy the grid position from the source object
    GridPosition = Other.GridPosition;
}