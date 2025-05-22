#include "SolitaireEnginePCH.h"

void SStringLibrary::GetCurrentTimeAsString(const SString& Format, SString& OutString)
{
	// Clear the output string to ensure it's empty before usage
	OutString.clear();

	// Get the current time as a time_t object
	time_t Time = std::time(nullptr);

	// Convert the time to local time using std::localtime
	tm LocalTime = *std::localtime(&Time);

	// Get current time (for milliseconds)
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	// Create a stringstream to format the time according to the provided format
	SOStringStream StringStream;

	// Use std::put_time to format the time into a string according to the given format
	StringStream << std::put_time(&LocalTime, Format.c_str()) << "_" << std::setfill('0') << std::setw(3) << ms.count();

	// Assign the formatted string to the output string
	OutString = StringStream.str();

	// If the output string is empty, set it to "Invalid"
	if (OutString.empty())
	{
		OutString = "Invalid";
	}
}

void SStringLibrary::GetCurrentTimeAsString(const SWString& Format, SWString& OutString)
{
	// Clear the output string to ensure it's empty before usage
	OutString.clear();

	// Get the current time as a time_t object
	time_t Time = std::time(nullptr);

	// Convert the time to local time using std::localtime
	tm LocalTime = *std::localtime(&Time);

	// Get current time (for milliseconds)
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	// Create a stringstream to format the time according to the provided format
	SWOStringStream StringStream;

	// Use std::put_time to format the time into a string according to the given format
	StringStream << std::put_time(&LocalTime, Format.c_str()) << TEXT("_") << std::setfill<SWideChar>(TEXT('0')) << std::setw(3) << ms.count();

	// Assign the formatted string to the output string
	OutString = StringStream.str();

	// If the output string is empty, set it to "Invalid"
	if (OutString.empty())
	{
		OutString = TEXT("Invalid");
	}
}

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