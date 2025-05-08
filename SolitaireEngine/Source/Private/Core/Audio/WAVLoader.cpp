#include "SolitaireEnginePCH.h"

//Register SWavLoader
REGISTER_ASSET_LOADER(SWavLoader)

#define RIFF "RIFF"      // Define RIFF chunk identifier
#define WAVE "WAVE"      // Define WAVE format identifier
#define FMT  "fmt "      // Define format chunk identifier
#define DATA "data"      // Define data chunk identifier
#define FACT "fact"      // Define fact chunk identifier

void SWavFile::ClearAsset()
{
    IsValid = false;           
    AudioFormat = 0;          
    NumChannels = 0;         
    SampleRate = 0;           
    ByteRate = 0;              
    BlockAlign = 0;             
    BitsPerSample = 0;          
    cbSize = 0;                 
    ValidBitsPerSample = 0;     
    ChannelMask = 0;           
    memset(SubFormat, 0, sizeof(SubFormat));
    TotalSampleFrames = 0;      
    Data.clear();
    Data.shrink_to_fit();
}

void SWavLoader::Load(const SWString& FilePath, SAsset* OutFile)
{
    // Start loading .wav file
    S_LOG(LogAudio, TEXT("Started loading audio file: %s"), FilePath.c_str());

    //Check if the OutFile is nullptr
    if (OutFile == nullptr)
    {
        //Log error
        S_LOG_ERROR(LogAudio, TEXT("OutFile is nullptr. Cannot load WAV file."));
        return;
    }

    // Cast
    SWavFile* WAVFile  = static_cast<SWavFile*>(OutFile);

    // Check if the WAVFile is nullptr
    if (WAVFile == nullptr)
    {
        S_LOG_ERROR(LogAudio, TEXT("Failed to cast OutFile to SWAVFile."));
        return;
    }
    
    // Open file in binary mode
    SFileReader File(FilePath, std::ios::binary);
    if (!File)
    {
        // Log open failure
        S_LOG_ERROR(LogAudio, TEXT("Could not open WAV file: %s"), FilePath.c_str());

        // Return default output on error
        return;
    }

    // Read RIFF header from the file
    if (!ReadRIFFHeader(File))
    {
        return;
    }

    bool FmtFound = false;   // Flag to mark if fmt chunk found
    bool DataFound = false;  // Flag to mark if data chunk found

    // Iterate through chunks until EOF
    while (File)
    {
        // Process each chunk in the file
        if (!ProcessChunk(File, WAVFile, FmtFound, DataFound))
        {
            // Mark WAV file as invalid
            WAVFile->IsValid = false;

            // Handle missing fmt or data chunk
            if (!FmtFound)
            {
                S_LOG_ERROR(LogAudio, TEXT("WAV file missing 'fmt ' chunk"));
                return;
            }
            if (!DataFound)
            {
                S_LOG_ERROR(LogAudio, TEXT("WAV file missing 'data' chunk"));
                return;
            }

            break;
        }
    }

    // Mark WAV file as valid
    WAVFile->IsValid = true;

    // Successfully loaded .wav file
    S_LOG(LogAudio, TEXT("Finished loading audio file: %s"), FilePath.c_str());
}

void SWavLoader::UnLoad(SAsset* OutFile)
{
    //Check if the OutFile is nullptr
    if (OutFile == nullptr)
    {
        //Log error
        S_LOG_ERROR(LogAudio, TEXT("OutFile is nullptr. Cannot load WAV file."))
    }

    //Casting test
    SWavFile* WAVFile = static_cast<SWavFile*>(OutFile);

    //Check if the WAVFile is nullptr
    if (WAVFile == nullptr)
    {
        //Log error
        S_LOG_ERROR(LogAudio, TEXT("Failed to cast OutFile to SWAVFile."));
    }

    //Clear Asset
    WAVFile->ClearAsset();
}

SSharedPtr<SAsset> SWavLoader::CreateAsset()
{
    return std::make_shared<SWavFile>();
}

SWString SWavLoader::GetSupportedExtension()
{
    return TEXT(".wav");
}

