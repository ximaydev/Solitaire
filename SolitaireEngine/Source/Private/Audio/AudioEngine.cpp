#include "SolitaireEnginePCH.h"
#include "Audio/AudioEngine.h"
#include "Audio/AudioMaster.h"
#include "Audio/WAVLoader.h"
#include "Asset/AssetRegistry.h"
#include "Config/IniFile.h"
#include "Config/IniFileManager.h"

SAudioEngine::SAudioEngine()
{
	//Intialize AudioEngine
	if (Initialize())
	{
		S_LOG(LogAudio, TEXT("AudioEngine initialized successfully."));
	}
}

bool SAudioEngine::Initialize()
{
	//Initialize COM
	HRESULT CoResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(CoResult))
	{
		// Exit the application if COM initialization fails
		S_LOG_ERROR(LogAudio, TEXT("COM initialization failed for Audio Engine. HRESULT: 0x%08X"), CoResult);
		assert(false && "COM initialization failed for Audio Engine.");
	}

	// Try to create the XAudio2 engine with the default processor.
	HRESULT Result = XAudio2Create(&AudioEngine, NULL, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(Result))
	{
		// If creation fails, log the error and terminate the application.
		S_LOG_ERROR(LogAudio, TEXT("Failed to create XAudio2 engine with the default processor. HRESULT: 0x%08X. Application will be terminated."), Result);
		assert(false && "Failed to create XAudio2 engine with the default processor.");;
	}

	// Create the master voice wrapper.
	Master = std::make_unique<SAudioMaster>(AudioEngine);

	// Set DefaultAudioConfigFile
	DefaultAudioConfigFile = SIniFileManager::GetInstance()->GetConfigFile(DefaultAudioConfig);

	return true;
}

void SAudioEngine::Shutdown()
{
	// Log shutdown initiation
	S_LOG(LogAudio, TEXT("Audio Engine shutdown initiated."));

	// Destroy all submix voices before shutting down.
	for (auto& [GroupName, Submix] : Submixes)
	{
		// Log each submix voice destruction
		S_LOG(LogAudio, TEXT("Destroying submix voice for group: %s"), GroupName.c_str());
		Submix->DestroyVoice();
	}

	// Clear maps holding submixes and group volumes.
	S_LOG(LogAudio, TEXT("Clearing submixes and group volumes."));
	Submixes.clear();
	GroupVolumes.clear();

	// Release the XAudio2 engine.
	if (AudioEngine)
	{
		// Log the release of the XAudio2 engine
		S_LOG(LogAudio, TEXT("Releasing XAudio2 engine."));
		AudioEngine->Release();
		AudioEngine = nullptr;
	}

	// Log shutdown completion
	S_LOG(LogAudio, TEXT("Audio Engine shutdown completed."));
}

void SAudioEngine::SetVolume(const SWString& GroupName, SFloat NewVolume)
{
	// Clamp the volume value between 0.0 and 1.0.
	NewVolume = std::clamp(NewVolume, 0.0f, 1.0f);

	// Log the volume change for the group
	S_LOG(LogAudio, TEXT("Setting volume for group '%s' to %.2f"), GroupName.c_str(), NewVolume);

	// Save the new volume value for the group.
	GroupVolumes[GroupName] = NewVolume;

	// Apply the volume to the corresponding submix (create if needed).
	if (IXAudio2SubmixVoice* Submix = GetOrCreateSubmix(GroupName))
	{
		// Log the volume application to the submix
		S_LOG(LogAudio, TEXT("Applying volume %.2f to submix for group '%s'"), (NewVolume * Master->GetMasterVolume()), GroupName.c_str());

		// Apply the volume
		Submix->SetVolume(NewVolume * Master->GetMasterVolume());
		GroupVolumes[GroupName] = NewVolume;
	}
}

