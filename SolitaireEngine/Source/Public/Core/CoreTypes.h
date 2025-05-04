#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <condition_variable>
#include <fstream>
#include <queue>

// =============================================
// Primitive Types
// =============================================

// Integer Types
using SInt8 = std::int8_t;
using SInt16 = std::int16_t;
using SInt32 = std::int32_t;
using SInt64 = std::int64_t;
using SUInt8 = std::uint8_t;
using SUInt16 = std::uint16_t;
using SUInt32 = std::uint32_t;
using SUInt64 = std::uint64_t;

// Floating-Point Types
using SFloat = float;
using SDouble = double;
using SLDouble = long double;

// Character Types
using SChar = char;
using SUChar = unsigned char;
using SWideChar = wchar_t;

// Boolean Type
using SBool = bool;

// =============================================
// String Types
// =============================================
using SString = std::string;
using SStringView = std::string_view;
using SWString = std::wstring;
using SWStringView = std::wstring_view;

// =============================================
// Memory Management
// =============================================
using SSize = std::size_t;

// Smart Pointers
template<typename T>
using SUniquePtr = std::unique_ptr<T>;

template<typename T>
using SSharedPtr = std::shared_ptr<T>;

template<typename T>
using SWeakPtr = std::weak_ptr<T>;

// =============================================
// Concurrency and Threading
// =============================================
using SMutex = std::mutex;
using SConditionVariable = std::condition_variable;
using SThread = std::thread;

template<typename T>
using SAtomic = std::atomic<T>;

// =============================================
// File Operations
// =============================================
using SFileWriter = std::ofstream;
using SWFileWriter = std::wofstream;
using SStringStream = std::ostringstream;
using SWStringStream = std::wostringstream;
// =============================================
// Containers
// =============================================
template<typename T>
using SQueue = std::queue<T>;