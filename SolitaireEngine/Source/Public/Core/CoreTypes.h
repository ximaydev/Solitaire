#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <condition_variable>
#include <fstream>
#include <queue>
#include <map>
#include <unordered_map>
#include <optional>
#include <filesystem>

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

template<typename CharT, typename Traits, typename Allocator>
using SBasicString = std::basic_string<CharT, Traits, Allocator>;

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

template<typename T>
using SOptional = std::optional<T>;

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

using SPath = std::filesystem::path;

using SFileWriter = std::ofstream;
using SWFileWriter = std::wofstream;
using SFileReader = std::ifstream;
using SWFileReader = std::wifstream;
using SStringStream = std::ostringstream;
using SWStringStream = std::wostringstream;

// =============================================
// Containers
// =============================================

template<typename T>
using SQueue = std::queue<T>;

template<typename Key, typename Value>
using SMap = std::map<Key, Value>;

template<typename Key, typename Value>
using SUnorderedMap = std::unordered_map<Key, Value>;

using SINISectionMap = SUnorderedMap<SString, SString>;
using SINIConfigMap = SUnorderedMap<SString, SINISectionMap>;

template<typename T>
using SVector = std::vector<T>;

template<typename T1, typename T2>
using SPair = std::pair<T1, T2>;