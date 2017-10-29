//
// Created by andrey on 29.10.17.
//

#pragma once

#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>

class ConcurrentTask {
private:

    void producer();

    void consumer();

    void daTask(uint64_t task);

public:

    ConcurrentTask(std::ifstream &input, std::ofstream &output, int CONSUMERS_NUM);

    void start();

private:

    const int CONSUMERS_NUM;

    std::ifstream &input;
    std::ofstream &output;

    std::queue<uint64_t> producedTasks;
    std::mutex lockerMutex;
    std::mutex writerMutex;
    std::condition_variable condVar;
    volatile bool finished = false;
    volatile bool notified = false;
};


