//
// Created by andrey on 16.10.17.
//

#include "RegistersConverter.h"

RegistersConverter::RegistersConverter(const std::string &fileName) : fileName(fileName) {

}

void RegistersConverter::saveAsIni(const std::string &iniFileName) const {

}

void RegistersConverter::saveAsIni() const {
    saveAsIni(fileName + INI_POSTFIX);
}

void RegistersConverter::saveAsHeader(const std::string &headerFileName) const {

}

void RegistersConverter::saveAsHeader() const {
    saveAsHeader(fileName + HEADER_POSTFIX);
}
