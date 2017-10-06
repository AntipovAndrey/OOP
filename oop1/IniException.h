//
// Defining exceptions that can be threw by IniParser.cpp
//

#pragma once

#include <exception>
#include <stdexcept>

class IniException : public std::runtime_error {
public:
    explicit IniException(const std::string &__arg) : runtime_error(__arg) {}
};

class IOException : public IniException {
public:
    explicit IOException(const std::string &fileName) : IniException(fileName + " doesn't exist or can't be read"),
                                                        fileName(fileName) {}

    std::string getFileName() const {
        return fileName;
    }

private:
    std::string fileName;
};

class ConfigNotInitialisedException : public IniException {
public:
    explicit ConfigNotInitialisedException() : IniException("Config wasn't initialised") {}
};

class SectionNotFoundException : public IniException {
public:
    explicit SectionNotFoundException(const std::string &sectionName) : IniException(
            "Section " + sectionName + " not found"), sectionName(sectionName) {}

    std::string getSectionName() const {
        return sectionName;
    }

private:
    std::string sectionName;
};

class NoSuchParameterException : public IniException {
public:
    explicit NoSuchParameterException(const std::string &parameterName) : IniException(
            "Parameter " + parameterName + " not found"), parameterName(parameterName) {}

    std::string getParameterName() const {
        return parameterName;
    }

private:
    std::string parameterName;
};