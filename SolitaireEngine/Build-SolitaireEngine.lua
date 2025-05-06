-- premake5.lua
-- Configuration for Solitaire Engine shared library (DLL)

project "SolitaireEngine"
    kind "SharedLib"            -- Output type: Dynamic Link Library (DLL)
    language "C++"              -- Source language
    cppdialect "C++20"          -- Requires C++20 compatible compiler
    systemversion "latest"      -- Use latest Windows SDK version
    staticruntime "Off"         -- Use dynamic CRT (MSVCRT.dll) for runtime
    dependson "AssetListGenerator"

    -- Source files organization
    files { 
        "Source/**.h",          -- All header files
        "Source/**.hpp",        -- All C++ header files
        "Source/**.cpp"          -- All implementation files
    }

    -- Include directories (public/private separation)
    includedirs { 
        "Source",               -- Base source directory
        "Source/Public"         -- Public API headers (exported interfaces)
    }

    -- Output directory configuration
    targetdir "Binaries/%{cfg.buildcfg}"      -- Final DLL output location
    implibdir "Binaries/%{cfg.buildcfg}"      -- Import library (.lib) location
    objdir "Binaries/Intermediates/%{cfg.buildcfg}"  -- Intermediate files directory

    -- Engine-specific preprocessor definitions
    defines {
        "SOLITAIRE_ENGINE_BUILD",  -- DLL export/import control macro
    }

    prebuildcommands {
        "echo Executing AssetListGenerator tool...",
        "\"Binaries/%{cfg.buildcfg}/AssetListGenerator.exe\""
    }

    -- Debug configuration settings
    filter "configurations:Debug"
        defines { 
            "DEBUG",            -- Debug mode flag
            "LOG_LEVEL=3"       -- Verbose logging level (3 = maximum)
        }
        symbols "On"            -- Generate debug symbols (PDB files)
        optimize "Debug"        -- Minimal optimizations for debugging
        targetsuffix "_d"       -- Debug build suffix
        
    -- Release configuration settings
    filter "configurations:Release"
        defines { 
            "RELEASE",          -- Release mode flag
            "LOG_LEVEL=1"        -- Minimal logging (1 = errors only)
        }
        optimize "Speed"        -- Maximum performance optimization
        symbols "Off"           -- No debug symbols in release builds

    -- Windows/Visual Studio specific settings
    filter { "system:windows", "action:vs*" }
        buildoptions {
            "/permissive-",     -- Enforce strict standards compliance
            "/Zc:__cplusplus",  -- Report accurate C++ standard version
            "/MP",              -- Enable parallel compilation
            "/std:c++20"        -- Force to use C++20
        }
        disablewarnings {
            "4251",  -- Suppress DLL-interface warnings (exported classes)
            "4275",  -- Suppress non-DLL-interface base class warnings
            "26812"  -- Ignore enum class preference warnings
        }
        flags {
            "MultiProcessorCompile",  -- Force multi-core compilation
            "FatalLinkWarnings"       -- Treat linker warnings as errors
        }

    -- Precompiled header configuration
    pchheader "SolitaireEnginePCH.h"                -- PCH header file
    pchsource "Source/Private/SolitaireEnginePCH.cpp"  -- PCH source implementation