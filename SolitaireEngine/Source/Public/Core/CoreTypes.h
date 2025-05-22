#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <utility>
#include <filesystem>
#include <array>
#include <functional>
#include <span>

/** Forward Declarations */
struct FCallbackRecord;

// =============================================
// Primitive Types
// =============================================

using SInt8 = std::int8_t;
using SInt16 = std::int16_t;
using SInt32 = std::int32_t;
using SInt64 = std::int64_t;

using SUInt8 = std::uint8_t;
using SUInt16 = std::uint16_t;
using SUInt32 = std::uint32_t;
using SUInt64 = std::uint64_t;

using SFloat = float;
using SDouble = double;
using SLDouble = long double;

using SChar = char;
using SUChar = unsigned char;
using SWideChar = wchar_t;

using SBool = bool;
using SSize = std::size_t;

// =============================================
// String Types
// =============================================

template<typename CharT, typename Traits, typename Allocator>
using SBasicString = std::basic_string<CharT, Traits, Allocator>;

using SString = std::string;
using SStringView = std::string_view;
using SWString = std::wstring;
using SWStringView = std::wstring_view;

using SOStringStream = std::ostringstream;
using SWOStringStream = std::wostringstream;

using SIStringStream = std::istringstream;
using SWIStringStream = std::wistringstream;

// =============================================
// Memory Management
// =============================================

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
// File System
// =============================================

namespace fs = std::filesystem;

using SPath = std::filesystem::path;
using SFileWriter = std::ofstream;
using SWFileWriter = std::wofstream;
using SFileReader = std::ifstream;
using SWFileReader = std::wifstream;

// =============================================
// Containers
// =============================================

template<typename T>
using SVector = std::vector<T>;

template<typename T>
using SQueue = std::queue<T>;

template<typename Key, typename Value>
using SMap = std::map<Key, Value>;

template<typename Key, typename Value>
using SUnorderedMap = std::unordered_map<Key, Value>;

template<typename T1, typename T2>
using SPair = std::pair<T1, T2>;

template<typename T, SUInt64 Size>
using SArray = std::array<T, Size>;

template<typename T>
using SInitializerList = std::initializer_list<T>;

template<typename T, SSize Size>
using SSpan = std::span<T, Size>;

// =============================================
// Rendering
// =============================================

// Represents a 2D grid coordinate: first = X, second = Y (x, y)
template<typename T1, typename T2>
using SGridPosition = std::pair<T1, T2>;

using SGridPositionU32 = SGridPosition<SUInt32, SUInt32>;

// =============================================
// Specialized Aliases
// =============================================

using SIniSectionMap = SUnorderedMap<SWString, SWString>;
using SIniConfigMap = SUnorderedMap<SWString, SIniSectionMap>;
using SCallback = std::function<void()>;
using SCallbackRecords = SVector<FCallbackRecord>;
using SConsoleLineCommitted = std::function<void(SWString&)>;