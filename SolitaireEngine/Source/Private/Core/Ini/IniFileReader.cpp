#include "SolitaireEnginePCH.h"
#include "Core/Ini/IniFileReader.h"

void SINIFileReader::LoadFromFile(const SWString& FileName, SINIConfigMap& ConfigFileMap)
{
    // Check if the file name ends with the '.ini' extension
    if (!FileName.ends_with(TEXT(".ini")))
    {
        S_LOG_ERROR(LogConfig, TEXT("Invalid file format: '%s' is not an .ini file or may be corrupted."), FileName.c_str());
        return;
    }

    // String to hold each line from the file
    SString Line;

    // Attempt to open the file using the UTF-8 encoding
    SFileReader FileReader(Core::Paths::GProjectConfigPath + TEXT("\\") + FileName, std::ios::in);

    // Verify that the file was successfully opened
    if (!FileReader.is_open())
    {
        S_LOG_ERROR(LogConfig, TEXT("Failed to open file '%s'."), FileName.c_str());
        return;
    }

    // List of prefixes that denote a comment line
    constexpr std::array<std::string_view, 2> CommentPrefixes = { ";", "##" };

    // Characters used to denote the start and end of a section
    constexpr char SectionStart = '[';
    constexpr char SectionEnd = ']';

    // Counter for keeping track of the current line number
    SUInt32 LineCounter = 1;

    // Currently active section name
    SString CurrentSection;

    // Process the file line by line
    while (std::getline(FileReader, Line))
    {
        // Trim leading and trailing whitespace
        SStringLibrary::RemoveWhiteSpaces(Line);

        // Skip lines that are empty
        if (Line.empty())
        {
            LineCounter++;
            continue;
        }

        // Skip lines that start with a known comment prefix
        if (Line.starts_with(CommentPrefixes[0]) || Line.starts_with(CommentPrefixes[1]))
        {
            LineCounter++;
            continue;
        }

        SSize StartPosition = Line.find_first_of(SectionStart);
        SSize EndPosition = Line.find_first_of(SectionEnd);

        // If both section start and end characters are found
        if (StartPosition != SString::npos && EndPosition != SString::npos)
        {
            // Extract section including brackets (e.g., "[SectionName]")
            CurrentSection = Line.substr(StartPosition, EndPosition - StartPosition + 1);

            // Check if the section name is empty or "[]", which means it has no name
            if (CurrentSection.empty() || CurrentSection == "[]")
            {
                CurrentSection.clear();
                S_LOG_WARNING(LogConfig, TEXT("Unnamed section is not allowed at line %d. Section %s will be skipped."), LineCounter, SStringLibrary::StringToWideString(CurrentSection).c_str());
                LineCounter++;
                continue;
            }

            // Insert the section into the config map if it doesn't exist
            ConfigFileMap.try_emplace(CurrentSection);
        }
        // Section has only one bracket or is malformed
        else if (StartPosition != SString::npos || EndPosition != SString::npos)
        {
            S_LOG_WARNING(LogConfig, TEXT("Malformed section at line %d. Skipping: %s"), LineCounter, SStringLibrary::StringToWideString(Line).c_str());
            CurrentSection.clear();
        }
        // Handle key-value pair lines inside a valid section
        else if (!CurrentSection.empty())
        {
            // Attempt to locate the first key-value separator character
            // Valid separators include '=' or ':'
            SSize SeparatorPos = Line.find_first_of("=:");

            // Proceed only if a separator was found
            if (SeparatorPos != SString::npos)
            {
                // Extract the key (everything before the separator)
                SString Key = Line.substr(0, SeparatorPos);

                // Extract the value (everything after the separator)
                SString Value = Line.substr(SeparatorPos + 1);

                // Check for an empty key and log a warning
                if (Key.empty())
                {
                    S_LOG_WARNING(LogConfig, TEXT("Missing key at line %d. Skipping entry."), LineCounter);
                    LineCounter++;
                    continue;
                }

                // Check for an empty value and log a warning
                if (Value.empty())
                {
                    S_LOG_WARNING(LogConfig, TEXT("Missing value at line %d. Skipping entry."), LineCounter);
                    LineCounter++;
                    continue;
                }

                // Store the key-value pair in the current section of the configuration map
                ConfigFileMap[CurrentSection].try_emplace(std::move(Key), std::move(Value));
            }
            else
            {
                // If no valid separator was found, log an invalid format warning
                S_LOG_WARNING(LogConfig, TEXT("Invalid key-value format at line %d. Skipping: %s"), LineCounter, SStringLibrary::StringToWideString(Line).c_str());
            }
        }
        else
        {
            S_LOG_WARNING(LogConfig, TEXT("Section is empty at line %d. Skipping: %s"), LineCounter, SStringLibrary::StringToWideString(Line).c_str());
        }

        //Increment Line Counter
        LineCounter++;
    }
}