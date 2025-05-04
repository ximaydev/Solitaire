#pragma once
#include "SolitaireEnginePCH.h"

/**
These are various log categories defined as constexpr std::wstring_view
The `constexpr` ensures that these string views are evaluated at compile time, 
meaning they are fast and do not incur runtime overhead. These string views 
point to string literals and can be used for logging messages with specific categories.
*/

constexpr std::wstring_view LogTemp = TEXT("LogTemp");
constexpr std::wstring_view LogFileSystem = TEXT("LogFileSystem");
constexpr std::wstring_view LogConfig = TEXT("LogConfig");