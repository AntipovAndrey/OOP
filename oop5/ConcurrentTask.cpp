//
// Created by andrey on 29.10.17.
//

#include <iostream>
#include <thread>
#include "ConcurrentTask.h"
#include "IntegerFactorization.h"

ConcurrentTask::ConcurrentTask(std::ifstream &input, std::ofstream &output, int CONSUMERS_NUM)
        : input(input), output(output), CONSUMERS_NUM(CONSUMERS_NUM) {}

void ConcurrentTask::producer() {
    uint64_t number = 0;

    while (input >> number) {
        std::unique_lock<std::mutex> lock(lockerMutex);
        condVar.wait(lock, [this]() { return !paused; });
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
            return !producedTasks.empty() && !paused;
        });
        while (!producedTasks.empty() && !paused) {
            uint64_t task = producedTasks.front();
            producedTasks.pop();
            // std::cout << std::this_thread::get_id() << " begin task " << task << std::endl;
            lockerMutex.unlock();
            doTask(task);
            // std::cout << std::this_thread::get_id() << " END TASK " << task << std::endl;
        }
        if (!isWorking()) {
            break;
        }
    }
}


void ConcurrentTask::doTask(uint64_t task) {
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
    pool.emplace_back(&ConcurrentTask::producer, this);
    for (int i = 0; i < CONSUMERS_NUM; ++i) {
        pool.emplace_back(&ConcurrentTask::consumer, this);
    }
}

void ConcurrentTask::join() {
    for (auto &t : pool) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ConcurrentTask::pauseCalculations() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    paused = true;
}

void ConcurrentTask::continueCalculations() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    if (paused) {
        paused = false;
        condVar.notify_all();
    }
}

void ConcurrentTask::terminate() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    terminated = true;
    std::queue<uint64_t> empty;
    std::swap(producedTasks, empty);
    finished = true;
}

bool ConcurrentTask::isWorking() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    if (!(finished && producedTasks.empty() || terminated) || paused) {
        return true;
    }
    if (!observersUpdated) {
        messageForObservers = "Done! Enter anything to continue";
        notifyObservers();
        observersUpdated = true;
    }
    return false;
}

void ConcurrentTask::registerObserver(ITaskObserver &o) {
    observers.push_back(&o);
}

void ConcurrentTask::removeObserver(ITaskObserver &o) {
    observers.erase(std::remove(std::begin(observers), std::end(observers), &o), observers.end());
}

void ConcurrentTask::notifyObservers() {
    for (auto item : observers) {
        item->update(messageForObservers);
    }
}
