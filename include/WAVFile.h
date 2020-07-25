#ifndef _WAVFILE_H
#define _WAVFILE_H
#include<string>
#include"AudioFile.h"

enum
{
    WAVE_PCM = 0x0001,
    WAVE_IEEEFloat = 0x0003
};

enum
{
    BIT_DEPTH_8 = 8,
    BIT_DEPTH_16 = 16,
    BIT_DEPTH_24 = 24,
    BIT_DEPTH_32 = 32
};

class WAVFileDecoder
{
    protected:
    bool Read(const std::string& path, AudioFileDesc* file_desc);
};

class WAVFileEncoder
{
    protected:
    bool Write(const std::string& path, AudioFileDesc* file_desc);
};

using WAVFile = AudioFile<WAVFileDecoder, WAVFileEncoder>;

#endif