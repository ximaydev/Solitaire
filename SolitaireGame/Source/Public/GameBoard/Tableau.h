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

	/** Constructors */
	SATableau(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, SVector<SSharedPtr<SACard>>&& NewCards);
    SATableau(const SATableau& Other);

    /** Operators */
    SATableau operator=(const SATableau& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }

        return *this;
    }

    /** Get Cards */
    inline const TableauArray& GetCards() const { return Cards; }

    /** Renders the tableau to the console */
    void Write() override;

    /** Clears the tableau from the console buffer */
    void ClearBuffer() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    virtual void CopyFrom(const SAActor& Other);

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SATableau(*this)); }

protected:
    /** Generates the card columns */
    void GenerateColumns(SVector<SSharedPtr<SACard>>&& NewCards);

    /** Array of card columns */
    TableauArray Cards;
};