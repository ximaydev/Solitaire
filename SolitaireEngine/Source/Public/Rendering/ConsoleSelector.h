#pragma once
#include "Globals.h"
#include "Rendering/ConsoleRenderer.h"

/** Represents a visual selection cursor in the console (e.g., "->") */
class SOLITAIRE_ENGINE_API FSelectionCursor : public SIConsoleRenderable
{
public:
    /** Constructors */
    FSelectionCursor() = delete;
    FSelectionCursor(const SWString& NewCursor, const WORD NewCursorColor);

    /** Calculates the X position for the selection cursor based on the grid X coordinate */
    inline SUInt32 CalculateCursorXPosition(SUInt32 GridX, const SWString& CursorString) { return GridX - static_cast<SUInt32>(CursorString.size()) - 1; }

    /** Writes the selection cursor to the console at the given grid position. */
    virtual void WriteAt(const SGridPositionU32& GridPosition) override;

    /** Clears the screen buffer at the given position */
    virtual void ClearBufferAt(const SGridPositionU32& GridPosition) override;

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
class SOLITAIRE_ENGINE_API SConsoleSelector : public SIConsoleRenderable
{
public:
    /** Constructor */
    SConsoleSelector() = delete;
    SConsoleSelector(const SGridPositionU32& NewGridPosition);
    SConsoleSelector(const SInitializerList<SPair<SWStringView, SCallback>>& InitializerList, const SGridPositionU32& NewGridPosition);

    /** Initializes this object. */
    SBool Initialize();

    /** Write all selectable options to the console buffer with the current highlight */
    virtual void Write() override;

    /** Adds a new selectable option with its associated callback */
    virtual void AddOption(const SWStringView& NewOption, const SCallback& Callback);

    /** Clears all registered options */
    virtual void ClearBuffer() override;

protected:
    /** List of selectable options with their associated callbacks */
    SUnorderedMap<SWStringView, SCallback> Options;

    /** Index of the currently selected option */
    SUInt8 CurrentIndex = {};

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