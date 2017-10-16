//
// Created by andrey on 16.10.17.
//

#include <sstream>
#include <iostream>
#include "RegistersParser.h"

const std::regex RegistersParser::registerPattern = std::regex("R[0-9]+.*0x[0-9A-Fa-f]{6}");
const std::regex RegistersParser::registerNamePattern = std::regex("R[0-9]+");
const std::regex RegistersParser::registerDataPattern = std::regex("[0-9A-Fa-f]{6}");

void RegistersParser::process(const std::string &line) {
    std::smatch matched;
    if (std::regex_search(line, matched, registerPattern)) {
        if (!isPreamble(matched[0].str())) {
            if (!isPostamble(matched[0].str())) {
                if (firstPush) {
                    firstPush = false;
                    fillWithPreamble();
                }
                repository.emplace_back(Register(matched[0].str()));
            } else if (lastPush) {
                fillWithPostamble();
            }
        }
    }
}


bool RegistersParser::isPreamble(const std::string &line) const {
    std::smatch matched;
    if (std::regex_search(line, matched, registerNamePattern)) {
        int numOfRegister = std::stoi(matched.str().substr(1));
        return numOfRegister >= 0 && numOfRegister <= 13;
    }
    return false;
}

bool RegistersParser::isPostamble(const std::string &line) const {
    std::smatch matched;
    if (std::regex_search(line, matched, registerNamePattern)) {
        int numOfRegister = std::stoi(matched.str().substr(1));
        if (numOfRegister == 8191) {
            lastPush = true;
        }
        return numOfRegister >= 8189 && numOfRegister <= 8191;
    }
    return false;
}

std::vector<std::vector<std::string>> RegistersParser::getParsedData() const {
    std::vector<std::vector<std::string>> allData;
    std::for_each(repository.begin(), repository.end(), [&](Register reg) { allData.emplace_back(reg.asStrings()); });

    return allData;
}

std::vector<std::string> RegistersParser::Register::asStrings() const {

    std::vector<std::string> toReturn(3);
    std::stringstream firstAddressStream, secondAddressStream, commandStream;
    firstAddressStream << std::hex << std::uppercase
                       << "0x" << (firstAddress < 16 ? "0" : "") << (int) firstAddress;;

    secondAddressStream << std::hex << std::uppercase
                        << "0x" << (secondAddress < 16 ? "0" : "") << (int) secondAddress;
    commandStream << std::hex << std::uppercase
                  << "0x" << (command < 16 ? "0" : "") << (int) command;
    toReturn[0] = firstAddressStream.str();
    toReturn[1] = secondAddressStream.str();
    toReturn[2] = commandStream.str();

    return toReturn;
}

RegistersParser::Register::Register(const std::string &correctLine) {
    std::smatch dataFromLine;
    if (std::regex_search(correctLine, dataFromLine, registerDataPattern)) {
        firstAddress = static_cast<uint8_t>(std::stoul(dataFromLine[0].str().substr(0, 2), nullptr, 16));
        secondAddress = static_cast<uint8_t>(std::stoul(dataFromLine[0].str().substr(2, 2), nullptr, 16));
        command = static_cast<uint8_t>(std::stoul(dataFromLine[0].str().substr(4, 2), nullptr, 16));
    }
}

void RegistersParser::fillWithPreamble() {
    repository.emplace_back(Register("0x000080"));
    repository.emplace_back(Register("0x000000"));
}

void RegistersParser::fillWithPostamble() {
    repository.emplace_back(Register("0x0143D1"));
    repository.emplace_back(Register("0x0143F1"));
    repository.emplace_back(Register("0x0143D1"));
    repository.emplace_back(Register("0x0144FF"));
    repository.emplace_back(Register("0x014351"));
    repository.emplace_back(Register("0x014352"));
    repository.emplace_back(Register("0x013902"));
}