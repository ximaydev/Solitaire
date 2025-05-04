#include "SolitaireEnginePCH.h"
#include "Core/Ini/IniFileWriter.h"

SINIFileWriter::SINIFileWriter(const SWString& FileName)
{
    // Open the file for writing
    IniFileWriter.open(Core::Paths::GProjectConfigPath + TEXT("\\") + FileName, std::ios::out | std::ios::ate);
    if (!IniFileWriter.is_open())
    {
        // Log an error if the file couldn't be opened
        S_LOG_ERROR(LogConfig, TEXT("Failed to open the file %s for IniFileWriter"), FileName.c_str());
    }
}

void SINIFileWriter::AddSection(const SString& SectionName)
{
    IniFileWriter << "\n[" << SectionName << ']' << '\n';  // Write section header
}

void SINIFileWriter::AddPairKeyValue(const SString& Key, const SString& Value)
{
    IniFileWriter << Key << " = " << Value << '\n';  // Write key-value pair
}

void SINIFileWriter::AddComment(const SString& Comment)
{
    IniFileWriter << "##" << Comment << '\n';  // Write comment line
}

void SINIFileWriter::AddSectionWithComment(const SString& SectionName, const SString& Comment)
{
    AddComment(Comment);  // Add comment before section
    AddSection(SectionName);  // Add section
}

void SINIFileWriter::SaveToFile()
{
    IniFileWriter.flush();  // Ensure all data is written to file
    IniFileWriter.close();  // Close the file
}