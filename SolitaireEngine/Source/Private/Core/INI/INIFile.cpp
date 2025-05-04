#include "SolitaireEnginePCH.h"
#include "Core/INI/INIFile.h"

#if DEBUG
void SINIFile::PrintLoadedFile() const
{
    // Iterate through each section in the ConfigFileMap
    for (const auto& Section : ConfigFileMap)
    {
        // Print the name of the section (e.g., [General], [Settings], etc.)
        std::cout << Section.first << '\n';

        // Iterate through each key-value pair within the section
        for (const auto& [Key, Value] : Section.second)
        {
            // Print the key and value in the format: key = value
            std::cout << "   " << Key << " = " << Value << '\n';
        }
    }
}
#endif