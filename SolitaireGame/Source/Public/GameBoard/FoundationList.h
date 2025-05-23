#pragma once
#include "Framework/Actor.h"

class SACard;

class SAFoundationList final : public SAActor
{
public:
    SAFoundationList() : SAActor(SGridPositionU32(0, 0)) {}
    /** Get Cards */
    inline const SArray<SVector<SSharedPtr<SACard>>, 4>& GetCards() const { return FoundationList; }

    /** Add a new card to the Foundation Lists */
    bool AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position);
    
    /** Set logical card data. */
    void SetCardInfo(SSharedPtr<SACard> Card);

    /** Renders the Foundation List. */
    void Write() override;

    /** Clears the Foundation List from the console. */
    void ClearBuffer() override;

protected:
    /** Array of 4 Foundation columns */
    SArray<SVector<SSharedPtr<SACard>>, 4> FoundationList;
};