#pragma once
#include <Windows.h>

namespace ConsoleColor
{
    constexpr WORD FG_BLACK = 0;
    constexpr WORD BG_BLACK = 0;

    constexpr WORD FG_BLUE          = FOREGROUND_BLUE;                                                               // 0x0001 = 1
    constexpr WORD FG_GREEN         = FOREGROUND_GREEN;                                                              // 0x0002 = 2
    constexpr WORD FG_CYAN          = FOREGROUND_BLUE | FOREGROUND_GREEN;                                            // 0x0003 = 3
    constexpr WORD FG_RED           = FOREGROUND_RED;                                                                // 0x0004 = 4
    constexpr WORD FG_PURPLE        = FOREGROUND_BLUE | FOREGROUND_RED;                                              // 0x0005 = 5
    constexpr WORD FG_YELLOW        = FOREGROUND_GREEN | FOREGROUND_RED;                                             // 0x0006 = 6
    constexpr WORD FG_GRAY          = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;                           // 0x0007 = 7
    constexpr WORD FG_DARK_GRAY     = FOREGROUND_INTENSITY;                                                          // 0x0008 = 8
    constexpr WORD FG_LIGHT_BLUE    = FOREGROUND_INTENSITY | FOREGROUND_BLUE;                                        // 0x0009 = 9
    constexpr WORD FG_LIGHT_GREEN   = FOREGROUND_INTENSITY | FOREGROUND_GREEN;                                       // 0x000A = 10
    constexpr WORD FG_LIGHT_CYAN    = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;                     // 0x000B = 11
    constexpr WORD FG_LIGHT_RED     = FOREGROUND_INTENSITY | FOREGROUND_RED;                                         // 0x000C = 12
    constexpr WORD FG_LIGHT_MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED;                       // 0x000D = 13
    constexpr WORD FG_LIGHT_YELLOW  = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED;                      // 0x000E = 14
    constexpr WORD FG_WHITE         = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;    // 0x000F = 15

    constexpr WORD BG_BLUE          = BACKGROUND_BLUE;                                                               // 0x0010 = 16
    constexpr WORD BG_GREEN         = BACKGROUND_GREEN;                                                              // 0x0020 = 32
    constexpr WORD BG_CYAN          = BACKGROUND_BLUE | BACKGROUND_GREEN;                                            // 0x0030 = 48
    constexpr WORD BG_RED           = BACKGROUND_RED;                                                                // 0x0040 = 64
    constexpr WORD BG_PURPLE        = BACKGROUND_BLUE | BACKGROUND_RED;                                              // 0x0050 = 80
    constexpr WORD BG_YELLOW        = BACKGROUND_GREEN | BACKGROUND_RED;                                             // 0x0060 = 96
    constexpr WORD BG_GRAY          = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;                           // 0x0070 = 112
    constexpr WORD BG_DARK_GRAY     = BACKGROUND_INTENSITY;                                                          // 0x0080 = 128
    constexpr WORD BG_LIGHT_BLUE    = BACKGROUND_INTENSITY | BACKGROUND_BLUE;                                        // 0x0090 = 144
    constexpr WORD BG_LIGHT_GREEN   = BACKGROUND_INTENSITY | BACKGROUND_GREEN;                                       // 0x00A0 = 160
    constexpr WORD BG_LIGHT_CYAN    = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN;                     // 0x00B0 = 176
    constexpr WORD BG_LIGHT_RED     = BACKGROUND_INTENSITY | BACKGROUND_RED;                                         // 0x00C0 = 192
    constexpr WORD BG_LIGHT_MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED;                       // 0x00D0 = 208
    constexpr WORD BG_LIGHT_YELLOW  = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED;                      // 0x00E0 = 224
    constexpr WORD BG_WHITE         = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;    // 0x00F0 = 240
}