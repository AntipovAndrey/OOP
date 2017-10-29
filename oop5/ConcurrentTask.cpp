//
// Created by andrey on 29.10.17.
//

#include <iostream>
#include <thread>
#include "ConcurrentTask.h"
#include "IntegerFactorization.h"

ConcurrentTask::ConcurrentTask(std::ifstream &input, std::ofstream &output, int CONSUMERS_NUM)
        : input(input), output(output), CONSUMERS_NUM(CONSUMERS_NUM) {

}

void ConcurrentTask::producer() {
    uint64_t number = 0;

    while (input >> number) {
        std::unique_lock<std::mutex> lock(lockerMutex);
        producedTasks.push(number);
        condVar.notify_all();
    }

    finished = true;
    condVar.notify_all();
}

void ConcurrentTask::consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(lockerMutex);
        condVar.wait(lock, [this]() {
            return !producedTasks.empty();
        });
        lock.unlock();
        while (!producedTasks.empty()) {
            lockerMutex.lock();
            uint64_t task = producedTasks.front();
            producedTasks.pop();
            //       std::cout << std::this_thread::get_id() <<  " begin task " << task << std::endl;
            lockerMutex.unlock();
            daTask(task);
            //      std::cout << std::this_thread::get_id() <<  " END TASK " << task << std::endl;
        }
        if (finished) break;
    }
}


void ConcurrentTask::daTask(uint64_t task) {
    try {
        IntegerFactorization factorization(task);
        factorization.calculate();
        std::lock_guard<std::mutex> guard(writerMutex);
        output << factorization.toString() << std::endl;
        output.flush();
    } catch (IntegerFactorizationException &exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void ConcurrentTask::start() {
    std::vector<std::thread> pool;
    pool.emplace_back(&ConcurrentTask::producer, this);
    for (int i = 0; i < CONSUMERS_NUM; ++i) {
        pool.emplace_back(&ConcurrentTask::consumer, this);
    }
    for (auto &t : pool) {
        t.join();
    }
}



