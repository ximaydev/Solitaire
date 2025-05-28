#pragma once
#include "Framework/Actor.h"

class SAOperationsHelpActor : public SAActor
{
public:
    /** Constructors */
    SAOperationsHelpActor(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld);
    SAOperationsHelpActor(const SAOperationsHelpActor& Other);

    /** Operators */
    SAOperationsHelpActor operator=(const SAOperationsHelpActor& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }
        return *this;
    }

    /** Renders the Operations Helper at a specified grid position. */
    void Write() override;

    /** Clears the Operations Helper from the console using its internal grid position. */
    void ClearBuffer() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    virtual void CopyFrom(const SAActor& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAOperationsHelpActor(*this)); }

protected:
    SVector<SWString> Operations;
};