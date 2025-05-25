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

    /** Get Cards */
    inline const FoundationPilesArray& GetCards() const { return FoundationList; }

    /** Add a new card to the Foundation Lists */
    bool AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position);

    /** Renders the Foundation List. */
    void Write() override;

    /** Clears the Foundation List from the console. */
    void ClearBuffer() override;

protected:
    /** Array of 4 Foundation columns */
    FoundationPilesArray FoundationList;
};