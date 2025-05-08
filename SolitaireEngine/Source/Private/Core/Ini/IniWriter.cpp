#include "SolitaireEnginePCH.h"

SIniWriter::SIniWriter(const SWString& FileName)
{
    // Open the file for writing, with appending mode
    IniFileWriter.open(Core::Paths::GetProjectConfigPath() + FileName, std::ios::out | std::ios::ate);

    // Check if the file was successfully opened
    if (!IniFileWriter.is_open())
    {
        // Log an error with the file name if it couldn't be opened for writing
        S_LOG_ERROR(LogConfig, TEXT("Failed to open the file '%s' for writing. Ensure the file exists or the path is valid."), FileName.c_str());
    }
}

void SIniWriter::AddSection(const SString& SectionName)
{
    IniFileWriter << "\n[" << SectionName << ']' << '\n';  // Write section header
}

void SIniWriter::AddPairKeyValue(const SString& Key, const SString& Value)
{
    IniFileWriter << Key << " = " << Value << '\n';  // Write key-value pair
}

void SIniWriter::AddComment(const SString& Comment)
{
    IniFileWriter << "##" << Comment << '\n';  // Write comment line
}

void SIniWriter::AddSectionWithComment(const SString& SectionName, const SString& Comment)
{
    AddComment(Comment);  // Add comment before section
    AddSection(SectionName);  // Add section
}

void SIniWriter::SaveToFile()
{
    IniFileWriter.flush();  // Ensure all data is written to file
    IniFileWriter.close();  // Close the file
}