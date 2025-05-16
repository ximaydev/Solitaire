#pragma once
#include "SolitaireEnginePCH.h"

/**
These are various log categories defined as constexpr std::wstring_view
The `constexpr` ensures that these string views are evaluated at compile time, 
meaning they are fast and do not incur runtime overhead. These string views 
point to string literals and can be used for logging messages with specific categories.
*/

constexpr SWStringView LogTemp = TEXT("LogTemp");
constexpr SWStringView LogFileSystem = TEXT("LogFileSystem");
constexpr SWStringView LogConfig = TEXT("LogConfig");
constexpr SWStringView LogAudio = TEXT("LogAudio");
constexpr SWStringView LogInputSystem = TEXT("LogInputSystem");
constexpr SWStringView LogSolitaireEngine = TEXT("LogSolitaireEngine");
constexpr SWStringView LogConsoleRenderer = TEXT("LogConsoleRenderer");
constexpr SWStringView LogCard = TEXT("LogCard");