bool SWavLoader::ReadRIFFHeader(SFileReader& File)
{
    // Read the first 4 bytes (should be "RIFF")
    SChar ChunkID[5] = { 0 }; // Buffer for chunk ID (4 chars + null)
    SUInt32 ChunkSize = 0;    // Variable for chunk size value

    // Read first 4 bytes (should be "RIFF")
    File.read(ChunkID, 4);

    // Check if the RIFF header is valid
    if (strcmp(ChunkID, RIFF) != 0)
    {
        // Log an error if the RIFF header is invalid
        S_LOG_ERROR(LogAudio, TEXT("Invalid RIFF header in WAV file. Expected 'RIFF'."));

        // Abort on invalid header
        return false;
    }

    // Read the overall file size
    File.read(reinterpret_cast<char*>(&ChunkSize), sizeof(ChunkSize));

    // Read format identifier (should be "WAVE")
    File.read(ChunkID, 4);

    // Check if the format identifier is "WAVE"
    if (strcmp(ChunkID, WAVE) != 0)
    {
        // Log an error if the WAVE format identifier is missing or incorrect
        S_LOG_ERROR(LogAudio, TEXT("Missing or invalid WAVE format identifier. Expected 'WAVE'."));

        // Abort on invalid format
        return false;
    }

    return true;
}

bool SWavLoader::ProcessChunk(SFileReader& File, SWavFile* OutFile, bool& FmtFound, bool& DataFound)
{
    // Read current chunk ID and size
    SChar ChunkID[5] = { 0 };   // Buffer for chunk ID (4 chars + null)
    SUInt32 ChunkSize = 0;      // Variable for chunk size value

    // Read chunk ID
    File.read(ChunkID, 4);

    // Read chunk size
    File.read(reinterpret_cast<char*>(&ChunkSize), sizeof(ChunkSize)); 

    // Check if the chunk is the format chunk (fmt)
    if (!strcmp(ChunkID, FMT))
    {
        // Process fmt chunk
        FmtFound = true;
        return ProcessFmtChunk(File, OutFile, ChunkSize); 
    }
    // Check if the chunk is the data chunk (data)
    else if (!strcmp(ChunkID, DATA))
    {
        // Process data chunk
        DataFound = true;
        return ProcessDataChunk(File, OutFile, ChunkSize); 
    }
    // Check if the chunk is a fact chunk (fact)
    else if (strncmp(ChunkID, FACT, 4) == 0)
    {
        // Process fact chunk
        return ProcessFactChunk(File, OutFile, ChunkSize);
    }
    else
    {
        // Skip unhandled chunks
        File.seekg(ChunkSize, std::ios::cur);
    }

    // Skip padding byte if chunk size is odd
    if (ChunkSize % 2 == 1)
    {
        // Skip 1 byte of padding
        File.seekg(1, std::ios::cur); 
    }

    return true;
}

bool SWavLoader::ProcessFmtChunk(SFileReader& File, SWavFile* OutFile, SUInt32 ChunkSize)
{
    // Read basic format information from the fmt chunk
    File.read(reinterpret_cast<SChar*>(&OutFile->AudioFormat), sizeof(OutFile->AudioFormat));
    File.read(reinterpret_cast<SChar*>(&OutFile->NumChannels), sizeof(OutFile->NumChannels));
    File.read(reinterpret_cast<SChar*>(&OutFile->SampleRate), sizeof(OutFile->SampleRate));
    File.read(reinterpret_cast<SChar*>(&OutFile->ByteRate), sizeof(OutFile->ByteRate));
    File.read(reinterpret_cast<SChar*>(&OutFile->BlockAlign), sizeof(OutFile->BlockAlign));
    File.read(reinterpret_cast<SChar*>(&OutFile->BitsPerSample), sizeof(OutFile->BitsPerSample));

    // Track number of bytes read from fmt chunk
    SUInt8 FmtBytesRead = 16; 

    // Read additional data if the chunk size is larger than basic information
    if (ChunkSize > FmtBytesRead)
    {
        // Read extra chunk size
        File.read(reinterpret_cast<SChar*>(&OutFile->cbSize), sizeof(OutFile->cbSize));
        FmtBytesRead += 2;

        // Check for extended format information (for certain formats like 0xFFFE)
        if (OutFile->AudioFormat == 0xFFFE && OutFile->cbSize >= 22)
        {
            File.read(reinterpret_cast<SChar*>(&OutFile->ValidBitsPerSample), sizeof(OutFile->ValidBitsPerSample)); // Read valid bits per sample
            File.read(reinterpret_cast<SChar*>(&OutFile->ChannelMask), sizeof(OutFile->ChannelMask));               // Read channel mask
            File.read(reinterpret_cast<SChar*>(OutFile->SubFormat), 16);                                            // Read subformat information

            FmtBytesRead += 22;

            // Skip extra bytes if any are left
            if (OutFile->cbSize > 22)
            {
                // Skip remaining bytes
                File.seekg(OutFile->cbSize - 22, std::ios::cur); 
                FmtBytesRead += (OutFile->cbSize - 22);
            }
        }
        else
        {
            // Skip remaining bytes if no extended format information is present
            File.seekg(OutFile->cbSize, std::ios::cur);
            FmtBytesRead += OutFile->cbSize;
        }
    }

    // Initialize sample frame count to 0
    OutFile->TotalSampleFrames = 0;

    return true;
}

