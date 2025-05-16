#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
#include <cassert>
#include "Config/IniFile.h"
#include "Config/IniFileManager.h"

SConsoleRenderer::SConsoleRenderer()
{
    // Acquire handle to the standard output console buffer
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);  
    if (ConsoleOutput == nullptr)
    {
        // Log error with code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("Failed to get STD_OUTPUT_HANDLE: %lu"), GetLastError());

        // Exit program on failure
        exit(1);                           
    }

    // Get system-defined minimum console dimensions
    SInt32 MinWidth = GetSystemMetrics(SM_CXMIN);    // Minimum console width
    SInt32 MinHeight = GetSystemMetrics(SM_CYMIN);   // Minimum console height
    SBool bResized = false;                          // Flag if dimensions changed

    // Ensure the desired width is at least the system minimum
    if (MinWidth > ScreenWidth)
    {
        // Update ScreenWidth to system minimum
        ScreenWidth = MinWidth;

        // Mark that we resized
        bResized = true;
    }
    // Ensure the desired height is at least the system minimum
    if (MinHeight > ScreenHeight)
    {
        // Update ScreenHeight to system minimum
        ScreenHeight = MinHeight;

        // Mark that we resized
        bResized = true;
    }

    // If either dimension changed, recalculate buffer size
    if (bResized)
    {
        // Total console buffer cells
        BufferSize = ScreenWidth * ScreenHeight;
    }

    // Define new console buffer size using COORD struct
    COORD NewBufferSize(ScreenWidth, ScreenHeight);

    // Attempt to resize buffer
    if (!SetConsoleScreenBufferSize(ConsoleOutput, NewBufferSize))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleScreenBufferSize failed: %lu"), GetLastError());

        // Terminate on failure
        exit(1);
    }

    // Define window rectangle to match buffer dimensions (inclusive coordinates)
    SMALL_RECT WindowRectangle(0, 0, ScreenWidth - 1, ScreenHeight - 1);
    if (!SetConsoleWindowInfo(ConsoleOutput, TRUE, &WindowRectangle))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleWindowInfo failed: %lu"), GetLastError());  

        // Terminate on failure
        exit(1);
    }

    // Get HWND for console window to manipulate position
    HWND ConsoleWindow = GetConsoleWindow();
    if (ConsoleWindow == nullptr)
    {
        // Log null handle
        S_LOG_ERROR(LogConsoleRenderer, TEXT("GetConsoleWindow returned NULL"));  

        // Terminate on failure
        exit(1);  
    }

    // Obtain current window rectangle (screen coordinates)
    if (!GetWindowRect(ConsoleWindow, &ConsoleWindowRectangle))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("GetWindowRect failed: %lu"), GetLastError());

        // Terminate on failure
        exit(1);
    }

    // Calculate center position of window on the screen
    int ScreenW = GetSystemMetrics(SM_CXSCREEN);  // Total screen width in pixels
    int screenH = GetSystemMetrics(SM_CYSCREEN);  // Total screen height in pixels
    int WinW = ConsoleWindowRectangle.right - ConsoleWindowRectangle.left;   // Window width
    int WinH = ConsoleWindowRectangle.bottom - ConsoleWindowRectangle.top;   // Window height
    int PositionX = (ScreenW - WinW) / 2;  // X-coordinate to center
    int PositionY = (screenH - WinH) / 2;  // Y-coordinate to center

    // Move window, keep size
    SetWindowPos(ConsoleWindow, nullptr, PositionX, PositionY, 0, 0, SWP_NOSIZE);

    // Configure console window
    ConfigureConsoleWindow(ConsoleWindow);

    // Allocate in-memory buffer for rendering characters
    ScreenBuffer = std::make_unique<SWideChar[]>(BufferSize);

    // Clear buffer contents (custom method)
    ClearBuffer();

    // Create a new console buffer
    ConsoleScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    if (ConsoleScreenBuffer == nullptr)
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("CreateConsoleScreenBuffer failed: %lu"), GetLastError());

        // Terminate on failure
        exit(1); 
    }

    // Activate the newly created console screen buffer
    if (!SetConsoleActiveScreenBuffer(ConsoleScreenBuffer))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleActiveScreenBuffer failed: %lu"), GetLastError());

        // Terminate on failure
        exit(1);
    }

    // Get Current Background Color from the DefaultEngine.ini file
    CurrentBackgroundColor = SIniFileManager::GetInstance()->GetConfigFile(DefaultEngineConfig)->GetValueFromKey<WORD>(TEXT("[Solitaire.ConsoleStyle]"), TEXT("BackgroundColor"));
    
    // Set the background color
    if (!SetBackgroundColor(SGridPositionU32(0, 0), ScreenWidth * ScreenHeight, CurrentBackgroundColor))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("FillConsoleOutputAttribute failed (BackgroundColor = %d): %lu"), CurrentBackgroundColor, GetLastError());

        // Terminate on failure
        exit(1);
    }

    // Hide cursor
    if (!ShowConsolCursor(false))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("ShowConsoleCursor failed: %lu"), GetLastError());

        // Terminate on failure
        exit(1);
    }

    // Log successful initialization with final dimensions
    S_LOG(LogConsoleRenderer, TEXT("ConsoleRenderer initialized: %ux%u"), ScreenWidth, ScreenHeight);
}

