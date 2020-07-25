#ifndef _AUDIOFILEDESC_H
#define _AUDIOFILEDESC_H
#include<vector>

class AudioFileDesc
{
    public:
    using AudioBuffer = std::vector<std::vector<double>>;
    std::vector<double>& operator[](size_t i);
    unsigned getSampleRate() const noexcept;

    void setSampleRate(unsigned val) noexcept;

    unsigned getBitDepth() const noexcept;

    void setBitDepth(unsigned val) noexcept;

    double getLengthInSec() const noexcept;

    size_t getNumChannels() const noexcept;

    void setNumChannels(size_t val) noexcept;

    size_t getNumSamplesPerChannel() const noexcept;

    void setNumSamplesPerChannel(size_t val) noexcept;

    unsigned getAudioFormat() const noexcept;

    void setAudioFormat(unsigned val) noexcept;

    AudioBuffer::iterator begin();
    
    AudioBuffer::iterator end();

    private:
    AudioBuffer samples;
    unsigned m_iSampleRate;
    unsigned m_iBitDepth;
    unsigned m_iAudioFormat;
};


#endif