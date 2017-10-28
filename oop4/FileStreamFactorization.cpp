//
// Created by andrey on 28.10.17.
//

#include <iostream>
#include "FileStreamFactorization.h"


FileStreamFactorization::FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename)
        : inputFilename(inputFilename), outputFilename(outputFilename) {}

void FileStreamFactorization::process() {
    initialiseStreams();

    uint64_t number = 0;
    while (input >> number) {
        IntegerFactorization factorization(number);
        factorization.calculate();
        output << factorization.toString() << std::endl;
        output.flush();
    }
    closeStreams();
}

void FileStreamFactorization::closeStreams() {
    input.close();
    output.close();
}

void FileStreamFactorization::initialiseStreams() {
    input = std::ifstream(inputFilename);
    output = std::ofstream(outputFilename);
}
