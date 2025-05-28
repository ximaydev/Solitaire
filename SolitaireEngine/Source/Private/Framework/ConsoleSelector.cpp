#include "SolitaireEnginePCH.h"
#include "Framework/ConsoleSelector.h"
#include "Inputs/InputSystem.h"

FSelectionCursor::FSelectionCursor(const SWString& NewCursor, const WORD NewCursorColor) : Cursor(NewCursor), CursorColor(NewCursorColor) {}

FSelectionCursor::FSelectionCursor(const SGridPositionU32& NewGridPosition, const SWString& NewCursor, const WORD NewCursorColor)
    : SIConsoleRenderable(NewGridPosition), Cursor(NewCursor), CursorColor(NewCursorColor) {}

void FSelectionCursor::Write()
{
    // Update the current cursor position using the new calculated position
    GridPosition = SGridPositionU32(CalculateCursorXPosition(GridPosition.first, Cursor), GridPosition.second);

    // Write the cursor at the new position to the console buffer with the specified color (RED)
    SConsoleRenderer::GetInstance()->Write(GridPosition, Cursor, static_cast<SUInt32>(Cursor.size()), true, FG_RED);
}

void FSelectionCursor::ClearBuffer()
{
    SConsoleRenderer::GetInstance()->ClearBufferAt(SGridPositionU32(GridPosition.first, GridPosition.second), static_cast<SUInt32>(Cursor.size()));
}

SAConsoleSelector::SAConsoleSelector(const SInitializerList<SPair<SWStringView, SCallback>>& InitializerList, const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld)
    : SAActor(NewGridPosition, NewWorld)
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
        // Calculate the X position of the cursor (left of the options text)
        SUInt32 CursorPositionX = FSelectionCursor::CalculateCursorXPosition(GetGridPosition().first, CursorString);

        // Validate if the cursor can be rendered within the screen bounds
        if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
        {
            // Create the cursor instance with the specified string, color, and position
            Cursor = std::make_shared<FSelectionCursor>(SGridPositionU32(CursorPositionX, GetGridPosition().second), CursorString, FG_RED);
            Initialize();
        }
        else
        {
            S_LOG_WARNING(LogConsoleRenderer, TEXT("We can't render cursor because CursorPositionX is greater than ScreenWidth"));
        }
    }
}

SAConsoleSelector::SAConsoleSelector(const SAConsoleSelector& Other)
{
    // Call CopyFrom and perform a deep copy
    CopyFrom(Other);
}

SBool SAConsoleSelector::Initialize()
{
    // Get Input System
    SInputSystem* InputSystem = SInputSystem::GetInstance();

    //Bind functions
    InputSystem->AddCallback(EKeyAction::MoveDown, std::bind(&SAConsoleSelector::OnArrowDownPressed, this));
    InputSystem->AddCallback(EKeyAction::MoveUp, std::bind(&SAConsoleSelector::OnArrowUpPressed, this));
    InputSystem->AddCallback(EKeyAction::Select, std::bind(&SAConsoleSelector::OnEnterPressed, this));

    return true;
}

void SAConsoleSelector::Write()
{
    // Get the instance of the Console Renderer
    SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

    // Iterate through each element in the 'Options' map (key-value pairs)
    SUInt8 Index = 0;
    for (const auto& Element : Options)
    {
        // Write the element's key (converted to a string) to the console at the calculated position
        ConsoleRenderer->Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index++), Element.first.data(), static_cast<SUInt32>(Element.first.size()), true, FG_WHITE);
    }

    // Clear old buffer
    Cursor->ClearBuffer();

    // Set new grid position
    Cursor->SetGridPosition(SGridPositionU32(GridPosition.first, GridPosition.second + CurrentIndex));

    // Write cursor
    Cursor->Write();
}

void SAConsoleSelector::ClearBuffer()
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
    Cursor->ClearBuffer();
    Cursor.reset();
}

void SAConsoleSelector::CopyFrom(const SAActor& Other)
{
    // Attempt to cast the base class reference to a SAConsoleSelector pointer
    if (const SAConsoleSelector* OtherConsoleSelector = dynamic_cast<const SAConsoleSelector*>(&Other))
    {
        // Call the parent CopyFrom
        SAActor::CopyFrom(Other);
        
        // Copy the list of selectable options from the source object
        Options = OtherConsoleSelector->Options;

        // Copy the currently selected index
        CurrentIndex = OtherConsoleSelector->CurrentIndex;

        // Copy the cursor position (used for navigating the options)
        Cursor = OtherConsoleSelector->Cursor;

        // Copy the string used to render the cursor
        CursorString = OtherConsoleSelector->CursorString;
    }
    else
    {
        S_LOG_ERROR(LogTemp, TEXT("CopyFrom failed, Casted failed other isn't type of SAConsoleSelector."))
    }
}

void SAConsoleSelector::AddOption(const SWStringView& NewOption, const SCallback& Callback)
{
    // Insert the new option and its callback into the Options map
    Options.try_emplace(NewOption, Callback);

    // Write to the buffer newly added option at the given position
    SConsoleRenderer::GetInstance()->Write(SGridPositionU32(GridPosition.first, GridPosition.second + static_cast<SUInt32>(Options.size())), NewOption.data(), static_cast<SUInt32>(NewOption.size()), true, FG_WHITE);

    //Check if the cursor is nullptr
    if (Cursor.get() == nullptr)
    {
        // Calculate the X position of the cursor (left of the options text)
        SUInt32 CursorPositionX = GridPosition.first - static_cast<SUInt32>(CursorString.size()) - 1;

        // Validate if the cursor can be rendered within the screen bounds
        if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
        {
            // Create the cursor instance with the specified string, color, and position
            Cursor = std::make_shared<FSelectionCursor>(CursorString, FG_RED);
        }
    }
}

void SAConsoleSelector::SetNewCallback(const SWStringView& NewOption, const SCallback& Callback)
{
    // Try to find the option key in the map
    auto It = Options.find(NewOption);
    if (It != Options.end())
    {
        // If found, update the callback associated with this option
        It->second = Callback;
    }
    else
    {
        S_LOG_WARNING(LogTemp, TEXT("We wanted to set new callback for a %s but this string doesn't exists in the Options map"), NewOption.data())
    }
}

void SAConsoleSelector::CreateCursor(const SGridPositionU32& GridPosition)
{
    // Calculate the X position of the cursor (left of the options text)
    SUInt32 CursorPositionX = GridPosition.first - static_cast<SUInt32>(CursorString.size()) - 1;

    // Validate if the cursor can be rendered within the screen bounds
    if (SConsoleRenderer::GetInstance()->GetScreenWidth() >= CursorPositionX)
    {
        // Create the cursor instance with the specified string, color, and position
        Cursor = std::make_shared<FSelectionCursor>(CursorString, FG_RED);
    }
}

void SAConsoleSelector::OnArrowUpPressed()
{
    // Get maximum index (i.e. total number of options)
    const SUInt32 MaxIndex = static_cast<SUInt32>(Options.size());

    // Move selection up, wrapping to bottom if at top
    CurrentIndex = (CurrentIndex + MaxIndex - 1) % MaxIndex;
}

void SAConsoleSelector::OnArrowDownPressed()
{
    // Move selection down, wrapping to top if at bottom
    CurrentIndex = (CurrentIndex + 1) % static_cast<SUInt32>(Options.size());
}

void SAConsoleSelector::OnEnterPressed()
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