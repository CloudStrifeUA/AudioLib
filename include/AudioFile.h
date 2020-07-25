#ifndef _AUDIOFILE_H
#define _AUDIOFILE_H
#include<string>
#include"AudioFileDesc.h"

template<class AudioFileDecoder, class AudioFileEncoder>
class AudioFile: public AudioFileDesc, private AudioFileDecoder, private AudioFileEncoder
{
    using AudioFileDecoder::Read;
    using AudioFileEncoder::Write;

    public:
    bool Load(std::string path)
    {
        return Read(path, this);
    }
    bool Save(std::string path)
    {
        return Write(path, this);
    }
};

#endif