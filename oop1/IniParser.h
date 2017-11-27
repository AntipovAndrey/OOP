//
// Header file for IniParser.cpp
//

#pragma once

#include <fstream>
#include <map>
#include <string>
#include "IniException.h"

class IniParser {
public:
    void initialize(std::string const &fileName);

    bool isHaveSection(std::string const &sectionName) const;

    bool isHaveParameter(std::string const &sectionName, std::string const &parameterName) const;

    template<typename T>
    T getValue(std::string const &sectionName, std::string const &parameterName) const;

    const std::string toString() const;

private:
    std::map<std::string, std::map<std::string, std::string>> parsedData;
    bool isInitialised = false;

    std::string deleteComments(std::string &line) const;

    bool lineIsEmpty(const std::string &line) const;

    bool lineIsSection(const std::string &line) const;

    std::string extractSection(std::string &line) const;

    std::pair<std::string, std::string> extractParameter(std::string &line) const;

    void assertExistence(const std::string &sectionName,
                         const std::string &parameterName) const;
};