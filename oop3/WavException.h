//
// Created by andrey on 27.09.17.
//

#pragma once

#include <exception>

class WavException : public std::runtime_error {
public:
    explicit WavException(const std::string &__arg) : runtime_error(__arg) {}
};

class IOException : public WavException {
public:
    explicit IOException(const std::string &fileName) : WavException("IO Error with " + fileName + " file") {}
};

class BadFormatException : public WavException {
public:
    explicit BadFormatException(const std::string &fileName) : WavException("Bad format in " + fileName + " file") {}
};

class UnsupportedFormatException : public WavException {
public:
    explicit UnsupportedFormatException(const std::string &fileName) : WavException(
            "Unsupported format of " + fileName + " file") {}
};

class BadParamsException : public WavException {
public:
    explicit BadParamsException(const std::string &fileName) : WavException(
            "Bad wavs parameters of " + fileName + " file") {}
};

class DataSizeErrorException : public WavException {
public:
    explicit DataSizeErrorException(const std::string &fileName) : WavException(
            "Corrupt size of " + fileName + " file") {}
};

class WavHeaderException : public WavException {
public:
    explicit WavHeaderException(const std::string &__arg) : WavException(__arg) {}
};

class HeaderRiffException : public WavHeaderException {
public:
    explicit HeaderRiffException(const std::string &fileName) : WavHeaderException(
            "This is not a RIFF format of " + fileName + " file") {}
};

class HeaderFileSizeErrorException : public WavHeaderException {
public:
    explicit HeaderFileSizeErrorException(const std::string &fileName) : WavHeaderException(
            "Corrupt header size of " + fileName + " file") {}
};

class HeaderWaveErrorException : public WavHeaderException {
public:
    explicit HeaderWaveErrorException(const std::string &fileName) : WavHeaderException(
            "This is not a WAVE header of " + fileName + " file") {}
};

class HeaderFtmException : public WavHeaderException {
public:
    explicit HeaderFtmException(const std::string &fileName) : WavHeaderException(
            "This is not a ftm subchunk in " + fileName + " file") {}
};

class HeaderNotPCMException : public WavHeaderException {
public:
    explicit HeaderNotPCMException(const std::string &fileName) : WavHeaderException(
            "Header of " + fileName + " file isn't a PCM header") {}
};

class HeaderFirstSubchunkErrorException : public WavHeaderException {
public:
    explicit HeaderFirstSubchunkErrorException(const std::string &fileName) : WavHeaderException(
            "Corrupt first subchunk in " + fileName + " file") {}
};

class HeaderSecondSubchunkErrorException : public WavHeaderException {
public:
    explicit HeaderSecondSubchunkErrorException(const std::string &fileName) : WavHeaderException(
            "Corrupt second subchunk in " + fileName + " file") {}
};

class HeaderBytesRateErrorException : public WavHeaderException {
public:
    explicit HeaderBytesRateErrorException(const std::string &fileName) : WavHeaderException(
            "Corrupt header bytes rate in " + fileName + " file") {}
};

class HeaderBlockAlignErrorException : public WavHeaderException {
public:
    explicit HeaderBlockAlignErrorException(const std::string &fileName) : WavHeaderException(
            "Corrupt header block align in " + fileName + " file") {}
};

class ReverberationException : public WavException {
public:
    explicit ReverberationException(const std::string &msg) : WavException("Reverberation error : " + msg) {}
};
