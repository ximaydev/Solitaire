#include "SolitaireEnginePCH.h"
#include "Audio/AudioMaster.h"
#include "Config/IniFile.h"
#include "Config/IniFileManager.h"

SAudioMaster::SAudioMaster(IXAudio2* AudioEngine)
{
	// Attempt to create the mastering voice on the provided audio engine.
	HRESULT Result = AudioEngine->CreateMasteringVoice(&MasteringVoice);
	if (FAILED(Result))
	{
		// Log an error with detailed information if creating the mastering voice fails.
		S_LOG_ERROR(LogAudio, TEXT("Failed to create mastering voice on audio engine. HRESULT: 0x%08X"), Result);
		MasteringVoice = nullptr;

		// Terminate the application since the mastering voice is critical for audio functionality.
		assert(false && "Failed to create mastering voice on audio engine");
	}
	else
	{
		// Log success when the mastering voice is created.
		S_LOG(LogAudio, TEXT("Successfully created mastering voice on audio engine."));
	}

	// Get the default audio configuration file.
	SSharedPtr<SIniFile> DefaultAudioConfigFile = SIniFileManager::GetInstance()->GetConfigFile(DefaultAudioConfig);

	if (DefaultAudioConfigFile.get())
	{
		// If the config file exists, try to load the master volume.
		if (SFloat LoadedVolume = DefaultAudioConfigFile->GetValueFromKey<SFloat>(SWString(TEXT("[AudioSettings]")), SWString(TEXT("MasterVolume"))))
		{
			// Set the master volume to the loaded value from the config file.
			SetMasterVolume(LoadedVolume);
			MasterVolume = LoadedVolume;
		}
	}
	else
	{
		// If the config file doesn't exist, set the master volume to the default value.
		SetMasterVolume(MasterVolume);
	}

	// Log the setting of the master volume.
	S_LOG(LogAudio, TEXT("Master volume set to: %.2f"), MasterVolume);
}

SAudioMaster::~SAudioMaster()
{
	// Log a message indicating the start of the destruction process for the mastering voice.
	if (MasteringVoice)
	{
		// Safely destroy the mastering voice if it was created.
		MasteringVoice->DestroyVoice();
		MasteringVoice = nullptr;

		// Log after destroying the mastering voice.
		S_LOG(LogAudio, TEXT("Mastering voice successfully destroyed."));
	}
	else
	{
		// Log if the mastering voice was not created and there is nothing to destroy.
		S_LOG_WARNING(LogAudio, TEXT("No mastering voice to destroy."));
	}
}

void SAudioMaster::SetMasterVolume(float NewMasterVolume)
{
	// Store the new master volume level.
	MasterVolume = NewMasterVolume;

	// Check if the MasteringVoice isn't nullptr
	if (MasteringVoice)
	{
		// Apply the volume to the mastering voice.
		HRESULT Result = MasteringVoice->SetVolume(MasterVolume);
		if (FAILED(Result))
		{
			// Log an error with detailed information if setting the volume fails.
			S_LOG_ERROR(LogAudio, TEXT("Failed to set Master Volume to %.2f. HRESULT: 0x%08X"), MasterVolume, Result);
		}
	}
}