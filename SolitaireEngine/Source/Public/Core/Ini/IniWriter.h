#pragma once
#include "Globals.h"

/**
 * SIniFileWriter is a utility class designed for writing INI configuration files. It allows users to create and manage INI files with sections, key-value pairs, and comments.
 * The class provides methods for adding sections, key-value pairs, and comments, and it allows saving the entire structure to a file.
 */
class SOLITAIRE_ENGINE_API SIniFileWriter
{
public:
    /** Constructors */
    SIniFileWriter() = delete;
    explicit SIniFileWriter(const SWString& FileName);

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