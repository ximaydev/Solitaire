#pragma once
#include "Globals.h"

class SOLITAIRE_ENGINE_API SINIFile
{
    friend class SINIFileManager;

public:
	/** Get typed value from the configuration map using a section name and key name. */
	template<typename T>
	T GetValueFromKey(const SString& SectionName, const SString& KeyName) const;

#if DEBUG
    /** This function prints out the loaded.ini file's contents */
    void PrintLoadedFile() const;
#endif

protected:
	/** Holds the contents of the INI file as a two-level map: Section -> (Key -> Value). */
	SINIConfigMap ConfigFileMap = {};
};

template<typename T>
T SINIFile::GetValueFromKey(const SString& SectionName, const SString& KeyName) const
{
    // Try to find the section in the configuration map
    auto sectionIt = ConfigFileMap.find(SectionName);
    if (sectionIt != ConfigFileMap.end())
    {
        // If section exists, try to find the key within that section
        auto keyIt = sectionIt->second.find(KeyName);
        if (keyIt != sectionIt->second.end())
        {
            // If key is found, convert the associated string value to the requested type
            return SStringLibrary::Convert<T>(keyIt->second);
        }
    }

    // Return default value of T if section or key is not found
    return T();
}