#include "SolitaireEnginePCH.h"

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
		S_LOG_ERROR(LogAudio, TEXT("Failed to initialize COM. HRESULT: 0x%08X"), CoResult);
		exit(1);
	}

	// Try to create the XAudio2 engine with the default processor.
	HRESULT Result = XAudio2Create(&AudioEngine, NULL, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(Result))
	{
		// If creation fails, log the error and terminate the application.
		S_LOG_ERROR(LogAudio, TEXT("Failed to create XAudio2 engine. Terminating application. HRESULT: 0x%08X"), Result);
		exit(1);
	}

	// Create the master voice wrapper.
	Master = std::make_unique<SAudioMaster>(AudioEngine);
	return true;
}

void SAudioEngine::Shutdown()
{
	// Destroy all submix voices before shutting down.
	for (auto& [GroupName, Submix] : Submixes)
	{
		Submix->DestroyVoice();
	}

	// Clear maps holding submixes and group volumes.
	Submixes.clear();
	GroupVolumes.clear();

	// Release the XAudio2 engine.
	if (AudioEngine)
	{
		AudioEngine->Release();
		AudioEngine = nullptr;
	}
}

void SAudioEngine::SetVolume(const SWString& GroupName, SFloat NewVolume)
{
	// Clamp the volume value between 0.0 and 1.0.
	NewVolume = std::clamp(NewVolume, 0.0f, 1.0f);

	// Save the new volume value for the group.
	GroupVolumes[GroupName] = NewVolume;

	// Apply the volume to the corresponding submix (create if needed).
	if (IXAudio2SubmixVoice* Submix = GetOrCreateSubmix(GroupName))
	{
		Submix->SetVolume(NewVolume * Master->GetMasterVolume());
		GroupVolumes[GroupName] = NewVolume;
	}
}

void SAudioEngine::SetMasterVolume(SFloat NewMasterVolume)
{
	// Update the master volume.
	Master->SetMasterVolume(NewMasterVolume);

	// Update each submix with the new master volume applied.
	for (const auto& [GroupName, Submix] : Submixes)
	{
		Submix->SetVolume(GroupVolumes[GroupName] * NewMasterVolume);
	}
}

SFloat SAudioEngine::GetVolume(const SWString& GroupName)
{
	// Return the stored volume for the specified group.
	return GroupVolumes.find(GroupName)->second;
}

// Function to play a sound given a file path
void SAudioEngine::PlaySound(const SWString& FilePath, const SWString& GroupName)
{
	// Load WAV file from the provided file path into WAVFile 
	SWAVFile* WAVFile = SAssetRegistry::GetInstance()->GetAsset<SWAVFile>(FilePath);

	//Check if the WAVFile isn't valid
	if (!WAVFile->IsValid)
	{
		// Log a warning if the WAV file is not valid.
		S_LOG_WARNING(LogAudio, TEXT("WAVFile isn't valid, we won't play sound."));
	}

	// Prepare XAUDIO2_BUFFER, which contains the actual audio data
	XAUDIO2_BUFFER Buffer = {};
	Buffer.AudioBytes = WAVFile->Data.size();  // Set the number of audio bytes
	Buffer.pAudioData = WAVFile->Data.data();  // Pointer to the audio data in the WAV file

	//Play Sound
	PlaySound(WAVFile, Buffer, GroupName);
}

void SAudioEngine::PlaySound(const SWAVFile* WAVFile, XAUDIO2_BUFFER& Buffer, const SWString& GroupName)
{
	// 2. Build WAVEFORMATEX structure, which defines the audio format
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
	XAUDIO2_VOICE_SENDS send = {};
	send.SendCount = 1; 								  // We only have one output (MasterVoice)
	XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };		 	  // Initialize send descriptor to zero
	sendDesc.pOutputVoice = GetOrCreateSubmix(GroupName); // Set the output voice
	send.pSends = &sendDesc;  							  // Point to the send descriptor

	// Create source voice with specified format
	HRESULT CreateSourceVoiceResult = AudioEngine->CreateSourceVoice(&SourceVoice, &Format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &send);
	if (FAILED(CreateSourceVoiceResult))
	{
		// Log an error if creating the source voice fails
		S_LOG_ERROR(LogAudio, TEXT("Failed to create source voice. HRESULT: 0x%08X"), CreateSourceVoiceResult);
		return;
	}

	// Submit the buffer to the source voice and start playback
	HRESULT SourecVoiceResult = SourceVoice->SubmitSourceBuffer(&Buffer);
	if (FAILED(SourecVoiceResult))
	{
		// Log an error if submitting the buffer fails
		S_LOG_ERROR(LogAudio, TEXT("Failed to submit source buffer. HRESULT: 0x%08X"), SourecVoiceResult);
		return;
	}

	// Start playback of the sound
	SourceVoice->Start();
}

IXAudio2SubmixVoice* SAudioEngine::GetOrCreateSubmix(const SWString& GroupName, SUInt32 Channels, SUInt32 SampleRate)
{
	// Try to find an existing submix for the given group.
	auto Iterator = Submixes.find(GroupName);
	if (Iterator != Submixes.end())
	{
		return Iterator->second;
	}

	// Create a new submix voice if one does not exist.
	IXAudio2SubmixVoice* NewSubmix = nullptr;
	HRESULT Result = AudioEngine->CreateSubmixVoice(&NewSubmix, Channels, SampleRate);
	if (FAILED(Result))
	{
		// Log a detailed error if submix creation fails.
		S_LOG_ERROR(LogAudio, TEXT("Could not create submix voice (Channels: %d, Sample Rate: %d). HRESULT: 0x%08X"), Channels, SampleRate, Result);
		return nullptr;
	}

	// Set a default base volume for the new submix.
	SFloat BaseVolume = 1.0f;
	GroupVolumes[GroupName] = BaseVolume;

	// Apply the volume to the submix, factoring in the master volume.
	NewSubmix->SetVolume(BaseVolume * Master->GetMasterVolume());

	// Store the newly created submix for future use.
	Submixes[GroupName] = NewSubmix;

	return NewSubmix;
}