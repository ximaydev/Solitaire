#include "SolitaireEnginePCH.h"
#include "Rendering/ConsoleRenderer.h"
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
        assert(false && "Failed to get STD_OUTPUT_HANDLE");
    }

    // Get the current screen resolution in pixels
    SUInt16 MonitorWidthPx = GetSystemMetrics(SM_CXSCREEN);    // Screen width in pixels
    SUInt16 MonitorHeightPx = GetSystemMetrics(SM_CYSCREEN);   // Screen height in pixels

    SBool WasResized = false;                             // Flag to indicate if console size was adjusted

    // Get current console font information
    CONSOLE_FONT_INFO FontInfo;
    GetCurrentConsoleFont(ConsoleOutput, FALSE, &FontInfo);

    // Calculate usable console area with margins:
    // 70% width (leaving 15% margin on each side)
    // 85% height (leaving 7.5% margin top and bottom)
    SUInt16 UsableWidthPx = static_cast<SUInt16>(MonitorWidthPx * 0.70);
    SUInt16 UsableHeightPx = static_cast<SUInt16>(MonitorHeightPx * 0.85);

    // Calculate number of console character cells that fit into usable area
    ScreenWidth = UsableWidthPx / FontInfo.dwFontSize.X;
    ScreenHeight = UsableHeightPx / FontInfo.dwFontSize.Y;

    // Total console buffer cells
    BufferSize = ScreenWidth * ScreenHeight;

    // Define new console buffer size using COORD struct
    COORD NewBufferSize(ScreenWidth, ScreenHeight);

    // Attempt to resize buffer
    if (!SetConsoleScreenBufferSize(ConsoleOutput, NewBufferSize))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleScreenBufferSize failed: %lu"), GetLastError());
        assert(false && "SetConsoleScreenBufferSize failed");
    }

    // Define window rectangle to match buffer dimensions (inclusive coordinates)
    SMALL_RECT WindowRectangle(0, 0, ScreenWidth - 1, ScreenHeight - 1);
    if (!SetConsoleWindowInfo(ConsoleOutput, TRUE, &WindowRectangle))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleWindowInfo failed: %lu"), GetLastError());  
        assert(false && "SetConsoleWindowInfo failed");
    }

    // Get HWND for console window to manipulate position
    HWND ConsoleWindow = GetConsoleWindow();
    if (ConsoleWindow == nullptr)
    {
        // Log null handle
        S_LOG_ERROR(LogConsoleRenderer, TEXT("GetConsoleWindow returned NULL"));  
        assert(false && "GetConsoleWindow returned NULL");
    }

    // Obtain current window rectangle (screen coordinates)
    if (!GetWindowRect(ConsoleWindow, &ConsoleWindowRectangle))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("GetWindowRect failed: %lu"), GetLastError());
        assert(false && "GetWindowRect failed");
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
    ScreenBuffer = std::make_unique<CHAR_INFO[]>(BufferSize);

    // Clear buffer contents (custom method)
    ClearBuffer();

    // Create a new console buffer
    ConsoleScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    if (ConsoleScreenBuffer == nullptr)
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("CreateConsoleScreenBuffer failed: %lu"), GetLastError());
        assert(false && "CreateConsoleScreenBuffer failed");
    }

    // Activate the newly created console screen buffer
    if (!SetConsoleActiveScreenBuffer(ConsoleScreenBuffer))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("SetConsoleActiveScreenBuffer failed: %lu"), GetLastError());
        assert(false && "SetConsoleActiveScreenBuffer failed");
    }

    // Get Current Background Color from the DefaultEngine.ini file
    CurrentBackgroundColor = SIniFileManager::GetInstance()->GetConfigFile(DefaultEngineConfig)->GetValueFromKey<WORD>(TEXT("[Solitaire.ConsoleStyle]"), TEXT("BackgroundColor"));
    
    // Initialize each CHAR_INFO element in the screen buffer
    for (SUInt16 Index = 0; Index < BufferSize; Index++)
    {
        // Set the character to a space ' ' to clear the visible content,
        // and apply the current background color as the attribute to maintain consistent background.
        ScreenBuffer[Index].Char.UnicodeChar = TEXT(' ');
        ScreenBuffer[Index].Attributes = CurrentBackgroundColor;
    }
    
    // Set the background color
    if (!SetBackgroundColor(SGridPositionU32(0, 0), ScreenWidth * ScreenHeight, CurrentBackgroundColor))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("FillConsoleOutputAttribute failed (BackgroundColor = %d): %lu"), CurrentBackgroundColor, GetLastError());
        assert(false && "FillConsoleOutputAttribute failed");
    }

    // Hide cursor
    if (!ShowConsolCursor(false))
    {
        // Log failure code
        S_LOG_ERROR(LogConsoleRenderer, TEXT("ShowConsoleCursor failed: %lu"), GetLastError());
        assert(false && "ShowConsoleCursor failed");
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
        // Set each cell to an empty wide character and default background
        ScreenBuffer[Index].Char.UnicodeChar = TEXT(' ');
        ScreenBuffer[Index].Attributes = GetCurrentBackgroundColor();
    }
}

void SConsoleRenderer::ClearBufferAt(const SGridPositionU32& GridPosition, SSize Size)
{
    // Create TempGridPosition to iterate from the starting point
    SGridPositionU32 TempGridPosition = GridPosition;

    while (Size > 0)
    {
        // Check if we've exceeded the screen height
        if (TempGridPosition.second >= ScreenHeight)
            break;

        // If we've reached the end of a line, wrap to the next line
        if (TempGridPosition.first >= ScreenWidth)
        {
            TempGridPosition.first = 0;
            TempGridPosition.second++;
        }

        // Calculate the linear index in the screen buffer
        SUInt32 Index = ValidateWriteBounds(TempGridPosition, 1);

        // Clear the character at the current position:
        // - Set character to a space ' '
        // - Apply the current background color as the attribute
        ScreenBuffer[Index].Char.UnicodeChar = TEXT(' ');
        ScreenBuffer[Index].Attributes = GetCurrentBackgroundColor();

        // Move to the next horizontal cell
        TempGridPosition.first++;
        Size--;
    }
}

void SConsoleRenderer::Write(const SGridPositionU32& GridPosition, const SWString& Text, const SUInt32 Length,SBool bRespectBorder, WORD Color)
{
    WriteInternal(GridPosition, Text, Length, bRespectBorder, [&](SUInt32 Index){ return Color; });
}

void SConsoleRenderer::Draw()
{
    // Draw the console border
    DrawBorder();

    // Set the size of the CHAR_INFO buffer being written (width x height)
    COORD BufferSize_COORD = COORD(ScreenWidth, ScreenHeight);

    // Set the top-left origin within the buffer (usually (0, 0))
    COORD Buffer_COORD = COORD(0, 0);

    // Define the target rectangle in the console where the buffer will be rendered
    // It starts at (0, 0) and ends at (ScreenWidth - 1, ScreenHeight - 1)
    SMALL_RECT WriteRegion = SMALL_RECT(0, 0, ScreenWidth - 1, ScreenHeight - 1);

    // Write the contents of the screen buffer to the actual console output
    WriteConsoleOutputW(ConsoleScreenBuffer, ScreenBuffer.get(), BufferSize_COORD, Buffer_COORD, &WriteRegion);
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
    Write(GridPosition, HorizontalPanel, static_cast<SUInt32>(HorizontalPanel.size()), false, Color);
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
        Write(SGridPositionU32(GridPosition.first, GridPosition.second + Index), Character, static_cast<SUInt32>(Character.size()), false, Color);
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