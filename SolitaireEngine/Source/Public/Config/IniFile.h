#pragma once
#include "Globals.h"

/** This class is responsible for accessing values from an INI config file. */
class SOLITAIRE_ENGINE_API SIniFile
{
    friend class SIniFileManager;

public:
	/** Get typed value from the configuration map using a section name and key name. */
	template<typename T>
	T GetValueFromKey(const SWString& SectionName, const SWString& KeyName) const;

#if DEBUG
    /** This function prints out the loaded.ini file's contents */
    void PrintLoadedFile() const;
#endif

protected:
	/** Holds the contents of the INI file as a two-level map: Section -> (Key -> Value). */
	SIniConfigMap ConfigFileMap = {};
};

template<typename T>
T SIniFile::GetValueFromKey(const SWString& SectionName, const SWString& KeyName) const
{
    // Try to find the section in the configuration map
    auto SectionIterator = ConfigFileMap.find(SectionName);
    if (SectionIterator != ConfigFileMap.end())
    {
        // If section exists, try to find the key within that section
        auto KeyIterator = SectionIterator->second.find(KeyName);
        if (KeyIterator != SectionIterator->second.end())
        {
            // If key is found, convert the associated string value to the requested type
            return SStringLibrary::Convert<T>(KeyIterator->second);
        }
        else
        {
            // Log a warning if the key was not found in the section
            S_LOG_WARNING(LogConfig, TEXT("Key '%s' not found in section '%s'"), KeyName.c_str(), SectionName.c_str());
        }
    }
    else
    {
        // Log a warning if the section was not found
        S_LOG_WARNING(LogConfig, TEXT("Section '%s' not found in the config."), SectionName.c_str());
    }

    // Return default value of T if section or key is not found
    return T();
}