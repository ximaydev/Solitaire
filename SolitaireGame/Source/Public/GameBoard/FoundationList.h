#pragma once
#include "Framework/Actor.h"

class SACard;

class SAFoundationList final : public SAActor
{
public:
    /** Add a new card to the Foundation Lists */
    bool AddNewCardToFoundationList(SSharedPtr<SACard> NewCard, SUInt8 Position);
    
    /** Set logical card data. */
    void SetCardInfo(SACard* Card, SUInt8 Position);

    /** Renders the Foundation List. */
    void Write() override;

    /** Clears the Foundation List from the console. */
    void ClearBuffer() override;

protected:
    /** Array of 4 Foundation columns */
    SArray<SVector<SSharedPtr<SACard>>, 4> FoundationList;
};