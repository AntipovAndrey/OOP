//
//  Wrapper for working with wave files.
//


#include <iostream>
#include <fstream>
#include <cstring>
#include "Wav.h"
#include <cmath>


Wav::Wav(const std::string &fileName) {

    clearHeader(); // Fill header with zeroes.

    wavFileName = fileName;

    std::ifstream file(fileName, std::ifstream::binary);
    if (!file.good()) {
        throw IOException(wavFileName);
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(&header), sizeof(WavHeader));

    if (file.gcount() != sizeof(WavHeader)) {
        // can't read header, because the file is too small.
        throw BadFormatException(wavFileName);
    }

    file.seekg(0, std::ios::end);

    checkHeader(file.tellg());

    extractDataInt16(file);
}

void Wav::clearHeader() {
    memset(&header, 0, sizeof(WavHeader));
}

void Wav::checkHeader(long fileSize) const {
    // Go to Wav.h for details

    if (header.chunkId[0] != 'R' ||
        header.chunkId[1] != 'I' ||
        header.chunkId[2] != 'F' ||
        header.chunkId[3] != 'F') {
        throw HeaderRiffException(wavFileName);
    }

    if (header.chunkSize != fileSize - 8) {
        throw HeaderFileSizeErrorException(wavFileName);
    }

    if (header.format[0] != 'W' ||
        header.format[1] != 'A' ||
        header.format[2] != 'V' ||
        header.format[3] != 'E') {
        throw HeaderWaveErrorException(wavFileName);
    }

    if (header.subchunk1Id[0] != 'f' ||
        header.subchunk1Id[1] != 'm' ||
        header.subchunk1Id[2] != 't' ||
        header.subchunk1Id[3] != ' ') {
        throw HeaderFtmException(wavFileName);
    }

    if (header.audioFormat != 1) {
        throw HeaderNotPCMException(wavFileName);
    }

    if (header.subchunk1Size != 16) {
        throw HeaderFirstSubchunkErrorException(wavFileName);
    }

    if (header.byteRate != header.sampleRate * header.numChannels * header.bitsPerSample / 8) {
        throw HeaderBytesRateErrorException(wavFileName);
    }

    if (header.blockAlign != header.numChannels * header.bitsPerSample / 8) {
        throw HeaderBlockAlignErrorException(wavFileName);
    }

    if (header.subchunk2Id[0] != 'd' ||
        header.subchunk2Id[1] != 'a' ||
        header.subchunk2Id[2] != 't' ||
        header.subchunk2Id[3] != 'a') {
        throw HeaderFtmException(wavFileName);
    }

    if (header.subchunk2Size != fileSize - HEADER_SIZE) {
        throw HeaderSecondSubchunkErrorException(wavFileName);
    }
}

std::string Wav::getDescription() const {
    std::string toReturn;
    toReturn = "audioFormat     " + std::to_string(header.audioFormat) + "\n"
               + "numChannels     " + std::to_string(header.numChannels) + "\n"
               + "sampleRate      " + std::to_string(header.sampleRate) + "\n"
               + "bitsPerSample   " + std::to_string(header.bitsPerSample) + "\n"
               + "byteRate        " + std::to_string(header.byteRate) + "\n"
               + "blockAlign      " + std::to_string(header.blockAlign) + "\n"
               + "chunkSize       " + std::to_string(header.chunkSize) + "\n"
               + "subchunk1Size   " + std::to_string(header.subchunk1Size) + "\n"
               + "subchunk2Size   " + std::to_string(header.subchunk2Size) + "\n";
    return toReturn;
}

