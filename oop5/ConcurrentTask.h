//
// Created by andrey on 29.10.17.
//

#pragma once

#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "ITaskObserver.h"
#include <algorithm>

class ConcurrentTask : ITaskObservable {
private:

    void producer();

    void consumer();

    void doTask(uint64_t task);


public:

    void registerObserver(ITaskObserver &o) override;

    void removeObserver(ITaskObserver &o) override;

    void notifyObservers() override;

    ConcurrentTask(std::ifstream &input, std::ofstream &output, int CONSUMERS_NUM);

    void start();

    void join();

    void pauseCalculations();

    void continueCalculations();

    void terminate();

    bool isWorking();

private:

    const int CONSUMERS_NUM;
    std::ifstream &input;
    std::ofstream &output;

    std::queue<uint64_t> producedTasks;
    std::mutex lockerMutex;
    std::mutex writerMutex;
    std::condition_variable condVar;

    volatile bool finished = false;
    volatile bool paused = false;
    volatile bool terminated = false;

    std::vector<std::thread> pool;

    std::string messageForObservers = "";
    bool observersUpdated = false;
};


