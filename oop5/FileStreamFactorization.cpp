//
// Created by andrey on 28.10.17.
//

#include <iostream>
#include <thread>
#include "FileStreamFactorization.h"
#include "ConcurrentTask.h"

// TODO:  encapsulate producer-consumer

FileStreamFactorization::FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename)
        : inputFilename(inputFilename), outputFilename(outputFilename) {}

void FileStreamFactorization::process() {
    // TODO:  dialog with user
    initialiseStreams();

    ConcurrentTask task(input, output, 4);
    task.start();

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


/**
9223372036854775807
9223372036854775807
 */