//
// Created by andrey on 06.09.17.
//

#include <iostream>
#include <algorithm>
#include "IniParser.h"

using namespace std;

void IniParser::initialize(string const &fileName) {
    fstream file;
    file.open(fileName);
    if (!file.good()) throw IOException(fileName);
    string currentSection;
    string currentLine;
    while (!file.eof() && getline(file, currentLine)) {
        currentLine = deleteComments(currentLine);
        if (lineIsEmpty(currentLine)) {
            continue;
        }
        if (lineIsSection(currentLine)) {
            currentSection = extractSection(currentLine);
            continue;
        }
        pair<string, string> parameter = extractParameter(currentLine);
        parsedData[currentSection][parameter.first] = parameter.second;
    }
    file.close();
    isInitialised = true;
}

string IniParser::deleteComments(string &line) const {
    size_t semicolon = line.find(';');
    if (semicolon != string::npos) {
        return line.substr(0, semicolon);
    }
    return line;
}

bool IniParser::lineIsEmpty(const string &line) const {
    size_t minLineLength = 3;
    return line.length() <= minLineLength;
}

bool IniParser::isHaveSection(string const &sectionName) const {
    if (!isInitialised) throw ConfigNotInitialisedException();
    return parsedData.count(sectionName) != 0;
}

bool IniParser::isHaveParameter(string const &sectionName,
                                string const &parameterName) const {
    if (!isInitialised) throw ConfigNotInitialisedException();
    if (!isHaveSection(sectionName)) throw SectionNotFoundException(sectionName);
    return parsedData.at(sectionName).count(parameterName) != 0;
}

template<>
std::string IniParser::getValue<string>(string const &sectionName,
                                        string const &parameterName) const {
    assertExistence(sectionName, parameterName);
    return parsedData.at(sectionName).at(parameterName);
}


template<>
int IniParser::getValue<int>(string const &sectionName,
                             string const &parameterName) const {
    assertExistence(sectionName, parameterName);
    return stoi(parsedData.at(sectionName).at(parameterName));
}

template<>
double IniParser::getValue<double>(string const &sectionName,
                                   string const &parameterName) const {
    assertExistence(sectionName, parameterName);
    return stod(parsedData.at(sectionName).at(parameterName));
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

bool IniParser::lineIsSection(const string &line) const {
    return line.find('[') != string::npos;
}

string IniParser::extractSection(string &line) const {
    return line.substr(line.find('[') + 1, line.find(']') - 1);
}

pair<string, string> IniParser::extractParameter(string &line) const {
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    string param = line.substr(0, line.find('='));
    string value = line.substr(line.find('=') + 1);
    return {param, value};
}

void IniParser::assertExistence(const std::string &sectionName,
                                const std::string &parameterName) const {
    if (!isHaveSection(sectionName)) {
        throw SectionNotFoundException(sectionName);
    }
    if (!isHaveParameter(sectionName, parameterName)) {
        throw NoSuchParameterException(parameterName);
    }
}