SConsoleRenderer* SConsoleRenderer::GetInstance()
{
    // Create a static unique pointer to hold the only instance of SConsoleRenderer
    static SUniquePtr<SConsoleRenderer> ConsoleRenderer = std::make_unique<SConsoleRenderer>();
    return ConsoleRenderer.get();
}

void SConsoleRenderer::ClearBuffer()
{
    // Loop through all buffer cells
    for (SInt32 Index = 0; Index < BufferSize; Index++)
    {
        // Set each cell to an empty wide character
        ScreenBuffer[Index] = TEXT(' ');
    }
}

void SConsoleRenderer::ClearBufferAt(const SGridPositionU32& GridPosition, SSize Size)
{
    // Create the empty string
    SWString EmptyString;
    EmptyString.resize(Size, TEXT(' '));

    // Write the empty text to the screen buffer
    swprintf(&ScreenBuffer[ValidateWriteBounds(GridPosition, Size)], BufferSize, EmptyString.c_str());
}

void SConsoleRenderer::Write(const SGridPositionU32& GridPosition, const SWString& Text, WORD Color)
{
    // Characters considered invalid for coloring
    SArray<SWideChar, 4> InvalidCharacters = { TEXT(' '), TEXT('\n'), TEXT('\r'), TEXT('\t') };

    // Lambda to check if a given character matches the first character of Text
    auto IsAnyOf = [&Text](SWideChar Character)
        {
            return Character == Text[0];
        };

    //Get the size of the text
    SSize TextSize = Text.size();

    // Apply color only if the text is not empty,
    // and the first character is not considered invalid
    if (!Text.empty())
    {
        if (std::find_if(InvalidCharacters.begin(), InvalidCharacters.end(), IsAnyOf) == InvalidCharacters.end())
        {
            SetTextColor(GridPosition, TextSize, Color);
        }
        else
        {
            S_LOG(LogConsoleRenderer, TEXT("Skipping SetTextColor: character '%c' is invalid."), Text[0]);
        }
    }

    // Write the text to the screen buffer
    swprintf(&ScreenBuffer[ValidateWriteBounds(GridPosition, TextSize)], BufferSize, Text.c_str());
}

void SConsoleRenderer::Draw()
{
    // Draw the console border
    DrawBorder();

    // Write the entire screen buffer to the console output starting at position (0, 0)
    DWORD BytesWritten = 0;
    WriteConsoleOutputCharacterW(ConsoleScreenBuffer, ScreenBuffer.get(), BufferSize, { 0, 0 }, &BytesWritten);
}

bool SConsoleRenderer::ShowConsolCursor(SBool bShowCursor)
{
    CONSOLE_CURSOR_INFO CursorInfo;

    // Attempt to retrieve the current cursor information
    if (!GetConsoleCursorInfo(ConsoleScreenBuffer, &CursorInfo))
    {
        S_LOG_WARNING(LogConsoleRenderer, TEXT("Failed to retrieve cursor info."));
        return false;
    }

    // Set cursor visibility based on input flag
    CursorInfo.bVisible = bShowCursor;

    // Apply the updated cursor settings
    return SetConsoleCursorInfo(ConsoleScreenBuffer, &CursorInfo);
}

void SConsoleRenderer::DrawHorizontalPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, WORD Color, const SWString& Character)
{
    // If the input character is empty, do nothing
    if (Character == TEXT(""))
        return;

    // Create a string that will hold the repeated characters
    SWString HorizontalPanel;
    HorizontalPanel.reserve(Width);

    // Append the given character 'Width' times to form the horizontal line
    for (SUInt32 Index = 0; Index < Width; Index++)
    {
        HorizontalPanel += Character;
    }

    // Write the constructed string to the screen buffer at the specified position with the given color
    Write(GridPosition, HorizontalPanel, Color);
}

