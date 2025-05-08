#pragma once
#include "Globals.h"
#include "Core/Asset/Asset.h"
#include "Core/Asset/AssetLoader.h"

/** Structure representing a WAV file and its properties. */
class SOLITAIRE_ENGINE_API SWavFile : public SAsset
{
    friend class SWavLoader;
    friend class SAudioEngine;

public:
    /** Get IsValid */
    SBool GetIsValid() const { return IsValid; }

    /** Get AudioFormat */
    SUInt16 GetAudioFormat() const { return AudioFormat; }

    /** Get NumChannels */
    SUInt16 GetNumChannels() const { return NumChannels; }

    /** Get SampleRate */
    SUInt32 GetSampleRate() const { return SampleRate; }

    /** Get ByteRate */
    SUInt32 GetByteRate() const { return ByteRate; }

    /** Get BlockAlign */
    SUInt16 GetBlockAlign() const { return BlockAlign; }

    /** Get BitsPerSample */
    SUInt16 GetBitsPerSample() const { return BitsPerSample; }

    /** Get cbSize */
    SUInt16 GetCbSize() const { return cbSize; }

    /** Get ValidBitsPerSample */
    SUInt16 GetValidBitsPerSample() const { return ValidBitsPerSample; }

    /** Get ChannelMask */
    SUInt32 GetChannelMask() const { return ChannelMask; }

    /** Get SubFormat */
    const SUInt8* GetSubFormat() const { return SubFormat; }

    /** Get TotalSampleFrames */
    SUInt32 GetTotalSampleFrames() const { return TotalSampleFrames; }

    /** Get Data */
    const std::vector<SUInt8>& GetData() const { return Data; }

    /** Clear the asset */
    virtual void ClearAsset() override;

protected:
    SBool IsValid = false;              /** Flag indicating if the WAV file is valid (properly loaded) */
    SUInt16 AudioFormat = 0;            /** Audio format (e.g., 1 = PCM, 17 = IMA ADPCM, 65534 = EXTENSIBLE) */
    SUInt16 NumChannels = 0;            /** Number of audio channels */
    SUInt32 SampleRate = 0;             /** Sampling rate in Hz */
    SUInt32 ByteRate = 0;               /** Byte rate (sampleRate * numChannels * bitsPerSample / 8) */
    SUInt16 BlockAlign = 0;             /** Block alignment (numChannels * bitsPerSample / 8) */
    SUInt16 BitsPerSample = 0;          /** Bit depth (e.g., 8, 16, 24, 32) */
    SUInt16 cbSize = 0;                 /** Size of additional data in 'fmt ' chunk (if present) */
    SUInt16 ValidBitsPerSample = 0;     /** Valid bits per sample for WAVE_FORMAT_EXTENSIBLE */
    SUInt32 ChannelMask = 0;            /** Channel mask for WAVE_FORMAT_EXTENSIBLE */
    SUInt8 SubFormat[16] = { 0 };       /** GUID of the format (for WAVE_FORMAT_EXTENSIBLE) */
    SUInt32 TotalSampleFrames = 0;      /** Total number of audio frames (from 'fact' chunk or calculated) */
    std::vector<SUInt8> Data = {};      /** Audio data (interleaved, as int32_t values) */
};

/** Class responsible for loading and parsing WAV files. */
class SOLITAIRE_ENGINE_API SWavLoader : public SAssetLoader
{
public:
    /** Loads a WAV file from the specified file path into the provided output object. */
    virtual void Load(const SWString& FilePath, SAsset* OutWAVFile) override;

    /** Unloads the Wav file, releasing any associated resources. */
    virtual void UnLoad(SAsset* OutFile) override;

    /** Creates a new WAV file asset. */
    virtual SSharedPtr<SAsset> CreateAsset() override;

    /** Returns the file extension supported by this loader. */
    virtual SWString GetSupportedExtension() override;

protected:
    /** Reads and validates the RIFF header of a WAV file, returning true if valid */
    bool ReadRIFFHeader(SFileReader& File);

    /** Processes a chunk of data in the WAV file and identifies the type of chunk (fmt, data, fact, etc.) */
    bool ProcessChunk(SFileReader& File, SWavFile* OutWAVFile, bool& FmtFound, bool& DataFound);

    /** Processes the fmt chunk to extract audio format information (e.g., sample rate, channels) */
    bool ProcessFmtChunk(SFileReader& File, SWavFile* OutWAVFile, SUInt32 ChunkSize);

    /** Processes the data chunk and extracts the audio data */
    bool ProcessDataChunk(SFileReader& File, SWavFile* OutWAVFile, SUInt32 ChunkSize);

    /** Processes 8/16/24/32-bit PCM data from the WAV file */
    void ProcessPCM(SFileReader& File, SWavFile* OutWAVFile, SUInt32 ChunkSize);

    /** Processes the fact chunk, typically containing additional data such as the number of samples */
    bool ProcessFactChunk(SFileReader& File, SWavFile* OutWAVFile, SUInt32 ChunkSize);
};
