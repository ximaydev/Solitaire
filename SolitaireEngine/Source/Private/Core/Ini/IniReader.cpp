#include "SolitaireEnginePCH.h"

void SIniReader::LoadFromFile(const SWString& FileName, SIniConfigMap& ConfigFileMap)
{
    // Check if the file name ends with the '.ini' extension
    if (!FileName.ends_with(TEXT(".ini")))
    {
        S_LOG_ERROR(LogConfig, TEXT("Invalid file format: '%s' is not an .ini file, or it may be corrupted."), FileName.c_str());
        return;
    }

    // String to hold each line from the file
    SWString Line;

    // Attempt to open the file
    SWFileReader FileReader(Core::Paths::GetProjectConfigPath() + FileName, std::ios::in);

    // Verify that the file was successfully opened
    if (!FileReader.is_open())
    {
        S_LOG_ERROR(LogConfig, TEXT("Failed to open the INI file '%s'. The file may not exist or there could be a permissions issue."), FileName.c_str());
        return;
    }

    // List of prefixes that denote a comment line
    constexpr SArray<SWStringView, 2> CommentPrefixes = { TEXT(";"), TEXT("##") };

    // Characters used to denote the start and end of a section
    constexpr SWideChar SectionStart = TEXT('[');
    constexpr SWideChar SectionEnd   = TEXT(']');

    // Counter for keeping track of the current line number
    SUInt32 LineCounter = 1;

    // Currently active section name
    SWString CurrentSection;

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
            if (CurrentSection.empty() || CurrentSection == TEXT("[]"))
            {
                // Clear the current section name as it's invalid
                CurrentSection.clear();

                // Log a warning with the line number and the invalid section name
                S_LOG_WARNING(LogConfig, TEXT("Invalid or empty section name found at line %d. Skipping section: '%s'."), LineCounter, CurrentSection.c_str());

                // Increment the line counter and skip this section
                LineCounter++;
                continue;
            }

            // Insert the section into the config map if it doesn't exist
            ConfigFileMap.try_emplace(CurrentSection);
        }
        // Section has only one bracket or is malformed
        else if (StartPosition != SString::npos || EndPosition != SString::npos)
        {
            // Log a warning with line number and the malformed section
            S_LOG_WARNING(LogConfig, TEXT("Malformed section at line %d. Section has unmatched brackets or invalid format: %s"), LineCounter, Line.c_str());

            // Clear the current section name since it’s invalid
            CurrentSection.clear();
        }
        // Handle key-value pair lines inside a valid section
        else if (!CurrentSection.empty())
        {
            // Attempt to locate the first key-value separator character
            // Valid separators include '=' or ':'
            SSize SeparatorPos = Line.find_first_of(TEXT("=:"));

            // Proceed only if a separator was found
            if (SeparatorPos != SString::npos)
            {
                // Extract the key (everything before the separator)
                SWString Key = Line.substr(0, SeparatorPos);

                // Extract the value (everything after the separator)
                SWString Value = Line.substr(SeparatorPos + 1);

                // Check for an empty key and log a warning
                if (Key.empty())
                {
                    // Log a warning with the line number and specify that the key is missing
                    S_LOG_WARNING(LogConfig, TEXT("Missing key at line %d. The entry will be skipped."), LineCounter);
                    LineCounter++;
                    continue;
                }

                // Check for an empty value and log a warning
                if (Value.empty())
                {
                    // Log a warning with the line number and specify that the value is missing
                    S_LOG_WARNING(LogConfig, TEXT("Missing value for key '%s' at line %d. The entry will be skipped."), Key.c_str(), LineCounter);
                    LineCounter++;
                    continue;
                }

                // Store the key-value pair in the current section of the configuration map
                ConfigFileMap[CurrentSection].try_emplace(std::move(Key), std::move(Value));
            }
            else
            {
                // If no valid separator was found, log an invalid format warning with details
                S_LOG_WARNING(LogConfig, TEXT("Invalid key-value format at line %d. The line '%s' will be skipped due to missing or invalid separator."), LineCounter, Line.c_str());
            }
        }
        else
        {
            // Log a warning if the section is empty, indicating the issue with the current line
            S_LOG_WARNING(LogConfig, TEXT("Empty section at line %d. The line '%s' will be skipped."), LineCounter, Line.c_str());
        }

        //Increment Line Counter
        LineCounter++;
    }
}