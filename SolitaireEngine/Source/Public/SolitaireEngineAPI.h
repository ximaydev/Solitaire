#pragma once

#ifdef SOLITAIRE_ENGINE_BUILD
	#define SOLITAIRE_ENGINE_API __declspec(dllexport)
#else
	#define SOLITAIRE_ENGINE_API __declspec(dllimport)
#endif