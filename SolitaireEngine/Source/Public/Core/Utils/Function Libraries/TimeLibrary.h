#pragma once
#include "Core/Utils/Function Libraries/FunctionLibrary.h"

class SOLITAIRE_ENGINE_API TimeLibrary : public FunctionLibrary
{
public:
	static void GetCurrentTimeAsString(const SString& Format, SString& OutString);
	static void GetCurrentTimeAsString(const SWString& Format, SWString& OutString);
};