void Wav::extractDataInt16(std::ifstream &file) {
    if (header.bitsPerSample != 16) {
        // Only 16-bit samples is supported.
        throw UnsupportedFormatException(wavFileName);
    }

    file.seekg(HEADER_SIZE, std::ios::beg);  // Seek to the begin of PCM data.

    uint16_t channelsCount = header.numChannels;
    unsigned int samplesPerChannel = (header.subchunk2Size / sizeof(uint16_t)) / channelsCount;

    // 1. Reading all PCM data from file to a single std::vector.
    std::vector<short> allChannels;
    allChannels.resize(channelsCount * samplesPerChannel);
    file.read((char *) allChannels.data(), header.subchunk2Size);

    if (file.gcount() != header.subchunk2Size) {
        throw IOException(wavFileName);
    }
    file.close();

    // 2. Put all channels to its own std::vector.
    channelsData.resize(channelsCount);
    for (auto &ch : channelsData) {
        ch.resize(samplesPerChannel);
    }

    for (int ch = 0; ch < channelsCount; ch++) {
        std::vector<short> &channel = channelsData[ch];
        for (size_t i = 0; i < samplesPerChannel; i++) {
            channel[i] = allChannels[channelsCount * i + ch];
        }
    }
}

void Wav::initHeader() {
    // Go to wav_header.h for details

    header.chunkId[0] = 'R';
    header.chunkId[1] = 'I';
    header.chunkId[2] = 'F';
    header.chunkId[3] = 'F';

    header.format[0] = 'W';
    header.format[1] = 'A';
    header.format[2] = 'V';
    header.format[3] = 'E';

    header.subchunk1Id[0] = 'f';
    header.subchunk1Id[1] = 'm';
    header.subchunk1Id[2] = 't';
    header.subchunk1Id[3] = ' ';

    header.subchunk2Id[0] = 'd';
    header.subchunk2Id[1] = 'a';
    header.subchunk2Id[2] = 't';
    header.subchunk2Id[3] = 'a';

    header.audioFormat = 1;
    header.subchunk1Size = 16;
    header.bitsPerSample = 16;
}

void Wav::makeWavFile(const std::string &fileName) {

    if (getChannelsCount() < 1) {
        throw BadParamsException(wavFileName);
    }

    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < getChannelsCount(); ch++) {
        if (channelsData[ch].size() != (size_t) getSamplesCountPerChannel()) {
            throw BadParamsException(wavFileName);
        }
    }

    int bitsPerSample = 16;
    fillHeader(getChannelsCount(), bitsPerSample, header.sampleRate, getSamplesCountPerChannel());

    std::vector<short> allChannels;
    allChannels.resize(getChannelsCount() * getSamplesCountPerChannel());

    for (int ch = 0; ch < getChannelsCount(); ch++) {
        const std::vector<short> &channelData = channelsData[ch];
        for (size_t i = 0; i < getSamplesCountPerChannel(); i++) {
            allChannels[getChannelsCount() * i + ch] = channelData[i];
        }
    }

    std::ofstream output(fileName, std::ofstream::binary | std::ofstream::out);

    output.write(reinterpret_cast<char *>(&header), sizeof(WavHeader));
    output.write(reinterpret_cast<char *>(allChannels.data()), allChannels.size() * sizeof(short));


    if (output.fail()) {
        throw IOException(wavFileName);
    }

    output.close();
}

void Wav::fillHeader(int channelsCount, int bitsPerSample, int sampleRate,
                     int samplesCountPerChannel) {

    if (bitsPerSample != 16) {
        throw UnsupportedFormatException(wavFileName);
    }

    if (channelsCount < 1) {
        throw BadParamsException(wavFileName);
    }
    initHeader();

    int fileSizeBytes = HEADER_SIZE + channelsCount * (bitsPerSample / 8) * samplesCountPerChannel;

    header.sampleRate = static_cast<uint32_t>(sampleRate);
    header.numChannels = static_cast<uint16_t>(channelsCount);
    header.bitsPerSample = 16;

    header.chunkSize = static_cast<uint32_t>(fileSizeBytes - 8);
    header.subchunk2Size = static_cast<uint32_t>(fileSizeBytes - HEADER_SIZE);

    header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = static_cast<uint16_t>(header.numChannels * header.bitsPerSample / 8);
}


