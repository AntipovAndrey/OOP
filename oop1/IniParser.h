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
    void initialize(std::string const &fileName)
    throw(IOException);

    bool isHaveSection(std::string const &sectionName) const
    throw(ConfigNotInitialisedException);

    bool isHaveParameter(std::string const &sectionName, std::string const &parameterName) const
    throw(ConfigNotInitialisedException, SectionNotFoundException);

    template<typename T>
    T getValue(std::string const &sectionName, std::string const &parameterName) const
    throw(IniException);

    const std::string toString() const;

private:
    std::map<std::string, std::map<std::string, std::string>> parsedData;
    bool isInitialised = false;

    std::string deleteComments(std::string &line) const;

    bool lineIsEmpty(std::string &line) const;

    bool lineIsSection(std::string &line) const ;

    std::string extractSection(std::string &line) const;

    std::pair<std::string, std::string> extractParameter(std::string &line) const;
};