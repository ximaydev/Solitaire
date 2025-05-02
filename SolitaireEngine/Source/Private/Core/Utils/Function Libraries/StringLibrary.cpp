#include "SolitaireEnginePCH.h"
#include "Core/Utils/Function Libraries/StringLibrary.h"
#include <codecvt>

SWString StringLibrary::StringToWideString(SString& String)
{
	SInt32 Length = static_cast<SInt32>(String.size());
	SInt32 SizeNeeded = MultiByteToWideChar(CP_UTF8, 0, String.c_str(), Length, nullptr, 0);
	if (SizeNeeded == 0) return TEXT("");

	SWString WideString;
	WideString.resize(SizeNeeded);
	MultiByteToWideChar(CP_UTF8, 0, String.c_str(), Length, &WideString[0], SizeNeeded);
	return WideString;
}

SWString StringLibrary::StringToWideString(SString&& String)
{
	return StringToWideString(String);
}

SString StringLibrary::WideStringToString(SWString& String)
{
	SInt32 Length = static_cast<SInt32>(String.size());
	SInt32 SizeNeeded = WideCharToMultiByte(CP_UTF8, 0, String.c_str(), Length, nullptr, 0, nullptr, nullptr);
	if (SizeNeeded == 0) return "";

	SString NarrowString;
	NarrowString.resize(SizeNeeded);
	WideCharToMultiByte(CP_UTF8, 0, String.c_str(), Length, &NarrowString[0], SizeNeeded, nullptr, nullptr);

	return NarrowString;
}

SString StringLibrary::WideStringToString(SWString&& String)
{
	return WideStringToString(String);
}
