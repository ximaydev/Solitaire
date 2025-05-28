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
	SAConsolePrompt(const SAConsolePrompt& Other);

	/** Operators */
	SAConsolePrompt operator=(const SAConsolePrompt& Other)
	{
		if (this != &Other)
		{
			// Call CopyFrom and perform a deep copy
			CopyFrom(Other);
		}
		return *this;
	}

	/** Renders the prompt text to the buffer */
	virtual void Write() override;

	/** Clears the prompt from the buffer */
	virtual void ClearBuffer() override;

	/** Performs a deep copy of all owned data from 'other' into this object. */
	void CopyFrom(const SAActor& Other) override;

	/** Performs a deep copy of the current object using the copy constructor. */
	virtual SSharedPtr<SAActor> Clone() const override { return SSharedPtr<SAActor>(new SAConsolePrompt(*this)); }

	/** Get Text To Show */
	inline const SWString& GetTextToShow() const { return TextToShow; }

	/** Set text to show */
	inline void SetTextToShow(const SWString& NewTextToShow) { TextToShow = NewTextToShow; }

	/** Set Text Color */
	inline void SetTextColor(const WORD NewTextColor) { TextColor = NewTextColor; }

	/** Set ConsolePromptCallback */
	void SetConsolePromptCallback(SConsoleLineCommitted NewConsolePromptCallback);

protected:
	/** Static prompt text to show */
	SWString TextToShow;

	/** Text color attribute */
	WORD TextColor = FG_WHITE;

	/** Callback called when player press enter (will want to finish typing */
	SConsoleLineCommitted ConsolePromptCallback;
};