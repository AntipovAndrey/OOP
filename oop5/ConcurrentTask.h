//
// Created by andrey on 30.10.17.
//

#pragma once

#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "ITaskObserver.h"
#include <algorithm>
#include <iostream>

template<typename T, typename R>
class ConcurrentTask : ITaskObservable {
public:
    ConcurrentTask(std::ifstream &input, std::ofstream &output, unsigned int threadCount, std::function<R(T)> task)
            : input(input),
              output(output),
              taskFunction(task),
              consumersNum(threadCount) {}

    ConcurrentTask(std::ifstream &input, std::ofstream &output, std::function<R(T)> task) : ConcurrentTask(input,
                                                                                                           output,
                                                                                                           std::thread::hardware_concurrency(),
                                                                                                           task) {}

    void registerObserver(ITaskObserver &o) override;

    void removeObserver(ITaskObserver &o) override;

    void notifyObservers() override;

    void start();

    void join();

    void pauseCalculations();

    void continueCalculations();

    void terminate();

    bool isWorking();

private:

    void producer();

    void consumer();

    void doTask(T taskData);

    void writeResult(R result);

    unsigned int consumersNum;
    std::ifstream &input;
    std::ofstream &output;

    std::queue<T> producedTasks;
    std::mutex lockerMutex;
    std::mutex writerMutex;
    std::condition_variable condVar;

    volatile bool finished = false;
    volatile bool paused = false;
    volatile bool terminated = false;

    std::vector<std::thread> pool;

    std::string messageForObservers = "";
    bool observersUpdated = false;

    std::function<R(T)> taskFunction;
};

template<typename T, typename R>
bool ConcurrentTask<T, R>::isWorking() {
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


template<typename T, typename R>
void ConcurrentTask<T, R>::terminate() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    terminated = true;
    std::queue<T> empty;
    std::swap(producedTasks, empty);
    finished = true;
}

template<typename T, typename R>
void ConcurrentTask<T, R>::continueCalculations() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    if (paused) {
        paused = false;
        condVar.notify_all();
    }
}


template<typename T, typename R>
void ConcurrentTask<T, R>::pauseCalculations() {
    std::lock_guard<std::mutex> lockGuard(lockerMutex);
    paused = true;
}


template<typename T, typename R>
void ConcurrentTask<T, R>::start() {
    pool.emplace_back(&ConcurrentTask::producer, this);
    for (int i = 0; i < consumersNum; ++i) {
        pool.emplace_back(&ConcurrentTask::consumer, this);
    }
}


template<typename T, typename R>
void ConcurrentTask<T, R>::join() {
    for (auto &t : pool) {
        if (t.joinable()) {
            t.join();
        }
    }
}


template<typename T, typename R>
void ConcurrentTask<T, R>::producer() {
    T taskData;
    while (input >> taskData) {
        std::unique_lock<std::mutex> lock(lockerMutex);
        condVar.wait(lock, [this]() { return !paused; });
        producedTasks.push(taskData);
        condVar.notify_all();
    }
    finished = true;
    condVar.notify_all();
}


template<typename T, typename R>
void ConcurrentTask<T, R>::consumer() {
    while (true) {
        if (!isWorking()) {
            return;
        }
        std::unique_lock<std::mutex> lock(lockerMutex);
        condVar.wait(lock, [this]() {
            return !producedTasks.empty() && !paused || !finished;
        });
        while (!producedTasks.empty() && !paused) {
            T taskData = producedTasks.front();
            producedTasks.pop();
            lock.unlock();
            doTask(taskData);
            lock.lock();
        }
    }
}


template<typename T, typename R>
void ConcurrentTask<T, R>::doTask(T taskData) {
    R result = taskFunction(taskData);
    writeResult(result);
}


template<typename T, typename R>
void ConcurrentTask<T, R>::writeResult(R result) {
    std::lock_guard<std::mutex> guard(writerMutex);
    output << result << std::endl;
    output.flush();
}


template<typename T, typename R>
void ConcurrentTask<T, R>::removeObserver(ITaskObserver &o) {
    observers.erase(std::remove(std::begin(observers), std::end(observers), &o), observers.end());
}

template<typename T, typename R>
void ConcurrentTask<T, R>::registerObserver(ITaskObserver &o) {
    observers.push_back(&o);
}

template<typename T, typename R>
void ConcurrentTask<T, R>::notifyObservers() {
    for (auto item : observers) {
        item->update(messageForObservers);
    }
}