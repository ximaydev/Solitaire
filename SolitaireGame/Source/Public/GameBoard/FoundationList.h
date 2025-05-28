#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** Represents the Foundation piles in Solitaire game. */
class SAFoundationList final : public SAActor
{
public:
    // Number of columns (piles) in the foundation area of the game board
    static constexpr SUInt32 NumFoundationColumns = 4;

    using FoundationPilesArray = SArray<SVector<SSharedPtr<SACard>>, NumFoundationColumns>;

    /** Constructor */
    SAFoundationList(SSharedPtr<SWorld> NewWorld, const SGridPositionU32& NewGridPosition);
    SAFoundationList(const SAFoundationList& Other);

    /** Operators */
    SAFoundationList operator=(const SAFoundationList& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }
        return *this;
    }

    /** Get Cards */
    inline const FoundationPilesArray& GetCards() const { return FoundationList; }

    /** Add a new card to the Foundation Lists */
    bool AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position);

    /** Renders the Foundation List. */
    void Write() override;

    /** Clears the Foundation List from the console. */
    void ClearBuffer() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    void CopyFrom(const SAActor& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAFoundationList(*this)); }
protected:
    /** Array of 4 Foundation columns */
    FoundationPilesArray FoundationList;
};