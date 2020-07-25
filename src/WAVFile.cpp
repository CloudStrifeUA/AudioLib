#include"WAVFile.h"
#include<fstream>
#include<iterator>
#include<iostream>
#include<stdint.h>
#include<limits>
#include"Utils.h"


bool WAVFileDecoder::Read(const std::string& path, AudioFileDesc* file_desc)
{
    std::ifstream file(path, std::ios::binary);
    if(!file.good())
    {
        std::cerr << "Could not open file " << path << std::endl;
        return false;
    }

    file.unsetf(std::ios::skipws);
    std::istream_iterator<uint8_t> begin(file), end;
    std::vector<uint8_t> fileData (begin, end);
    auto cur_pos = fileData.begin();
    std::string ChunkID(cur_pos, cur_pos + 4);
    cur_pos += 8;
    std::string format(cur_pos, cur_pos + 4);

    cur_pos += 4;
    std::string fmtChunkID(cur_pos, cur_pos + 4);
    if(ChunkID != "RIFF" || format != "WAVE" || fmtChunkID != "fmt ")
    {
        std::cerr << "Invalid .wav file" << std::endl;
        return false;
    }

    cur_pos += 8; //skip subchunk size
    //Reading audio format
    file_desc->setAudioFormat(static_cast<unsigned>(BytesToInt<uint16_t>(cur_pos)));

    //Reading number of the  channels
    cur_pos += 2;
    file_desc -> setNumChannels(static_cast<size_t>(BytesToInt<uint16_t>(cur_pos)));

    //Reading sample rate
    cur_pos += 2;
    file_desc -> setSampleRate(BytesToInt<uint32_t>(cur_pos));

    //Reading bit depth
    cur_pos += 10;
    file_desc -> setBitDepth(static_cast<unsigned>(BytesToInt<uint16_t>(cur_pos)));

    //Trying to find "data" subchunk
    cur_pos += 2;
    cur_pos = findChunkPos(cur_pos, fileData.end(), "data");
    if(cur_pos == fileData.end())
    {
        std::cerr << "Data chunk not found" << std::endl;
        return false;
    }
    cur_pos += 4; //skip subchunk id field
    auto iDataSize = BytesToInt<uint32_t>(cur_pos);
    file_desc -> setNumSamplesPerChannel(static_cast<size_t>(iDataSize/((file_desc->getBitDepth()/8)*file_desc -> getNumChannels())));

    //Read all samples
    cur_pos += 4;
    auto bitDepth = file_desc -> getBitDepth();
    auto iBytesPerSample = bitDepth/8;
    for(size_t sample = 0; sample < file_desc -> getNumSamplesPerChannel(); sample++)
    {
        for(size_t channel = 0; channel < file_desc -> getNumChannels(); channel++)
        {
            double res;
            switch (bitDepth)
            {
                case BIT_DEPTH_8:
                {
                    int8_t sample = BytesToInt<int8_t>(cur_pos);
                    res = sample/(double)0x7F;
                    break;
                }
                case BIT_DEPTH_16:
                {
                    int16_t sample = BytesToInt<int16_t>(cur_pos);
                    res = sample/(double)((1 << 15) - 1);
                    break;
                }
                case BIT_DEPTH_24:
                {
                    int sample = BytesToInt<int>(cur_pos, true, bitDepth/8);
                    if(sample & (1 << 23))
                        sample |= ~((1 << 24) - 1);
                    res = sample/(double)((1 << 23) - 1);
                    break;
                }
                case BIT_DEPTH_32:
                {
                    int sample = BytesToInt<int>(cur_pos);
                    if(file_desc -> getAudioFormat() == WAVE_IEEEFloat)
                        res = (double)reinterpret_cast<float&>(sample);
                    else
                        res = sample/(double)std::numeric_limits<std::int32_t>::max();
                    break;
                }
                default:
                {
                    std::cerr << "Invalid bit depth: " << bitDepth << std::endl; 
                    return false;
                    break;
                }
            }

            (*file_desc)[channel][sample] = res;
            cur_pos += iBytesPerSample;
        }
    }
    return true;
}

bool WAVFileEncoder::Write(const std::string& path, AudioFileDesc* file_desc)
{
    std::ofstream output(path, std::ios::out|std::ios::trunc|std::ios::binary);
    if(!output.good())
    {
        std::cerr << "Could not open file " << path << std::endl;
        return false;
    }

    output << "RIFF";
    uint16_t bitDepth = file_desc -> getBitDepth();
    uint32_t iDataChunkSize = file_desc -> getNumSamplesPerChannel() * file_desc -> getNumChannels() * bitDepth/8;
    WriteIntToFile<uint32_t>(output, 4 + 8 + 16 + 8 + iDataChunkSize);
    output << "WAVE";
    output << "fmt ";
    WriteIntToFile<uint32_t>(output, 16);
    WriteIntToFile<uint16_t>(output, file_desc -> getAudioFormat());
    WriteIntToFile<uint16_t>(output, file_desc -> getNumChannels());
    WriteIntToFile<uint32_t>(output, file_desc -> getSampleRate());
    // Bytes/sec
    WriteIntToFile<uint32_t>(output, file_desc -> getSampleRate() * file_desc -> getNumChannels() * bitDepth/8);
    // Block align
    WriteIntToFile<uint16_t>(output, file_desc -> getNumChannels() * bitDepth/8);
    WriteIntToFile<uint16_t>(output, bitDepth);
    output << "data";
    WriteIntToFile<uint32_t>(output, iDataChunkSize);
    for(size_t sample = 0; sample < file_desc -> getNumSamplesPerChannel(); sample++)
    {
        for(size_t channel = 0; channel < file_desc -> getNumChannels(); channel++)
        {
            switch (bitDepth)
            {
                case BIT_DEPTH_8:
                {
                    WriteIntToFile<int8_t>(output, static_cast<int8_t>((*file_desc)[channel][sample] * 0x7F));
                    break;
                }
                case BIT_DEPTH_16:
                {
                    WriteIntToFile<int16_t>(output, static_cast<int16_t>((*file_desc)[channel][sample] * ((1 << 15) - 1)));
                    break;
                }
                case BIT_DEPTH_24:
                {
                    WriteIntToFile<int>(output, static_cast<int>((*file_desc)[channel][sample] * ((1 << 23) - 1)), 3);
                    break;
                }
                case BIT_DEPTH_32:
                {
                    if(file_desc -> getAudioFormat() == WAVE_IEEEFloat)
                        WriteIntToFile<int>(output, reinterpret_cast<int&>((*file_desc)[channel][sample]));
                    else
                        WriteIntToFile<int>(output, static_cast<int>((*file_desc)[channel][sample] * std::numeric_limits<int>::max()));
                    break;
                }
                default:
                {
                    std::cerr << "Invalid bit depth " << bitDepth << std::endl;
                    return false;
                    break;
                }
            }
        }
    }
    output.close();
    return true;
}