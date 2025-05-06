#include <Windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "Shlwapi.lib")

namespace fs = std::filesystem;

// Directory names
#define GENERATED_DIR TEXT("Generated")
#define CONTENT_DIR TEXT("Content")

// Read allowed extensions
void ReadAllowedExtensions(const fs::path& Path, std::vector<std::string>& OutExtensions)
{
    // Open file as wide stream
    std::ifstream FileReader(Path);

    // Check open success
    if (!FileReader)
    {
        std::wcerr << TEXT("Cannot open extensions file\n");
    }

    std::string Line;

    // Read first line
    while (std::getline(FileReader, Line))
    {
        // Skip lines starting with '#'
        if (Line.empty() || Line.find_first_of('#') != std::string::npos)
        {
            // Skip this line and move to the next one
            continue;  
        }

        std::stringstream StringStream(Line);
        std::string Extension;

        // Split by comma
        while (std::getline(StringStream, Extension, ','))
        {
            // Add to list
            OutExtensions.push_back(Extension);
        }
    }
}

// Collect files with allowed extensions
void CollectAssets(const fs::path& Path, const std::vector<std::string>& Extensions, std::vector<std::wstring>& OutList)
{
    for (auto& entry : fs::recursive_directory_iterator(Path))
    {
        // Skip non-files
        if (!entry.is_regular_file()) continue;

        // Get extension
        std::string Extension = entry.path().extension().string(); 

        if (std::find(Extensions.begin(), Extensions.end(), Extension) != Extensions.end())
        {
            // Get relative path
            std::wstring RelativePath = entry.path().lexically_relative(Path).wstring(); 

            // Add to list
            OutList.push_back(RelativePath);
        }
    }
}

// Write list to file
void WriteList(const fs::path& Path, const std::vector<std::wstring>& List)
{
    // Open output file
    std::wofstream FileWriter(Path);             

    // Check if it's sopen
    if (!FileWriter) {
        std::wcerr << TEXT("Cannot write asset list\n");
        return;
    }
    for (auto& w : List) 
    {
        // Write each path
        FileWriter << w << TEXT("\n");
    }

    //Close file
    FileWriter.close();
}

int main() 
{
    // Buffer for exe path
    wchar_t ExeBuffer[MAX_PATH];

    // Buffer for Solitaire Game directory
    wchar_t RootBuffer[MAX_PATH];

    // Get full exe path
    GetModuleFileNameW(NULL, ExeBuffer, MAX_PATH);

    // Set RootBuffer
    PathCombineW(RootBuffer, ExeBuffer, TEXT("..\\..\\..\\..\\SolitaireGame"));
    
    // Set root path
    fs::path Root = RootBuffer;

    // Set Content path
    fs::path Content = Root / CONTENT_DIR;

    // Set Generated path
    fs::path Generated = Root / GENERATED_DIR;

    // Set Extension File path
    fs::path ExtensionFile = Root.parent_path() / TEXT("SolitaireEngine") / TEXT("Tools") / TEXT("AssetListGenerator") / TEXT("AllowedExtensions.txt"); 

    // Ensure that Generated directory exists
    if (!fs::exists(Generated))
    {
        fs::create_directory(Generated);
    }

    /// Load extensions
    std::vector<std::string> Extensions;
    ReadAllowedExtensions(ExtensionFile, Extensions);

    // Gather assets
    std::vector<std::wstring> Assets;
    CollectAssets(Content, Extensions, Assets); 

    // Write AssetList.txt
    WriteList(Generated / TEXT("AssetList.txt"), Assets);

    // Success
    return 0;                                
}