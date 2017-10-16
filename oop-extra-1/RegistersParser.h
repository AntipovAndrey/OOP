/**
 *  RegistersParser class.
 *  Class for encapsulate processing with lines with pattern : <register name><tab><adr1><adr2><command>
 *  R0-R13 registers will be replaced with preamble, R8189-R8191 - with postamble
 *
 */
#pragma once

#include <string>
#include <vector>

class RegistersParser {
public:
    void process(const std::string &line);

    std::string getNextString() const;

private:

    bool isPreamble(const std::string &line) const;

    bool isPostamble(const std::string &line) const;

    class Register {
    public:
        Register(std::string &name, uint8_t firstAddress, uint8_t secondAddress, uint8_t command)
                : name(name), firstAddress(firstAddress), secondAddress(secondAddress), command(command) {}

        std::string toString() const;

    private:
        std::string &name;
        uint8_t firstAddress;
        uint8_t secondAddress;
        uint8_t command;
    };

    std::vector<Register> repository;
    std::vector<Register>::iterator nextRegister;
};


