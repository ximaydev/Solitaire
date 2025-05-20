#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** Represents the tableau area in a card game, managing multiple columns of cards. */
class SATableau final : public SAActor
{
public:
	/** Constructor */
	SATableau(const SGridPositionU32& NewGridPosition, SVector<SSharedPtr<SACard>>&& NewCards);

    /** Renders the tableau to the console */
    void Write() override;

    /** Clears the tableau from the console buffer */
    void ClearBuffer() override;

protected:
    /** Generates the card columns */
    void GenerateColumns(SVector<SSharedPtr<SACard>>&& NewCards);

    /** Number of columns in the tableau */
    SUInt32 ColumnCount = 7;

    /** Array of card columns */
    SArray<SVector<SSharedPtr<SACard>>, 7> Cards;
};