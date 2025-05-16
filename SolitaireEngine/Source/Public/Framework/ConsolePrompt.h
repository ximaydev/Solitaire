#pragma once
#include "Globals.h"
#include "Framework/Actor.h"

class SOLITAIRE_ENGINE_API SConsolePrompt : public SAActor
{
public:
	/** Constructor */
	SConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow, const SConsoleLineCommitted& NewCallback);
	SConsolePrompt(const SGridPositionU32& NewGridPosition, WORD NewTextColor, const SWString& NewTextToShow);
	SConsolePrompt(const SGridPositionU32& NewGridPosition);

	/** Get Text To Show */
	inline const SWString& GetTextToShow() const { return TextToShow; }

	/** Set text to show */
	inline void SetTextToShow(const SWString& NewTextToShow) { TextToShow = NewTextToShow; }

	/** Set Text Color */
	inline void SetTextColor(const WORD NewTextColor) { TextColor = NewTextColor; }

	/** Initializes the console prompt */
	bool Initialize();

	/** Renders the prompt text to the buffer */
	virtual void Write() override;

	/** Clears the prompt from the buffer */
	virtual void ClearBuffer() override;

protected:
	/** Static prompt text to show */
	SWString TextToShow;

	/** Text color attribute */
	WORD TextColor = FG_WHITE;

	/** Callback called when player press enter (will want to finish typing */
	SConsoleLineCommitted ConsolePromptCallback;
};