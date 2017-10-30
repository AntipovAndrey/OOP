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
#include "ITaskObserver.h"
#include "ConcurrentTask.h"


class FileStreamFactorization : ITaskObserver {
public:
    FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename);

    void process();

    void enableDialog(bool enabled);

    void setThreadsCount(unsigned int count);

private:
    std::ifstream input;
    std::ofstream output;

    const std::string inputFilename;
    const std::string outputFilename;

    void initialiseStreams();

    void closeStreams();

    bool dialogEnabled = false;

    void startDialog(ConcurrentTask<uint64_t, std::string> &task);

    void update(std::string message) override;

    unsigned int threadsNumber = std::thread::hardware_concurrency();
};