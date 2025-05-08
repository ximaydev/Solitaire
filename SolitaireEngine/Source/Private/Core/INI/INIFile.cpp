#include "SolitaireEnginePCH.h"

#if DEBUG
void SIniFile::PrintLoadedFile() const
{
    // Iterate through each section in the ConfigFileMap
    for (const auto& Section : ConfigFileMap)
    {
        // Print the name of the section (e.g., [General], [Settings], etc.)
        std::wcout << Section.first << TEXT('\n');

        // Iterate through each key-value pair within the section
        for (const auto& [Key, Value] : Section.second)
        {
            // Print the key and value in the format: key = value
            std::wcout << TEXT("   ") << Key << TEXT(" = ") << Value << TEXT('\n');
        }
    }
}
#endif