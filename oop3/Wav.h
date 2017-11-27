//
// Created by andrey on 27.09.17.
//

#pragma once

#include <vector>
#include "WavException.h"
#include <cstdint>

class Wav {
public:

    explicit Wav(const std::string &fileName);

    void applyReverberation(double delay, float decay);

    void applyReverberationByChannel(double delay, float decay, int channel);

    void makeMono() ;

    void makeWavFile(const std::string &fileName) ;

    std::string getDescription() const;

    void save();

    void cutLastSeconds(float seconds);

    void cutFirstSeconds(float seconds);

    bool isStereo() const {
        return channelsData.size() > 1;
    }

    static const int leftChannel = 1, rightChannel = 2;

private:

    void clearHeader();

    void initHeader();

    void checkHeader(long fileSize) const ;

    void fillHeader(int channelsCount, int bitsPerSample, int sampleRate,
                    int samplesCountPerChannel);

    void updateHeader();

    void extractDataInt16(std::ifstream &file);

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

    unsigned int getChannelsCount() const {
        return static_cast<unsigned int>(channelsData.size());
    }

    unsigned int getSamplesCountPerChannel() const {
        return static_cast<unsigned int>(channelsData[0].size());
    }

    unsigned int secondsToBytes(float seconds) const;

    bool isMono() {
        return channelsData.size() == 1;
    };
};

