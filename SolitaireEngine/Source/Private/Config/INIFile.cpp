#include "SolitaireEnginePCH.h"
#include "Config/IniFile.h"

void SIniFile::GetAllSections(SVector<SWString>& OutResult)
{
    // Clear the output vector to ensure it's empty before filling
    OutResult.clear();

    // Reserve memory to avoid unnecessary reallocations
    OutResult.reserve(ConfigFileMap.size());

    // Iterate over all sections and collect their names (keys)
    for (const auto& Section : ConfigFileMap)
    {
        OutResult.push_back(Section.first);
    }
}

void SIniFile::GetLastSection(SWString& OutResult)
{
    SUInt32 maxIndex = 0;

    // Assuming the key has the format "Score_<number>"
    const SWString Prefix = TEXT("[Score_");  // Prefix including opening bracket

    // Iterate through all keys in the configuration map
    for (const auto& [key, value] : ConfigFileMap)
    {
        // Check if the current key starts with the expected prefix
        if (key.compare(0, Prefix.size(), Prefix) == 0)
        {
            // Extract the numeric part after the prefix
            SWString NumberPart = key.substr(Prefix.size());

            // Convert the numeric substring to an integer
            SUInt32 Index = SStringLibrary::Convert<SUInt32>(NumberPart);

            // Update maxIndex if this index is greater than the current maximum
            if (Index > maxIndex)
            {
                maxIndex = Index;
            }
        }
    }

    // Construct the name of the last section using the highest index found
    OutResult = Prefix + std::to_wstring(maxIndex) + TEXT("]");
}