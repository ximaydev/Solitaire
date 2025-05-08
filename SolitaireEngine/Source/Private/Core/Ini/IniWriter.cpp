#include "SolitaireEnginePCH.h"

SIniFileWriter::SIniFileWriter(const SWString& FileName)
{
    // Open the file for writing, with appending mode
    IniFileWriter.open(Core::Paths::GProjectConfigPath + TEXT("\\") + FileName, std::ios::out | std::ios::ate);

    // Check if the file was successfully opened
    if (!IniFileWriter.is_open())
    {
        // Log an error with the file name if it couldn't be opened for writing
        S_LOG_ERROR(LogConfig, TEXT("Failed to open the file '%s' for writing. Ensure the file exists or the path is valid."), FileName.c_str());
    }
}

void SIniFileWriter::AddSection(const SString& SectionName)
{
    IniFileWriter << "\n[" << SectionName << ']' << '\n';  // Write section header
}

void SIniFileWriter::AddPairKeyValue(const SString& Key, const SString& Value)
{
    IniFileWriter << Key << " = " << Value << '\n';  // Write key-value pair
}

void SIniFileWriter::AddComment(const SString& Comment)
{
    IniFileWriter << "##" << Comment << '\n';  // Write comment line
}

void SIniFileWriter::AddSectionWithComment(const SString& SectionName, const SString& Comment)
{
    AddComment(Comment);  // Add comment before section
    AddSection(SectionName);  // Add section
}

void SIniFileWriter::SaveToFile()
{
    IniFileWriter.flush();  // Ensure all data is written to file
    IniFileWriter.close();  // Close the file
}