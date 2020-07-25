#ifndef _AUDIO_UTILS_H
#define _AUDIOF_UTILS_H
#include<iterator>
#include<vector>

template<typename T>
T BytesToInt(std::vector<uint8_t>::iterator start, bool LittleEndian = true, unsigned size = sizeof(T))
{
    T tmp = 0;
    auto end = (start + size - 1);
    for(;start != end + 1; start++)
    {
        tmp |= static_cast<T>((*start)) << ((1 - LittleEndian)*(end - start) + LittleEndian*(size - 1 - (end - start)))*8;
    }
    return tmp;
}

template<typename T>
void WriteIntToFile(std::ofstream& output, const T& val, bool LittleEndian = true, unsigned size = sizeof(T))
{
    unsigned i = size;
    while(i--)
    {
        output << static_cast<char>(val >> ((i*(1 - LittleEndian) + LittleEndian*(size - 1 - i))*8) & 0xFF);
    }
}

auto findChunkPos(const std::vector<uint8_t>::iterator& begin, const std::vector<uint8_t>::iterator& end, const std::string& chunkID)
{
    auto pos = begin;
    for(; pos!=end; pos++)
    {
        std::string tmp(pos, pos + chunkID.size());
        if(tmp == chunkID)
            return pos;
    }
    return end;
}

#endif