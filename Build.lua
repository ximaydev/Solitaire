-- premake5.lua
-- Main workspace configuration
workspace "Solitaire"
    architecture "x64"            -- Target 64-bit architecture
    configurations { "Debug", "Release" } -- Build configurations
    startproject "SolitaireGame"      -- Default project to run/debug
    
    -- Output directory pattern: System-Architecture/BuildConfig
    OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
    
    -- Common settings for all configurations
    defines { "_CRT_SECURE_NO_WARNINGS" } -- Disable CRT warnings
        
    -- Windows platform settings
    filter "system:windows"
        defines {
            "WIN32_LEAN_AND_MEAN", -- Exclude rarely-used Win32 stuff
            "NOMINMAX"            -- Prevent min/max macro conflicts
        }
    
    -- Include project-specific configuration
    include "SolitaireGame/Build-SolitaireGame.lua"
    include "SolitaireEngine/Build-SolitaireEngine.lua"
    include "SolitaireEngine/Tools/AssetListGenerator/Build-SolitaireEngine-AssetListGenerator.lua"