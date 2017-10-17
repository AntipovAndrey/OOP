/**
 *
 *  Class for convert file with information about registers to .ini or .h file
 *  Source file format - multiple lines pattern-matching with
 *                       <Register name><tab><address_1><address_2><command>
 *                       address_1, address_2 and command are one 3-byte HEX number,
 *                       e.g: R267  0x010B07 means:  Name        = R267
 *                                                   Address_1   = 0x01
 *                                                   Address_2   = 0x0B
 *                                                   Command     = 0x07
 *  Ini output format  - same ordered data with no name of register, each line is comma-separated
 *                       e.g:   0x01,  0x13,  0x07,
 *                              0x01,  0x14,  0x20,
 *                              0x01,  0x15,  0x11,
 *  Header output format - Include Guard in C-style, information about number of commands, array of commands
 *                       e.g (without IG):   const uint32_t numCommands = 2;
 *                                           const uint8_t LmkConfigCmds[numCommands][3]={
 *                                                  { 0x00, 0x00, 0x80 },
 *                                                  { 0x00, 0x00, 0x00 },
 *                                           };
 *
 *
 *
 *
 */

#pragma once

#include <string>
#include "RegistersParser.h"

class RegistersConverter {
public:
    explicit RegistersConverter(const std::string &fileName);

    bool parseFile();

    bool saveAsIni(const std::string &iniFileName) const;

    bool saveAsIni() const;

    bool saveAsHeader(const std::string &headerFileName) const;

    bool saveAsHeader() const;

private:

    const std::string &fileName;
    static const std::string INI_POSTFIX;
    static const std::string HEADER_POSTFIX;
    RegistersParser parser;
    std::vector<std::vector<std::string>> parsedData;
};