void SAudioEngine::SetMasterVolume(SFloat NewMasterVolume)
{
	// Clamp the master volume to the range [0.0, 1.0]
	NewMasterVolume = std::clamp(NewMasterVolume, 0.0f, 1.0f);

	// Log the change of master volume
	S_LOG(LogAudio, TEXT("Setting master volume to %.2f"), NewMasterVolume);

	// Update the master volume.
	Master->SetMasterVolume(NewMasterVolume);

	// Update each submix with the new master volume applied.
	//TODO Sprawdziæ czy to ma sens, master mo¿e automatycznie zmniejszaæ ich g³oœnoœæ
	for (const auto& [GroupName, Submix] : Submixes)
	{
		// Log the application of the master volume to each submix
		S_LOG(LogAudio, TEXT("Updating volume for group '%s' with master volume %.2f"), GroupName.c_str(), NewMasterVolume);

		Submix->SetVolume(GroupVolumes[GroupName] * NewMasterVolume);
	}
}


SFloat SAudioEngine::GetVolume(const SWString& GroupName)
{
	// Return the stored volume for the specified group.
	return GroupVolumes.find(GroupName)->second;
}

// Function to play a sound given a file path
void SAudioEngine::PlaySound(const SWString& FilePath, const SWString& SoundName, const SWString& GroupName)
{
	// Load WAV file from the provided file path into WAVFile 
	SWavFile* WAVFile = SAssetRegistry::GetInstance()->GetAsset<SWavFile>(FilePath);

	//Check if the WAVFile isn't valid
	if (!WAVFile->IsValid)
	{
		// Log a warning if the WAV file is not valid. Exit early if the WAV file is invalid
		S_LOG_WARNING(LogAudio, TEXT("WAV file '%s' isn't valid or couldn't be loaded. Sound will not be played."), FilePath.c_str());
		return;
	}

	// Prepare XAUDIO2_BUFFER, which contains the actual audio data
	XAUDIO2_BUFFER Buffer = {};
	Buffer.AudioBytes = static_cast<SUInt32>(WAVFile->Data.size());  // Set the number of audio bytes
	Buffer.pAudioData = WAVFile->Data.data();						 // Pointer to the audio data in the WAV file

	PlaySound(WAVFile, SoundName, Buffer, GroupName);
}

void SAudioEngine::PlaySound(const SWavFile* WAVFile, const SWString& SoundName, XAUDIO2_BUFFER& Buffer, const SWString& GroupName)
{
	// Build WAVEFORMATEX structure, which defines the audio format
	WAVEFORMATEX Format = {};
	Format.wFormatTag = WAVFile->AudioFormat;		// Audio format (e.g., PCM)
	Format.nChannels = WAVFile->NumChannels;		// Number of channels (stereo or mono)
	Format.nSamplesPerSec = WAVFile->SampleRate;	// Sample rate (e.g., 44100 Hz)
	Format.wBitsPerSample = WAVFile->BitsPerSample; // Bits per sample (e.g., 16 bits)
	Format.nBlockAlign = WAVFile->BlockAlign;		// Block alignment (audio data block size)
	Format.nAvgBytesPerSec = WAVFile->ByteRate;		// Average bytes per second (data rate)
	Format.cbSize = WAVFile->cbSize;				// Size of extra data in WAVEFORMATEX (if any)

	// Create the source voice, which will be used to play the sound
	IXAudio2SourceVoice* SourceVoice = nullptr;

	//Initialize sends structure
	XAUDIO2_VOICE_SENDS Send = {};
	Send.SendCount = 1; 								  // We only have one output (MasterVoice)
	XAUDIO2_SEND_DESCRIPTOR SendDesc = { 0 };		 	  // Initialize send descriptor to zero
	SendDesc.pOutputVoice = GetOrCreateSubmix(GroupName); // Set the output voice
	Send.pSends = &SendDesc;  							  // Point to the send descriptor

	// Create source voice with specified format
	HRESULT CreateSourceVoiceResult = AudioEngine->CreateSourceVoice(&SourceVoice, &Format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &Send);
	if (FAILED(CreateSourceVoiceResult))
	{
		// Log an error if creating the source voice fails
		S_LOG_ERROR(LogAudio, TEXT("Failed to create source voice with HRESULT: 0x%08X."), CreateSourceVoiceResult);
		return;
	}

	// Submit the buffer to the source voice and start playback
	HRESULT SourceVoiceResult = SourceVoice->SubmitSourceBuffer(&Buffer);
	if (FAILED(SourceVoiceResult))
	{
		// Log an error if submitting the buffer fails
		S_LOG_ERROR(LogAudio, TEXT("Failed to submit source buffer to source voice. HRESULT: 0x%08X"), SourceVoiceResult);
		return;
	}

	// Keep reference to this voice
	ActiveVoices[SoundName] = SourceVoice;

	// Log the sound start
	S_LOG(LogAudio, TEXT("Playing sound for group '%s'."), GroupName.c_str());

	// Start playback of the sound
	SourceVoice->Start();
}

