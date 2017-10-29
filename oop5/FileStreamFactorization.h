//
// Created by andrey on 28.10.17.
//

#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "IntegerFactorization.h"


class FileStreamFactorization {
public:
    FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename);

    void process();

private:
    std::ifstream input;
    std::ofstream output;

    const std::string inputFilename;
    const std::string outputFilename;

    void initialiseStreams();

    void closeStreams();
};
