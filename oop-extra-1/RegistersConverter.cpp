//
// Created by andrey on 16.10.17.
//

#include <fstream>
#include "RegistersConverter.h"

const std::string RegistersConverter::INI_POSTFIX = ".ini";
const std::string RegistersConverter::HEADER_POSTFIX = ".h";

RegistersConverter::RegistersConverter(const std::string &fileName) : fileName(fileName) {
    parseFile();
}

void RegistersConverter::saveAsIni(const std::string &iniFileName) const {
    std::ofstream iniFile(iniFileName);
    std::for_each(parsedData.begin(), parsedData.end(), [&](std::vector<std::string> inner) {
        std::for_each(inner.begin(), inner.end(), [&](std::string value) {
            iniFile << value << ",  ";
        });
        iniFile << std::endl;
    });
}

void RegistersConverter::saveAsIni() const {
    saveAsIni(fileName + INI_POSTFIX);
}

void RegistersConverter::saveAsHeader(const std::string &headerFileName) const {
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
    headerFile << "const uint32_t lmkConfigCommands[" << parsedData.size() << "][3] = {" << std::endl;

    std::for_each(parsedData.begin(), parsedData.end(), [&](std::vector<std::string> inner) {
        headerFile << "    { ";
        for (int i = 0; i < inner.size(); ++i) {
            headerFile << inner.at(i) << ((i == inner.size() - 1) ? " " : ", ");
        }
        headerFile << "}," << std::endl;
    });

    headerFile << "};" << std::endl;
    headerFile << "#endif //" << includeGuardName << std::endl;
}

void RegistersConverter::saveAsHeader() const {
    saveAsHeader(fileName + HEADER_POSTFIX);
}

void RegistersConverter::parseFile() {
    std::fstream file;
    file.open(fileName);
    //   if (!file.good()) throw IOException(fileName);
    std::string currentLine;
    while (!file.eof() && std::getline(file, currentLine)) {
        parser.process(currentLine);
    }
    parsedData = parser.getParsedData();
}
