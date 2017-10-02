//
// Created by andrey on 06.09.17.
//

#include <iostream>
#include <algorithm>
#include "IniParser.h"

using namespace std;

void IniParser::initialize(string const &fileName) throw(IOException) {
    fstream file;
    file.open(fileName);
    if (!file.good()) throw IOException(fileName);
    string currentSection;
    string currentLine;
    while (!file.eof() && getline(file, currentLine)) {
        size_t semicolon = currentLine.find(';');
        if (semicolon != string::npos) {
            currentLine = currentLine.substr(0, semicolon);
        }
        if (currentLine.length() < 4) {
            continue;
        }
        if (currentLine.find('[') != string::npos) {
            currentSection = currentLine.substr(currentLine.find('[') + 1, currentLine.find(']') - 1);
            continue;
        }
        currentLine.erase(remove(currentLine.begin(), currentLine.end(), ' '), currentLine.end());
        string param = currentLine.substr(0, currentLine.find('='));
        string value = currentLine.substr(currentLine.find('=') + 1);
        parsedData[currentSection][param] = value;
    }
    file.close();
    isInitialised = true;
}

bool IniParser::isHaveSection(string const &sectionName) const throw(ConfigNotInitialisedException) {
    if (!isInitialised) throw ConfigNotInitialisedException();
    return parsedData.count(sectionName) != 0;
}

bool IniParser::isHaveParameter(string const &sectionName,
                                string const &parameterName) const throw(ConfigNotInitialisedException, SectionNotFoundException) {
    if (!isInitialised) throw ConfigNotInitialisedException();
    if (!isHaveSection(sectionName)) throw SectionNotFoundException(sectionName);
    return parsedData.at(sectionName).count(parameterName) != 0;
}

template<>
std::string IniParser::getValue<string>(string const &sectionName,
                                        string const &parameterName) throw(IniException) {
    if (isHaveSection(sectionName) && isHaveParameter(sectionName, parameterName)) {
        return parsedData.at(sectionName).at(parameterName);
    }
    throw NoSuchParameterException(parameterName);
}


template<>
int IniParser::getValue<int>(string const &sectionName,
                             string const &parameterName) throw(IniException) {
    if (isHaveSection(sectionName) && isHaveParameter(sectionName, parameterName)) {

        return stoi(parsedData.at(sectionName).at(parameterName));
    }
    throw NoSuchParameterException(parameterName);
}

template<>
double IniParser::getValue<double>(string const &sectionName,
                                   string const &parameterName) throw(IniException) {
    if (isHaveSection(sectionName) && isHaveParameter(sectionName, parameterName)) {
        return stod(parsedData.at(sectionName).at(parameterName));
    }
    throw NoSuchParameterException(parameterName);
}

const string IniParser::toString() const {
    string toReturn;
    for (auto it : parsedData) {
        toReturn += " ======= " + it.first + " =======\n";
        map<string, string> innerMap = it.second;
        for (auto it2 : innerMap) {
            toReturn += it2.first + ":" + it2.second + "\n";
        }
    }
    return toReturn;
}