-- premake5.lua
-- Configuration for Solitaire Engine shared library (DLL)

project "AssetListGenerator"
    kind "ConsoleApp"           -- Output type: Dynamic Link Library (DLL)
    language "C++"              -- Source language
    cppdialect "C++20"          -- Requires C++20 compatible compiler
    systemversion "latest"      -- Use latest Windows SDK version

    -- Source files organization
    files { 
        "Source/**.h",          -- All header files
        "Source/**.hpp",        -- All C++ header files
        "Source/**.cpp"         -- All implementation files
    }

    -- Include directories (public/private separation)
    includedirs { 
        "Source",               -- Base source directory
    }

    -- Output directory configuration
    targetdir "../..//Binaries/%{cfg.buildcfg}"      -- Final DLL output location
    objdir "../..//Binaries/Intermediates/%{cfg.buildcfg}"  -- Intermediate files directory
    
    -- Debug configuration settings
    filter "configurations:Debug"
        defines { 
            "DEBUG"             -- Debug mode flag
        }
        symbols "On"            -- Generate debug symbols (PDB files)
        optimize "Debug"        -- Minimal optimizations for debugging
        
    -- Release configuration settings
    filter "configurations:Release"
        defines { 
            "RELEASE"           -- Release mode flag
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