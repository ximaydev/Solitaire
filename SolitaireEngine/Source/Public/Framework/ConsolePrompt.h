#pragma once
#include "Globals.h"
#include "Framework/Actor.h"

class SOLITAIRE_ENGINE_API SAConsolePrompt : public SAActor
{
public:
	/** Constructor */
	SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, WORD NewTextColor, const SWString& NewTextToShow, const SConsoleLineCommitted& NewCallback);
	SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld, WORD NewTextColor, const SWString& NewTextToShow);
	SAConsolePrompt(const SGridPositionU32& NewGridPosition, SSharedPtr<SWorld> NewWorld);

	/** Get Text To Show */
	inline const SWString& GetTextToShow() const { return TextToShow; }

	/** Set text to show */
	inline void SetTextToShow(const SWString& NewTextToShow) { TextToShow = NewTextToShow; }

	/** Set Text Color */
	inline void SetTextColor(const WORD NewTextColor) { TextColor = NewTextColor; }

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