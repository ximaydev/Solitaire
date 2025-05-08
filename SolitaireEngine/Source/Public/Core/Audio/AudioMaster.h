#pragma once
#include "Globals.h"
#include <xaudio2.h>

/** Class responsible for handling the master voice and global volume control. */
class SOLITAIRE_ENGINE_API SAudioMaster
{
public:
    /** Constructors */
    SAudioMaster() = delete;
    SAudioMaster(IXAudio2* AudioEngine);

    /** Destructor */
    virtual ~SAudioMaster();

    /** Sets the master volume for all audio. */
    void SetMasterVolume(float NewMasterVolume);

    /** Gets the current master volume. */
    inline float GetMasterVolume() const { return MasterVolume; }

    /** Get Mastering Voice */
    inline IXAudio2MasteringVoice* GetMasteringVoice() const { return MasteringVoice; }

protected:
    /** Pointer to the mastering voice. */
    IXAudio2MasteringVoice* MasteringVoice = {};

    /** Master volume level. */
    float MasterVolume = 0.5f;
};