#pragma once
#include "Core/Utility/FunctionLibrary/FunctionLibrary.h"

class SOLITAIRE_ENGINE_API SStringLibrary : public SFunctionLibrary
{
public:
	static void GetCurrentTimeAsString(const SString& Format, SString& OutString);
	static void GetCurrentTimeAsString(const SWString& Format, SWString& OutString);

	static SWString StringToWideString(const SString& String);
	static SString  WideStringToString(const SWString& String);

	template<typename CharT, typename Traits, typename Allocator>
	inline static void RemoveWhiteSpaces(SBasicString<CharT, Traits, Allocator>& String);

	template<typename CharT, typename Traits, typename Allocator>
	static SBasicString<CharT, Traits, Allocator> ToLower(const SBasicString<CharT, Traits, Allocator>& String);

	template<typename CharT, typename Traits, typename Allocator>
	static void ToLowerInPlace(SBasicString<CharT, Traits, Allocator>& String);

	template<typename CharT, typename Traits, typename Allocator>
	static SBasicString<CharT, Traits, Allocator> ToUpper(const SBasicString<CharT, Traits, Allocator>& String);

	template<typename CharT, typename Traits, typename Allocator>
	static void ToUpperInPlace(SBasicString<CharT, Traits, Allocator>& String);

	template<typename CharT, typename Traits, typename Allocator>
	static inline void GetFileExtension(const SBasicString<CharT, Traits, Allocator>& FilePath, SBasicString<CharT, Traits, Allocator>& OutExtension);

	template<typename T, typename CharT, typename Traits, typename Allocator>
	static T Convert(const SBasicString<CharT, Traits, Allocator>& String);
};

template<typename CharT, typename Traits, typename Allocator>
inline void SStringLibrary::RemoveWhiteSpaces(SBasicString<CharT, Traits, Allocator>& String)
{
	String.erase(std::remove_if(String.begin(), String.end(), [](CharT Char)
		{
			return std::isspace(Char);
		}), String.end());
}

template<typename CharT, typename Traits, typename Allocator>
inline SBasicString<CharT, Traits, Allocator> SStringLibrary::ToLower(const SBasicString<CharT, Traits, Allocator>& String)
{
	SBasicString<CharT, Traits, Allocator> Result = String;
	ToLowerInPlace(Result);
	return Result;
}

template<typename CharT, typename Traits, typename Allocator>
inline void SStringLibrary::ToLowerInPlace(SBasicString<CharT, Traits, Allocator>& String)
{
	std::transform(String.begin(), String.end(), String.begin(),
		[](CharT Char) { return static_cast<CharT>(std::tolower(static_cast<SUChar>(Char))); });
}

template<typename CharT, typename Traits, typename Allocator>
inline SBasicString<CharT, Traits, Allocator> SStringLibrary::ToUpper(const SBasicString<CharT, Traits, Allocator>& String)
{
	SBasicString<CharT, Traits, Allocator> Result = String;
	ToUpperInPlace(Result);
	return Result;
}

template<typename CharT, typename Traits, typename Allocator>
inline void SStringLibrary::ToUpperInPlace(SBasicString<CharT, Traits, Allocator>& String)
{
	std::transform(String.begin(), String.end(), String.begin(),
		[](CharT Char) { return static_cast<CharT>(std::toupper(static_cast<SUChar>(Char))); });
}

template<typename CharT, typename Traits, typename Allocator>
inline void SStringLibrary::GetFileExtension(const SBasicString<CharT, Traits, Allocator>& FilePath, SBasicString<CharT, Traits, Allocator>& OutExtension)
{
	OutExtension.clear();

	SSize Position = FilePath.find_last_of('.');
	if (Position != SBasicString<CharT, Traits, Allocator>::npos)
	{
		OutExtension = FilePath.substr(Position);
	}
}

template<typename T, typename CharT, typename Traits, typename Allocator>
inline T SStringLibrary::Convert(const SBasicString<CharT, Traits, Allocator>& String)
{
	if constexpr (std::is_same_v<T, SBool>)
	{
		SBasicString<CharT, Traits, Allocator> LowerString = ToLower(String);

		if constexpr (std::is_same_v <CharT, SChar>)
		{
			return (LowerString == "true" || LowerString == "1");
		}
		else
		{
			return LowerString == TEXT("true") || LowerString == TEXT("1");
		}
	}
	else if constexpr (std::is_same_v<T, SBasicString<CharT, Traits, Allocator>>)
	{
		return String;
	}
	else if constexpr (std::is_same_v<T, SString>)
	{
		return WideStringToString(String);
	}
	else if constexpr (std::is_same_v<T, SWString>)
	{
		return StringToWideString(String);
	}
	else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
	{
		if constexpr (sizeof(T) <= sizeof(SInt32))
			return static_cast<T>(std::stoi(String));
		else
			return static_cast<T>(std::stoll(String));
	}
	else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
	{
		if constexpr (sizeof(T) <= sizeof(SUInt32))
			return static_cast<T>(std::stoul(String));
		else
			return static_cast<T>(std::stoull(String));
	}
	else if constexpr (std::is_same_v<T, SFloat>)
	{
		SBasicString<CharT, Traits, Allocator> FormattedString = String;
		std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
		return std::stof(FormattedString);
	}
	else if constexpr (std::is_same_v<T, SDouble>)
	{
		SBasicString<CharT, Traits, Allocator> FormattedString = String;
		std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
		return std::stod(FormattedString);
	}
	else if constexpr (std::is_same_v<T, SLDouble>)
	{
		SBasicString<CharT, Traits, Allocator> FormattedString = String;
		std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
		return std::stold(FormattedString);
	}
	else
	{
		static_assert(!sizeof(T), "Unsupported type in Convert function.");
	}
}
