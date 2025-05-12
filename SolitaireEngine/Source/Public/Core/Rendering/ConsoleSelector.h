#pragma once
#include "Globals.h"

/** Represents an invalid or uninitialized 2D grid position(commonly used as a sentinel value) */
#define INVALID_GRID_POSITION SGridPositionU32((SUInt32)-1, (SUInt32)-1)

/** Represents a visual selection cursor in the console (e.g., "->") */
struct SOLITAIRE_ENGINE_API FSelectionCursor
{
public:
    /** Constructors */
    FSelectionCursor() = delete;
    FSelectionCursor(const SWString& NewCursor, const WORD NewCursorColor);

    /** Calculates the X position for the selection cursor based on the grid X coordinate */
    inline SUInt32 CalculateCursorXPosition(SUInt32 GridX, const SWString& CursorString) { return GridX - static_cast<SUInt32>(CursorString.size()) - 1; }

    /** Writes the selection cursor to the console at the given grid position. */
    void Write(const SGridPositionU32& GridPosition);

    /** Clears the cursor from the screen at the specified grid position */
    void ClearAt(const SGridPositionU32& GridPosition);

protected:
    /** Text or symbol representing the cursor (e.g., "->") */
    const SWString Cursor;

    /** Color used to render the cursor */
    const WORD CursorColor;

    /** Previous position of the cursor */
    SGridPositionU32 PreviousCursorPosition = INVALID_GRID_POSITION;
};

/**
 * Represents a console-based selector that can draw selectable options
 * and handle keyboard navigation through them.
 */
class SOLITAIRE_ENGINE_API SConsoleSelector
{
public:
    /** Constructor */
    SConsoleSelector() = delete;
    SConsoleSelector(const SGridPositionU32& NewGridPosition);
    SConsoleSelector(const SInitializerList<SPair<SWStringView, SCallback>>& InitializerList, const SGridPositionU32& NewGridPosition);

    /** Initializes this object. */
    SBool Initialize();

    /** Write all selectable options to the console buffer with the current highlight */
    virtual void Write();

    /** Adds a new selectable option with its associated callback */
    virtual void AddOption(const SWStringView& NewOption, const SCallback& Callback);

    /** Clears all registered options */
    virtual void ClearOptions();

protected:
    /** List of selectable options with their associated callbacks */
    SUnorderedMap<SWStringView, SCallback> Options;

    /** Index of the currently selected option */
    SUInt8 CurrentIndex = {};

    /** Position of the fist option in the console (x,y) */
    const SGridPositionU32 GridPosition;

    /** Cursor indicating the current line of option */
    SUniquePtr<FSelectionCursor> Cursor;

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