bool SWavLoader::ProcessDataChunk(SFileReader& File, SWavFile* OutFile, SUInt32 ChunkSize)
{
    // Handle different audio formats (PCM, IMA ADPCM, MS ADPCM)
    if (OutFile->AudioFormat == 1) // PCM format
    {
        // Handle different PCM bit depths
        if (OutFile->BitsPerSample == 8 || OutFile->BitsPerSample == 16 || OutFile->BitsPerSample == 24 || OutFile->BitsPerSample == 32)
        {
            // Read PCM data
            ProcessPCM(File, OutFile, ChunkSize);
            S_LOG(LogAudio, TEXT("Processing PCM data with %u bits per sample."), OutFile->BitsPerSample);
        }
        else
        {
            // Log unsupported PCM bit depth
            S_LOG_ERROR(LogAudio, TEXT("Unsupported PCM bit depth: %u. Skipping data."), OutFile->BitsPerSample);

            // Skip unsupported bit depth
            File.seekg(ChunkSize, std::ios::cur);

            //Return false
            return false;
        }
    }
    else
    {
        // Log warning for unsupported IMA ADPCM format
        S_LOG_WARNING(LogAudio, TEXT("AudioFormat %d is not supported. Skipping data."), OutFile->AudioFormat);

        // Skip IMA ADPCM data
        File.seekg(ChunkSize, std::ios::cur);

        //Return false
        return false;
    }

    // Update total sample frame count based on data size
    if (OutFile->BitsPerSample > 0 && OutFile->NumChannels > 0)
    {
        // Calculate total sample frames
        OutFile->TotalSampleFrames = static_cast<SUInt32>(OutFile->Data.size() / OutFile->NumChannels);
        S_LOG(LogAudio, TEXT("Calculated TotalSampleFrames: %u."), OutFile->TotalSampleFrames);
    }
    else
    {
        S_LOG_WARNING(LogAudio, TEXT("Invalid bits per sample or number of channels. Unable to calculate total sample frames."));
    }

    return true;
}

void SWavLoader::ProcessPCM(SFileReader& File, SWavFile* OutFile, SUInt32 ChunkSize)
{
    //Clear
    OutFile->Data.clear();

    // Resize data buffer
    OutFile->Data.resize(ChunkSize);

    // Read and store 8/16/24/32-bit PCM samples
    File.read(reinterpret_cast<SChar*>(OutFile->Data.data()), ChunkSize);
}

bool SWavLoader::ProcessFactChunk(SFileReader& File, SWavFile* OutFile, SUInt32 ChunkSize)
{
    // Variable to hold the number of sample frames from the FACT chunk
    SUInt32 SampleFrames = 0;

    // Read the number of sample frames from the FACT chunk
    File.read(reinterpret_cast<SChar*>(&SampleFrames), sizeof(SampleFrames));

    // Set the number of sample frames in the output WAV file structure
    OutFile->TotalSampleFrames = SampleFrames;

    // If there are extra bytes in the FACT chunk, skip them
    if (ChunkSize > 4)
    {
        // Seek forward by the remaining bytes to skip over them
        File.seekg(ChunkSize - 4, std::ios::cur);
    }

    // Return true to indicate successful processing of the FACT chunk
    return true;
}