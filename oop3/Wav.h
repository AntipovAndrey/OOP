//
// Created by andrey on 27.09.17.
//

#pragma once

#include <vector>
#include "WavException.h"
#include <cstdint>

class Wav {
public:

    explicit Wav(const std::string &fileName) throw(WavException);

    void applyReverberation(double delay, float decay) throw(BadParamsException);

    void applyReverberationByChannel(double delay, float decay, int channel) throw(BadParamsException);

    void makeMono() throw(BadParamsException);

    void makeWavFile(const std::string &filename) throw(WavException);

    std::string getDescription() const;

    void save();

    void cutLastSeconds(float seconds);

    void cutFirstSeconds(float seconds);

    bool isStereo() {
        return channelsData.size() > 1;
    }

    static const int leftChannel = 0, rightChannel = 1;

private:

    void clearHeader();

    void initHeader();

    void checkHeader(long fileSize) const throw(WavHeaderException);

    void fillHeader(int channelsCount, int bitsPerSample, int sampleRate,
                    int samplesCountPerChannel) throw(WavException);

    void updateHeader();

    void extractDataInt16(std::ifstream &file) throw(WavException);

    std::vector<std::vector<short>> channelsData;

    std::string wavFileName;

    struct WavHeader {
        char chunkId[4];

        uint32_t chunkSize;

        char format[4];

        char subchunk1Id[4];

        uint32_t subchunk1Size;

        uint16_t audioFormat;

        uint16_t numChannels;

        uint32_t sampleRate;

        uint32_t byteRate;

        uint16_t blockAlign;

        uint16_t bitsPerSample;

        char subchunk2Id[4];

        uint32_t subchunk2Size;
    };

    WavHeader header;

    const static short HEADER_SIZE = 44;

    unsigned int getChannelsCount() {
        return static_cast<unsigned int>(channelsData.size());
    }

    unsigned int getSamplesCountPerChannel() {
        return static_cast<unsigned int>(channelsData[0].size());
    }

    unsigned int secondsToBytes(float seconds) const;

};