void SAudioEngine::StopSound(const SWString& SoundName)
{
	// Try to find the sound in the map of active voices
	auto Iterator = ActiveVoices.find(SoundName);
	if (Iterator != ActiveVoices.end())
	{
		// Destroy the source voice
		Iterator->second->DestroyVoice();

		// Remove the entry from the ActiveVoices map
		ActiveVoices.erase(Iterator);
	}
	else
	{
		S_LOG_ERROR(LogAudio, TEXT("Couldn't find the sound in list: %s"), SoundName.c_str());
	}
}

void SAudioEngine::StopAllSounds()
{
	// Iterate through all active voices and destroy each one
	for (auto& Element : ActiveVoices)
	{
		// Stop and destroy the audio voice
		Element.second->DestroyVoice();
	}

	// Clear the map to remove all references to the stopped voices
	ActiveVoices.clear();
}

IXAudio2SubmixVoice* SAudioEngine::GetOrCreateSubmix(const SWString& GroupName, SUInt32 Channels, SUInt32 SampleRate)
{
	// Try to find an existing submix for the given group.
	auto Iterator = Submixes.find(GroupName);
	if (Iterator != Submixes.end())
	{
		// Log when the submix is found, indicating reuse.
		S_LOG(LogAudio, TEXT("Submix for group '%s' already exists. Reusing existing submix."), GroupName.c_str());
		return Iterator->second;
	}

	// Create a new submix voice if one does not exist.
	IXAudio2SubmixVoice* NewSubmix = nullptr;
	HRESULT Result = AudioEngine->CreateSubmixVoice(&NewSubmix, Channels, SampleRate);
	if (FAILED(Result))
	{
		// Log a detailed error if submix creation fails.
		S_LOG_ERROR(LogAudio, TEXT("Failed to create submix voice for group '%s' (Channels: %d, Sample Rate: %d). HRESULT: 0x%08X"), GroupName.c_str(), Channels, SampleRate, Result);
		return nullptr;
	}

	// Log the successful creation of a new submix.
	S_LOG(LogAudio, TEXT("Successfully created submix for group '%s' with Channels: %d, Sample Rate: %d."), GroupName.c_str(), Channels, SampleRate);

	//Try to get the volume from the .ini file
	if (DefaultAudioConfigFile)
	{
		//Load Value
		float LoadedVolume = DefaultAudioConfigFile->GetValueFromKey<SFloat>(SWString(TEXT("[AudioSettings]")), GroupName);

		// Apply the volume to the submix, factoring in the master volume.
		NewSubmix->SetVolume(LoadedVolume * Master->GetMasterVolume());

		// Store the newly created submix for future use.
		Submixes[GroupName] = NewSubmix;

		return NewSubmix;
	}

	// Set a default base volume for the new submix.
	SFloat BaseVolume = 1.0f;
	GroupVolumes[GroupName] = BaseVolume;

	return NewSubmix;

}