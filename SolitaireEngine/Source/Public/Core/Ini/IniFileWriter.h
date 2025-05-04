#pragma once
#include "Globals.h"

class SOLITAIRE_ENGINE_API SINIFileWriter
{
public:
    /** Constructors */
    SINIFileWriter() = delete;
    explicit SINIFileWriter(const SWString& FileName);

    /** Adds a new section to the INI file. */
    void AddSection(const SString& SectionName);

    /** Adds a key-value pair to the current section in the INI file. */
    void AddPairKeyValue(const SString& Key, const SString& Value);

    /** Adds a comment to the INI file. */
    void AddComment(const SString& Comment);

    /** Adds a section with an optional comment to the INI file. */
    void AddSectionWithComment(const SString& SectionName, const SString& Comment);

    /** Saves the INI file. */
    void SaveToFile();

protected:
    SFileWriter IniFileWriter = {};
};