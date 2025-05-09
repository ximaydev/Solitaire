#pragma once
#include "Globals.h"

/**
 * Enum class representing possible key actions within the Solitaire Engine.
 * Used to define and manage keybindings in a structured and type-safe way.
 */
enum class SOLITAIRE_ENGINE_API EKeyAction : SUInt8
{
	MoveUp = 1,
	MoveDown,
	Select,
	Back,
	Pause,
	Max
};
