#include "SolitaireEnginePCH.h"

SAudioMaster::SAudioMaster(IXAudio2* AudioEngine)
{
	// Attempt to create the mastering voice on the provided audio engine.
	HRESULT Result = AudioEngine->CreateMasteringVoice(&MasteringVoice);
	if (FAILED(Result))
	{
		S_LOG_ERROR(LogAudio, TEXT("Failed to create mastering voice. HRESULT: 0x%08X"), Result);
		MasteringVoice = nullptr;
	}

	//Set Master Volume
	SetMasterVolume(MasterVolume);
}

SAudioMaster::~SAudioMaster()
{
	// Safely destroy the mastering voice if it was created.
	if (MasteringVoice)
	{
		MasteringVoice->DestroyVoice();
		MasteringVoice = nullptr;
	}
}

void SAudioMaster::SetMasterVolume(float NewMasterVolume)
{
	// Store the new master volume level.
	MasterVolume = NewMasterVolume;

	if (MasteringVoice)
	{
		// Apply the volume to the mastering voice.
		HRESULT Result = MasteringVoice->SetVolume(MasterVolume);
		if (FAILED(Result))
		{
			S_LOG_ERROR(LogAudio, TEXT("Failed to set Master Volume to %.2f. HRESULT: 0x%08X"), MasterVolume, Result);
		}
	}
	else
	{
		// Log a warning if the mastering voice hasn't been created yet.
		S_LOG_WARNING(LogAudio, TEXT("Attempted to set Master Volume to %.2f, but Mastering Voice is null."), MasterVolume);
	}
}