#pragma once
#include "Core/Utils/Function Libraries/FunctionLibrary.h"

class SOLITAIRE_ENGINE_API StringLibrary : public FunctionLibrary
{
public:
	static SWString StringToWideString(const SString& String);
	static SWString StringToWideString(SString&& String);

	static SString  WideStringToString(const SWString& String);
	static SString  WideStringToString(SWString&& String);
};