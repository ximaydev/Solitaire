#pragma once
#include "Framework/Actor.h"

/** Forward Declarations */
class SACard;

/** Represents the Foundation piles in Solitaire game. */
class SAFoundationList final : public SAActor
{
public:
    /** Constructor */
    SAFoundationList(SSharedPtr<SWorld> NewWorld);

    /** Get Cards */
    inline const SArray<SVector<SSharedPtr<SACard>>, 4>& GetCards() const { return FoundationList; }

    /** Add a new card to the Foundation Lists */
    bool AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position);

    /** Renders the Foundation List. */
    void Write() override;

    /** Clears the Foundation List from the console. */
    void ClearBuffer() override;

protected:
    /** Array of 4 Foundation columns */
    SArray<SVector<SSharedPtr<SACard>>, 4> FoundationList;
};