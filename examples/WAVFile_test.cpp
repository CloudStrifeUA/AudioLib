#include<WAVFile.h>
#include<iostream>

int main()
{
    WAVFile file;
    file.Load("test.wav");
    size_t NumChannels = file.getNumChannels();
    size_t NumSamples = file.getNumSamplesPerChannel();

    /*  Printing file information */
    std::cout << "AudioFile format: " << ((file.getAudioFormat() == 1)?std::string("PCM"):std::string("IEEEFloat")) << std::endl;
    std::cout << "Sample rate: " << file.getSampleRate() << std::endl;
    std::cout << "Number of the channels: " << NumChannels << std::endl;
    std::cout << "Number of the samples per channel: " << NumSamples << std::endl;
    std::cout << "Bit depth: " << file.getBitDepth() << std::endl;
    std::cout << "Length of the audio in seconds: " << file.getLengthInSec() << std::endl;

    for(size_t channel = 0; channel < NumChannels; channel++)
    {
        for(size_t sample = 1; sample < NumSamples - 1; sample++)
        {
            file[channel][sample] *= 2;
        }
    }

    file.setBitDepth(BIT_DEPTH_32);
    file.Save("test_copy.wav");

    file.setSampleRate(22050);
    file.Save("test_slow.wav");

    file.setSampleRate(88200);
    file.Save("test_fast.wav");


    file.setSampleRate(44100);
    file.setNumSamplesPerChannel(NumSamples/2);
    file.Save("test_cut.wav");

    return 0;
}