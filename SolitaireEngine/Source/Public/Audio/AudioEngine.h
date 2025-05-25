#pragma once
#include "Globals.h"

/** Forward Declarations */
class SWavFile;
class SAudioMaster;
class SIniFile;

/** Main audio engine class responsible for initializing and managing the audio system. */
class SOLITAIRE_ENGINE_API SAudioEngine
{
public:
    /** Constructor for audio engine. */
    SAudioEngine();   

    /** Sets the volume for a specific audio group. */
    void SetVolume(const SWString& GroupName, SFloat NewVolume);

    /** Gets the volume level for a specific audio group. */
    SFloat GetVolume(const SWString& GroupName);

    /** Sets the volume for Master. */
    void SetMasterVolume(SFloat NewMasterVolume);

    /** Plays a sound from a specific group. */
    void PlaySound(const SWString& FilePath, const SWString& SoundName, const SWString& GroupName);
    void PlaySound(const SWavFile* WAVFile, const SWString& SoundName, XAUDIO2_BUFFER& Buffer, const SWString& GroupName);

    /** Stops a specific sound by name if it's currently playing. */
    void StopSound(const SWString& SoundName);

    /** Stops and removes all currently playing sounds. */
    void StopAllSounds();

    /** Cleans up and shuts down the audio engine. */
    virtual void Shutdown();

    /** Pointer to the master volume controller. */
    SUniquePtr<SAudioMaster> Master = nullptr;

    /** Cached pointer to the DefaultAudio.ini file */
    SSharedPtr<SIniFile> DefaultAudioConfigFile = nullptr;

protected:
    /** Initializes XAudio2 and creates the master voice. */
    virtual bool Initialize();

    // Ensure we have a submix voice for this group
    IXAudio2SubmixVoice* GetOrCreateSubmix(const SWString& GroupName, SUInt32 Channels = 2, SUInt32 SampleRate = 48000);

    /** Pointer to the main XAudio2 engine instance. */
    IXAudio2* AudioEngine = nullptr;

    /** Map of submix voices for grouping different types of audio (e.g., Music, SFX). */
    SUnorderedMap<SWString, IXAudio2SubmixVoice*> Submixes;

    /** Volume levels for each audio group. */
    SUnorderedMap<SWString, SFloat> GroupVolumes;

    /** Map storing active sound voices by name for playback control. */
    SUnorderedMap<SWString, IXAudio2SourceVoice*> ActiveVoices;
};