void SConsoleRenderer::DrawVerticalPanel(const SGridPositionU32& GridPosition, const SUInt32 Height, WORD Color, const SWString& Character)
{
    // If the input character is empty, do nothing
    if (Character == TEXT(""))
        return;

    // Append the given character 'Height' times to form the vertical line
    for (SUInt32 Index = 0; Index < Height; Index++)
    {
        // Write the character to the screen buffer at the specified position with the given color
        Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index), Character, Color);
    }
}

void SConsoleRenderer::DrawPanel(const SGridPositionU32& GridPosition, const SUInt32 Width, const SUInt32 Height, WORD Color, const SWString& WidthCharacter, const SWString& HeightCharacter)
{
    // Draw top horizontal border
    DrawHorizontalPanel(GridPosition, Width, Color, WidthCharacter);

    // Draw bottom horizontal border
    DrawHorizontalPanel(SGridPositionU32(GridPosition.first, GridPosition.second + Height), Width, Color, WidthCharacter);

    // Draw left vertical border
    DrawVerticalPanel(SGridPositionU32(GridPosition.first, GridPosition.second + 1), Height - 1, Color, HeightCharacter);

    // Draw right vertical border
    DrawVerticalPanel(SGridPositionU32(GridPosition.first + Width - 1, GridPosition.second + 1), Height - 1, Color, HeightCharacter);
}

SUInt32 SConsoleRenderer::ValidateWriteBounds(const SGridPositionU32& GridPosition, SSize TextLength)
{
    // Convert (x, y) position to buffer index
    SUInt32 Position = GridPosition.second * ScreenWidth + GridPosition.first;

    // Validate that we're not writing past the buffer
    if (Position + TextLength > BufferSize)
    {
        S_LOG_WARNING(LogConsoleRenderer, TEXT("Attempted to write beyond buffer! Position: %u, TextSize: %zu, BufferSize: %zu"), Position, TextLength, BufferSize);
        assert(false && "You wanted to write beyond the buffer");
    }

    return Position;
}

void SConsoleRenderer::DrawBorder()
{
    DrawPanel(SGridPosition<SUInt32, SUInt32>(1, 0), GetScreenWidth() - 2, GetScreenHeight() - 1, FG_BLACK | GetCurrentBackgroundColor(), TEXT("#"), TEXT("#"));
}

bool SConsoleRenderer::SetTextColor(const SGridPositionU32& GridPosition, const SSize Size, WORD Color)
{
    // Define the position in the console screen buffer where the text color should be applied
    COORD Position(GridPosition.first, GridPosition.second);

    // Create a vector of colors (with the same color for all positions)
    SVector<WORD> Write(Size, Color);

    DWORD Written = 0;
    
    // Attempt to write the color attributes to the console screen buffer
    if (!WriteConsoleOutputAttribute(ConsoleScreenBuffer, &Write[0], static_cast<DWORD>(Size), Position, &Written))
    {
        // Log a warning if writing the color attributes fails
        S_LOG_WARNING(LogConsoleRenderer, TEXT("Unable to change the text color"));
        return false;
    }

    // Successfully set the text color
    return true;
}

bool SConsoleRenderer::SetBackgroundColor(const SGridPositionU32& GridPosition, const SUInt32 BufferSize, WORD BackgroundColor)
{
    // Fill the console with the specified background color
    DWORD Written = 0;
    if (!FillConsoleOutputAttribute(ConsoleScreenBuffer, BackgroundColor, BufferSize, {0, 0}, &Written))
    {
        return false;
    }

    return true;
}

void SConsoleRenderer::ConfigureConsoleWindow(HWND ConsoleHandle)
{
    // Get the current window style of the console window
    LONG Style = GetWindowLong(ConsoleHandle, GWL_STYLE);

    // Disable the ability to resize the window (WS_SIZEBOX) and prevent maximizing it (WS_MAXIMIZEBOX)
    Style &= ~WS_SIZEBOX;
    Style &= ~WS_MAXIMIZEBOX;

    // Apply the modified window style back to the console window
    SetWindowLongPtr(ConsoleHandle, GWL_STYLE, Style);

    // Redraw the window with the new style to enforce the changes
    SetWindowPos(ConsoleHandle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // Get handle to the standard output console buffer
    HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

    // Get the current input mode for the console input buffer
    DWORD ConsoleMode;
    GetConsoleMode(ConsoleInput, &ConsoleMode);

    // Remove mouse support and quick edit mode
    ConsoleMode &= ~ENABLE_MOUSE_INPUT;
    ConsoleMode &= ~ENABLE_QUICK_EDIT_MODE;

    // Apply the modified console mode settings
    SetConsoleMode(ConsoleInput, ConsoleMode);
}