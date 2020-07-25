#include"AudioFileDesc.h"

unsigned AudioFileDesc::getAudioFormat() const noexcept
{
    return m_iAudioFormat;
}

void AudioFileDesc::setAudioFormat(unsigned val) noexcept
{
    m_iAudioFormat = val;
}

size_t AudioFileDesc::getNumChannels() const noexcept
{
    return samples.size();
}

void AudioFileDesc::setNumChannels(size_t val) noexcept
{
    samples.resize(val, {0.0});
}

unsigned AudioFileDesc::getSampleRate() const noexcept
{
    return m_iSampleRate;
}

void AudioFileDesc::setSampleRate(unsigned val) noexcept
{
    m_iSampleRate = val;
}

unsigned AudioFileDesc::getBitDepth() const noexcept
{
    return m_iBitDepth;
}

void AudioFileDesc::setBitDepth(unsigned val) noexcept
{
    m_iBitDepth = val;
}

size_t AudioFileDesc::getNumSamplesPerChannel() const noexcept
{
    return samples[0].size();
}

void AudioFileDesc::setNumSamplesPerChannel(size_t val) noexcept
{
    for(auto& channel:samples)
        channel.resize(val);
}

double AudioFileDesc::getLengthInSec() const noexcept
{
    return static_cast<double>(getNumSamplesPerChannel())/m_iSampleRate;
}

std::vector<double>& AudioFileDesc::operator[](size_t i)
{
    return samples[i];
}

AudioFileDesc::AudioBuffer::iterator AudioFileDesc::begin()
{
    return samples.begin();
}

AudioFileDesc::AudioBuffer::iterator AudioFileDesc::end()
{
    return samples.end();
}