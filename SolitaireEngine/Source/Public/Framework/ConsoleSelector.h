#pragma once
#include "Globals.h"
#include "Framework/Actor.h"

/** Represents a visual selection cursor in the console (e.g., "->") */
class SOLITAIRE_ENGINE_API FSelectionCursor : public SIConsoleRenderable
{
public:
    /** Constructors */
    FSelectionCursor(const SWString& NewCursor, const WORD NewCursorColor);
    FSelectionCursor(const SGridPositionU32& NewGridPosition, const SWString& NewCursor, const WORD NewCursorColor);

    /** Writes the selection cursor to the console. */
    virtual void Write() override;

    /** Clears the screen buffer. */
    virtual void ClearBuffer() override;

    /** Calculates the X position for the selection cursor based on the grid X coordinate */
    static inline SUInt32 CalculateCursorXPosition(SUInt32 GridX, const SWString& CursorString) { return GridX - static_cast<SUInt32>(CursorString.size()) - 1; }

protected:
    /** Text or symbol representing the cursor (e.g., "->") */
    const SWString Cursor;

    /** Color used to render the cursor */
    const WORD CursorColor;
};

/**
 * Represents a console-based selector that can draw selectable options
 * and handle keyboard navigation through them.
 */
class SOLITAIRE_ENGINE_API SAConsoleSelector : public SAActor
{
public:
    /** Constructor */
    SAConsoleSelector(const SInitializerList<SPair<SWStringView, SCallback>>& InitializerList, const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld);
    SAConsoleSelector(const SAConsoleSelector& Other);

    /** Operators */
    SAConsoleSelector operator=(const SAConsoleSelector& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }
        return *this;
    }

    /** Initializes this object. */ 
    SBool Initialize();

    /** Write all selectable options to the console buffer with the current highlight */
    virtual void Write() override;

    /** Clears all registered options */
    virtual void ClearBuffer() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    virtual void CopyFrom(const SAActor& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAConsoleSelector(*this)); }

    /** Adds a new selectable option with its associated callback */
    virtual void AddOption(const SWStringView& NewOption, const SCallback& Callback);

    /** Set new callback for the given key */
    void SetNewCallback(const SWStringView& NewOption, const SCallback& Callback);

protected:
    /** List of selectable options with their associated callbacks */
    SUnorderedMap<SWStringView, SCallback> Options;

    /** Index of the currently selected option */
    SUInt8 CurrentIndex = {};

    /** Cursor indicating the current line of option */
    SSharedPtr<FSelectionCursor> Cursor;

    /** The visual representation of the cursor in the console */
    SWString CursorString = TEXT("->");

    /** This function creates the cursor object */
    void CreateCursor(const SGridPositionU32& GridPosition);

private:
    /** Should be called when the user presses the Up Arrow key */
    void OnArrowUpPressed();

    /** Should be called when the user presses the Down Arrow key */
    void OnArrowDownPressed();

    /** Should be called when the user presses the Enter key */
    void OnEnterPressed();
};