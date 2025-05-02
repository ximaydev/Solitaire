-- premake5.lua
-- Configuration for Solitaire Game executable

-- Main project configuration
project "SolitaireGame"
    kind "ConsoleApp"            -- Output type: Windows console application
    language "C++"               -- Source language
    cppdialect "C++20"           -- Requires C++20 compatible compiler (VS2019+)
    systemversion "latest"       -- Use latest Windows SDK version
    dependson "SolitaireEngine"  -- Build dependency on engine DLL
    
    -- Source files to include in the project
    files { 
        "Source/**.h",           -- All header files in Source/ directory
        "Source/**.hpp",         -- All C++ header files
        "Source/**.cpp"          -- All implementation files
    }
    
    -- Include directories for header resolution
    includedirs { 
        "Source",                -- Game-specific headers
        "../SolitaireEngine/Source/Public"  -- Engine public API headers
    }

    -- Output directory configuration
    targetdir "Binaries/%{cfg.buildcfg}"       -- Final executable output
    objdir "Binaries/Intermediates/%{cfg.buildcfg}"  -- Intermediate files directory

    -- Library search paths
    libdirs { "Binaries/%{cfg.buildcfg}" }    -- Location of engine .lib files
    
    -- Library dependencies
    links { 
        "SolitaireEngine"        -- Link against engine library (Debug/Release config must match)
    }
    
    -- Pre-build commands (copy required DLLs)
    prebuildcommands {
        "{COPY} ../SolitaireEngine/Binaries/%{cfg.buildcfg}/SolitaireEngine*.dll %{cfg.targetdir}"
    }
    
    -- Debug configuration settings
    filter "configurations:Debug"
        defines {
            "DEBUG",             -- Debug mode flag
            "LOG_LEVEL=3"       -- Verbose logging level (3 = maximum)
        }
        symbols "On"             -- Generate debug symbols
        optimize "Debug"         -- Safe optimization level
        targetsuffix "_d"        -- Debug build suffix
        
    -- Release configuration settings
    filter "configurations:Release"
        defines { 
            "RELEASE",            -- Release mode flag
            "LOG_LEVEL=1"        -- Minimal logging (1 = errors only)
        }
        optimize "Speed"        -- Maximum performance optimization
        symbols "Off"           -- No debug symbols in release
        flags { "LinkTimeOptimization" }  -- Enable LTO for better optimization
        
    -- Visual Studio specific compiler settings
    filter "action:vs*"
        buildoptions {
            "/permissive-",     -- Enforce standards compliance
            "/Zc:__cplusplus",  -- Report accurate C++ standard version
            "/MP"               -- Enable multi-processor compilation
        }
        disablewarnings {
            "26812",           -- Ignore enum class preference warning
            "26495"            -- Suppress uninitialized variable warnings
        }
        
    -- Precompiled header configuration
    pchheader "SolitaireGamePCH.h"         -- Precompiled header file
    pchsource "Source/SolitaireGamePCH.cpp"  -- Source file for PCH generation