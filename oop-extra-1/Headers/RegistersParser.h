/**
 *  RegistersParser class.
 *  Class for encapsulate processing with lines with pattern : <register name><tab><adr1><adr2><command>
 *  R0-R13 registers will be replaced with preamble, R8189-R8191 - with postamble
 *
 */
#pragma once

#include <string>
#include <vector>
#include <regex>

class RegistersParser {
public:
    void process(const std::string &line);

    std::vector<std::vector<std::string>> getParsedData() const;

private:

    bool isPreamble(const std::string &line) const;

    bool isPostamble(const std::string &line) const;

    class Register {
    public:
        explicit Register(const std::string &correctLine);

        std::vector<std::string> asStrings() const;

    private:
        uint8_t firstAddress;
        uint8_t secondAddress;
        uint8_t command;
    };

    static const std::regex registerPattern;
    static const std::regex registerNamePattern;
    static const std::regex registerDataPattern;

    mutable bool firstPush = true, lastPush = false;

    std::vector<Register> repository;

    void fillWithPreamble();

    void fillWithPostamble();
};


