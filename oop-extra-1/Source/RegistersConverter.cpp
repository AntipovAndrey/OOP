//
// Created by andrey on 16.10.17.
//

#include <fstream>
#include <iostream>
#include "../Headers/RegistersConverter.h"

const std::string RegistersConverter::INI_POSTFIX = ".ini";
const std::string RegistersConverter::HEADER_POSTFIX = ".h";

RegistersConverter::RegistersConverter(const std::string &fileName) : fileName(fileName) {}

bool RegistersConverter::saveAsIni(const std::string &iniFileName) const {
    std::ofstream iniFile(iniFileName);

    for (unsigned int i = 0; i < parsedData.size(); ++i) {
        for (unsigned int j = 0; j < parsedData.at(i).size(); ++j) {
            iniFile << parsedData.at(i).at(j);
            if (j != parsedData.at(i).size() - 1) {
                iniFile << ",  ";
            }
        }
        if (i != parsedData.size() - 1) {
            iniFile << ",  " << std::endl;
        }
    }

    if (iniFile.fail()) {
        std::cerr << "Can not write file: " << iniFileName << std::endl;
        return false;
    }
    return true;
}

bool RegistersConverter::saveAsIni() const {
    return saveAsIni(cutFilenameExtension(fileName) + INI_POSTFIX);
}

bool RegistersConverter::saveAsHeader(const std::string &headerFileName) const {
    std::ofstream headerFile(headerFileName);
    std::string includeGuardName(headerFileName);
    std::for_each(includeGuardName.begin(), includeGuardName.end(), [&](char &c) {
        c = static_cast<char>(toupper(c));
        if (c == '.') c = '_';
    });
    headerFile << "#ifndef " << includeGuardName << std::endl;
    headerFile << "#define " << includeGuardName << std::endl;
    headerFile << "#include <stdint.h>" << std::endl;
    headerFile << "const uint32_t numCommands = " << parsedData.size() << ";" << std::endl;
    headerFile << "const uint32_t lmkConfigCommands[numCommands][3] = {" << std::endl;

    for (unsigned int i = 0; i < parsedData.size(); ++i) {
        headerFile << "    { ";
        for (unsigned int j = 0; j < parsedData.at(i).size(); ++j) {
            headerFile << parsedData.at(i).at(j);
            if (j != parsedData.at(i).size() - 1) {
                headerFile << ",  ";
            }
        }
        if (i != parsedData.size() - 1) {
            headerFile << "}, " << std::endl;
        } else {
            headerFile << "}" << std::endl;
        }
    }

    headerFile << "};" << std::endl;
    headerFile << "#endif //" << includeGuardName << std::endl;
    if (headerFile.fail()) {
        std::cerr << "Can not write file: " << headerFileName << std::endl;
        return false;
    }
    return true;
}

bool RegistersConverter::saveAsHeader() const {
    return saveAsHeader(cutFilenameExtension(fileName) + HEADER_POSTFIX);
}

std::string RegistersConverter::cutFilenameExtension(const std::string &str) const {
    return str.substr(0, str.find_last_of('.'));
}

bool RegistersConverter::parseFile() {
    std::fstream file;
    file.open(fileName);
    if (!file.good()) {
        std::cerr << "Can not read file: " << fileName << std::endl;
        return false;
    }
    std::string currentLine;
    while (!file.eof() && std::getline(file, currentLine)) {
        parser.process(currentLine);
    }
    parsedData = parser.getParsedData();
    return true;
}
