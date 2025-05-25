#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** Represents the tableau area in Solitaire game, managing multiple columns of cards. */
class SATableau final : public SAActor
{
public:
    /** Number of columns in the tableau */
    static constexpr SUInt32 ColumnCount = 7;

    using TableauArray = SArray<SVector<SSharedPtr<SACard>>, ColumnCount>;

	/** Constructor */
	SATableau(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& NewCards);

    /** Get Cards */
    inline const TableauArray& GetCards() const { return Cards; }

    /** Renders the tableau to the console */
    void Write() override;

    /** Clears the tableau from the console buffer */
    void ClearBuffer() override;

protected:
    /** Generates the card columns */
    void GenerateColumns(SVector<SSharedPtr<SACard>>&& NewCards);

    /** Array of card columns */
    TableauArray Cards;
};