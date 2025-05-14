#include "SolitaireEnginePCH.h"
#include "Core/Rendering/ConsoleSelector.h"

FSelectionCursor::FSelectionCursor(const SWString& NewCursor, const WORD NewCursorColor)
    : Cursor(NewCursor), CursorColor(NewCursorColor) {}

void FSelectionCursor::WriteAt(const SGridPositionU32& NewGridPosition)
{
    // Set new grid position
    SetGridPosition(NewGridPosition);

    // Get the console renderer instance
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Check if the previous cursor position is set (i.e., it's not the value (-1, -1))
    if (PreviousCursorPosition != INVALID_GRID_POSITION)
    {
        // If the previous cursor position is valid, clear the previous cursor position in the buffer
        // (where "Cursor.size()" defines the width of the cursor to be cleared)
        ConsoleRenderer->ClearBufferAt(PreviousCursorPosition, Cursor.size());
    }

    // Update the current cursor position using the new calculated position
    PreviousCursorPosition = SGridPositionU32(CalculateCursorXPosition(GridPosition.first, Cursor), GridPosition.second);

    // Write the cursor at the new position to the console buffer with the specified color (RED)
    ConsoleRenderer->Write(PreviousCursorPosition, Cursor, RED);
}

void FSelectionCursor::ClearBufferAt(const SGridPositionU32& GridPosition)
{
    SConsoleRenderer::GetInstance()->ClearBufferAt(SGridPositionU32(CalculateCursorXPosition(GridPosition.first, Cursor), GridPosition.second), Cursor.size());
}

SConsoleSelector::SConsoleSelector(const SGridPositionU32& NewGridPosition) : SIConsoleRenderable(NewGridPosition) {}

SConsoleSelector::SConsoleSelector(const SInitializerList<SPair<SWStringView, SCallback>>& InitializerList, const SGridPositionU32& NewGridPosition)
    : SIConsoleRenderable(NewGridPosition)
{   
    // Try to insert each element from the initializer list into the 'Options' map
    for (auto& Element : InitializerList)
    {
        // Try to emplace the Element
        Options.try_emplace(Element.first, Element.second);
    }
    
    // Check if the InitializerList isn't empty
    if (InitializerList.size() != 0)
    {
        // Create the cursor string (e.g., "->" to indicate selection)
        SWString CursorString = TEXT("->");

        // Calculate the X position of the cursor (left of the options text)
        SUInt32 CursorPositionX = GridPosition.first - static_cast<SUInt32>(CursorString.size()) - 1;

        // Validate if the cursor can be rendered within the screen bounds
        if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
        {
            // Create the cursor instance with the specified string, color, and position
            Cursor = std::make_unique<FSelectionCursor>(CursorString, RED);
        }
        else
        {
            S_LOG_WARNING(LogConsoleRenderer, TEXT("We can't render cursor because CursorPositionX is greater than ScreenWidth"));
        }
    }
}

SBool SConsoleSelector::Initialize()
{
    //Bind functions
    SInputSystem::GetInstance()->AddCallback(EKeyAction::MoveDown, std::bind(&SConsoleSelector::OnArrowDownPressed, this));
    SInputSystem::GetInstance()->AddCallback(EKeyAction::MoveUp, std::bind(&SConsoleSelector::OnArrowUpPressed, this));
    SInputSystem::GetInstance()->AddCallback(EKeyAction::Select, std::bind(&SConsoleSelector::OnEnterPressed, this));

    return true;
}

void SConsoleSelector::Write()
{
    // Get the instance of the Console Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Iterate through each element in the 'Options' map (key-value pairs)
    SUInt8 Index = 0;
    for (const auto& Element : Options)
    {
        // Write the element's key (converted to a string) to the console at the calculated position
        ConsoleRenderer->Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index++), Element.first.data(), WHITE);
    }

    //Write cursor
    Cursor->WriteAt(SGridPositionU32(GridPosition.first, GridPosition.second + CurrentIndex));
}

void SConsoleSelector::AddOption(const SWStringView& NewOption, const SCallback& Callback)
{
    // Insert the new option and its callback into the Options map
    Options.try_emplace(NewOption, Callback);

    // Write to the buffer newly added option at the given position
    SConsoleRenderer::GetInstance()->Write(SGridPositionU32(GridPosition.first, GridPosition.second + static_cast<SUInt32>(Options.size())), NewOption.data(), WHITE);

    //Check if the cursor is nullptr
    if (Cursor.get() == nullptr)
    {
        // Create the cursor string (e.g., "->" to indicate selection)
        SWString CursorString = TEXT("->");

        // Calculate the X position of the cursor (left of the options text)
        SUInt32 CursorPositionX = GridPosition.first - static_cast<SUInt32>(CursorString.size()) - 1;

        // Validate if the cursor can be rendered within the screen bounds
        if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
        {
            // Create the cursor instance with the specified string, color, and position
            Cursor = std::make_unique<FSelectionCursor>(CursorString, RED);
        }
    }
}

void SConsoleSelector::ClearBuffer()
{
    // Get the instance of the Console Renderer singleton
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();
    
    // Iterate through all options and clear each one from the console buffer
    SUInt8 Index = 0;
    for (const auto& Element : Options)
    {
        // Clear the text at the specific grid position where the option was drawn
        ConsoleRenderer->ClearBufferAt(SGridPositionU32(GridPosition.first, GridPosition.second + Index++), Element.first.size());
    }

    //Clear map
    Options.clear();

    //Destroy cursor
    Cursor->ClearBufferAt(SGridPositionU32(GridPosition.first, GridPosition.second + CurrentIndex));
    Cursor.reset();
}

void SConsoleSelector::CreateCursor(const SGridPositionU32& GridPosition)
{
    // Create the cursor string (e.g., "->" to indicate selection)
    SWString CursorString = TEXT("->");

    // Calculate the X position of the cursor (left of the options text)
    SUInt32 CursorPositionX = GridPosition.first - static_cast<SUInt32>(CursorString.size()) - 1;

    // Validate if the cursor can be rendered within the screen bounds
    if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
    {
        // Create the cursor instance with the specified string, color, and position
        Cursor = std::make_unique<FSelectionCursor>(CursorString, RED);
    }
}

void SConsoleSelector::OnArrowUpPressed()
{
    // Get maximum index (i.e. total number of options)
    const SUInt32 MaxIndex = static_cast<SUInt32>(Options.size());

    // Move selection up, wrapping to bottom if at top
    CurrentIndex = (CurrentIndex + MaxIndex - 1) % MaxIndex;
}

void SConsoleSelector::OnArrowDownPressed()
{
    // Move selection down, wrapping to top if at bottom
    CurrentIndex = (CurrentIndex + 1) % static_cast<SUInt32>(Options.size());
}

void SConsoleSelector::OnEnterPressed()
{
    // Get the iterator pointing to the currently selected option 
    auto Iterator = Options.begin();
    std::advance(Iterator, CurrentIndex);

    // If the callback is valid, execute it
    if (Iterator->second)
    {
        Iterator->second();
    }
}