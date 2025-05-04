#include "SolitaireEnginePCH.h"
#include "Core/Utils/Function Libraries/StringLibrary.h"
#include <codecvt>
#include <cwctype>
#include <ctype.h>

SWString SStringLibrary::StringToWideString(const SString& String)
{
	SInt32 Length = static_cast<SInt32>(String.size());
	SInt32 SizeNeeded = MultiByteToWideChar(CP_UTF8, 0, String.c_str(), Length, nullptr, 0);
	if (SizeNeeded == 0) return TEXT("");

	SWString WideString;
	WideString.resize(SizeNeeded);
	MultiByteToWideChar(CP_UTF8, 0, String.c_str(), Length, &WideString[0], SizeNeeded);
	return WideString;
}

SWString SStringLibrary::StringToWideString(SString&& String)
{
	return StringToWideString(String);
}

SString SStringLibrary::WideStringToString(const SWString& String)
{
	SInt32 Length = static_cast<SInt32>(String.size());
	SInt32 SizeNeeded = WideCharToMultiByte(CP_UTF8, 0, String.c_str(), Length, nullptr, 0, nullptr, nullptr);
	if (SizeNeeded == 0) return "";

	SString NarrowString;
	NarrowString.resize(SizeNeeded);
	WideCharToMultiByte(CP_UTF8, 0, String.c_str(), Length, &NarrowString[0], SizeNeeded, nullptr, nullptr);

	return NarrowString;
}

SString SStringLibrary::WideStringToString(SWString&& String)
{
	return WideStringToString(String);
}

 void SStringLibrary::ToLower(SString& String)
{
	for (SChar& Char : String)
	{
		Char = std::tolower(Char);
	}
}

 void SStringLibrary::ToLower(SWString& String)
{
	for (SWideChar& Char : String)
	{
		Char = std::towlower(Char);
	}
}

 void SStringLibrary::ToLower(SString&& String)
{
	for (SChar& Char : String)
	{
		Char = std::tolower(Char);
	}
}

 void SStringLibrary::ToLower(SWString&& String)
{
	for (SWideChar& Char : String)
	{
		Char = std::towlower(Char);
	}
}

 SString SStringLibrary::ToLower(const SString& String)
{
	SString Result;
	Result.reserve(String.size());

	for (const SChar& Char : String)
	{
		Result += std::tolower(Char);
	}

	return Result;
}

 SWString SStringLibrary::ToLower(const SWString& String)
{
	SWString Result;
	Result.reserve(String.size());

	for (const SWideChar& Char : String)
	{
		Result += std::towlower(Char);
	}

	return Result;
}

 void SStringLibrary::ToUpper(SString& String)
{
	for (SChar& Char : String)
	{
		Char = std::toupper(Char);
	}
}

 void SStringLibrary::ToUpper(SWString& String)
{
	for (SWideChar& Char : String)
	{
		Char = std::towupper(Char);
	}
}

 void SStringLibrary::ToUpper(SString&& String)
{
	for (SChar& Char : String)
	{
		Char = std::toupper(Char);
	}
}

 void SStringLibrary::ToUpper(SWString&& String)
{
	for (SWideChar& Char : String)
	{
		Char = std::towupper(Char);
	}
}

 SString SStringLibrary::ToUpper(const SString& String)
{
	SString Result;
	Result.reserve(String.size());

	for (const SChar& Char : String)
	{
		Result += std::toupper(Char);
	}

	return Result;
}

 SWString SStringLibrary::ToUpper(const SWString& String)
{
	SWString Result;
	Result.reserve(String.size());

	for (const SWideChar& Char : String)
	{
		Result += std::towupper(Char);
	}

	return Result;
}