void Wav::makeMono() {

    if (getChannelsCount() != 2) {
        throw BadParamsException(wavFileName);
    }

    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < getChannelsCount(); ch++) {
        if (channelsData[ch].size() != (size_t) getSamplesCountPerChannel()) {
            throw BadParamsException(wavFileName);
        }
    }

    std::vector<short> mono = channelsData[0];
    mono.resize(1);
    mono.resize(getSamplesCountPerChannel());

    // Mono channel is an arithmetic mean of all (two) channels.
    for (size_t i = 0; i < getSamplesCountPerChannel(); i++) {
        mono[i] = static_cast<short>((channelsData[0][i] / 2 + channelsData[1][i] / 2)
                                     + (channelsData[0][i]) % 2 * channelsData[1][i] % 2);
    }
    channelsData.pop_back();
    channelsData[0] = mono;
    updateHeader();
}

void Wav::save() {
    makeWavFile(wavFileName);
}

void Wav::cutLastSeconds(float seconds) {

    int shortsToDelete = secondsToBytes(seconds) / sizeof(short);

    for (auto &ch : channelsData) {
        ch.erase(ch.end() - shortsToDelete, ch.end());
    }
    updateHeader();
}


unsigned int Wav::secondsToBytes(float seconds) const {
    return static_cast<unsigned int>((header.bitsPerSample / 8) * header.sampleRate * seconds);
}

void Wav::cutFirstSeconds(float seconds) {
    int shortsToDelete = secondsToBytes(seconds) / sizeof(short);

    for (auto &ch : channelsData) {
        ch.erase(ch.begin(), ch.begin() + shortsToDelete);
    }
    updateHeader();
}

void Wav::updateHeader() {
    fillHeader(static_cast<int>(channelsData.size()), header.bitsPerSample, header.sampleRate,
               getSamplesCountPerChannel());
}

void Wav::applyReverberation(double delay, float decay) {
    for (int i = 0; i < getChannelsCount(); ++i) {
        applyReverberationByChannel(delay, decay, i);
    }
}

void Wav::applyReverberationByChannel(double delay, float decay,
                                      int channel) {

    int channelAsIndex = channel - 1;

    if (isMono()) {
        channelAsIndex = 0;
    }

    if (channelAsIndex < 0 || channelAsIndex > getChannelsCount()) {
        throw ReverberationException(
                "bad channel number : " + std::to_string(channel) + ". There is only " +
                std::to_string(getChannelsCount()));
    }


    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < getChannelsCount(); ch++) {
        if (channelsData[ch].size() != (size_t) getSamplesCountPerChannel()) {
            throw BadParamsException(wavFileName);
        }
    }

    auto delaySamples = static_cast<uint32_t>(delay * header.sampleRate);

    std::vector<float> tmp;
    tmp.resize(channelsData[channelAsIndex].size());

    // Convert signal from short to float
    for (size_t i = 0; i < getSamplesCountPerChannel(); i++) {
        tmp[i] = channelsData[channelAsIndex][i];
    }

    // Add a reverb
    for (size_t i = 0; i < getSamplesCountPerChannel() - delaySamples; i++) {
        tmp[i + delaySamples] += decay * tmp[i];
    }

    // Find maximum signal's magnitude
    float maxMagnitude = 0.0001f;
    for (size_t i = 0; i < getSamplesCountPerChannel() - delaySamples; i++) {
        if (abs(tmp[i]) > maxMagnitude) {
            maxMagnitude = abs(tmp[i]);
        }
    }
    // Signed short can keep values from -32768 to +32767,
    // After reverb, usually there are values large 32000.
    // So we must scale all values back to [ -32768 ... 32768 ]
    float normalCoef = 30000.0f / maxMagnitude;

    // Scale back and transform floats to shorts.
    for (size_t i = 0; i < getSamplesCountPerChannel(); i++) {
        channelsData[channelAsIndex][i] = (short) (normalCoef * tmp[i]);
    }

    updateHeader();
}