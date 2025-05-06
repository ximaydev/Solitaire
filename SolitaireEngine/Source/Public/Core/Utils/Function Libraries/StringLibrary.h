#pragma once
#include "Core/Utils/Function Libraries/FunctionLibrary.h"

class SOLITAIRE_ENGINE_API SStringLibrary : public SFunctionLibrary
{
public:
	static SWString StringToWideString(const SString& String);
	static SWString StringToWideString(SString&& String);

	static SString  WideStringToString(const SWString& String);
	static SString  WideStringToString(SWString&& String);

	template<typename CharT, typename Traits, typename Allocator>
	inline static void RemoveWhiteSpaces(SBasicString<CharT, Traits, Allocator>& String);

	static void ToLower(SString& String);
	static void ToLower(SWString& String);
	static void ToLower(SString&& String);
	static void ToLower(SWString&& String);
	static SString ToLower(const SString& String);
	static SWString ToLower(const SWString& String);

	static void ToUpper(SString& String);
	static void ToUpper(SWString& String);
	static void ToUpper(SString&& String);
	static void ToUpper(SWString&& String);
	static SString ToUpper(const SString& String);
	static SWString ToUpper(const SWString& String);

	template<typename CharT, typename Traits, typename Allocator>
	inline static void GetFileExtension(const SBasicString<CharT, Traits, Allocator>& FilePath, SBasicString<CharT, Traits, Allocator>& OutExtension);

	template<typename T>
	static inline T Convert(const SString& String);
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
inline void SStringLibrary::GetFileExtension(const SBasicString<CharT, Traits, Allocator>& FilePath, SBasicString<CharT, Traits, Allocator>& OutExtension)
{
	OutExtension.clear();

	SSize Position = FilePath.find_last_of('.');
	if (Position != SBasicString<CharT, Traits, Allocator>::npos)
	{
		OutExtension = FilePath.substr(Position);
	}
}

template<>
inline bool SStringLibrary::Convert<bool>(const SString& String)
{
	SString LowerString = ToLower(String);
	return (LowerString == "true" || LowerString == "1");
}

template<>
inline SInt8 SStringLibrary::Convert<SInt8>(const SString& String)
{
	return static_cast<SInt8>(std::stoi(String));
}

template<>
inline SInt16 SStringLibrary::Convert<SInt16>(const SString& String)
{
	return static_cast<SInt16>(std::stoi(String));
}

template<>
inline SInt32 SStringLibrary::Convert<SInt32>(const SString& String)
{
	return static_cast<SInt32>(std::stoi(String));
}

template<>
inline SInt64 SStringLibrary::Convert<SInt64>(const SString& String)
{
	return static_cast<SInt64>(std::stoll(String));
}

template<>
inline SUInt8 SStringLibrary::Convert<SUInt8>(const SString& String)
{
	return static_cast<SUInt8>(std::stoi(String));
}

template<>
inline SUInt16 SStringLibrary::Convert<SUInt16>(const SString& String)
{
	return static_cast<SUInt16>(std::stoi(String));
}

template<>
inline SUInt32 SStringLibrary::Convert<SUInt32>(const SString& String)
{
	return static_cast<SUInt32>(std::stoi(String));
}

template<>
inline SUInt64 SStringLibrary::Convert<SUInt64>(const SString& String)
{
	return static_cast<SUInt64>(std::stoull(String));
}

template<>
inline SFloat SStringLibrary::Convert<SFloat>(const SString& String)
{
	SString FormattedString = String;
	std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
	return std::stof(FormattedString);
}

template<>
inline SDouble SStringLibrary::Convert<SDouble>(const SString& String)
{
	SString FormattedString = String;
	std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
	return std::stod(FormattedString);
}

template<>
inline SLDouble SStringLibrary::Convert<SLDouble>(const SString& String)
{
	SString FormattedString = String;
	std::replace(FormattedString.begin(), FormattedString.end(), ',', '.');
	return std::stold(FormattedString);
}