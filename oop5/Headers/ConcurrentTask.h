#pragma once

#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "Interfaces/Observer.h"
#include "Interfaces/Readable.h"
#include "Interfaces/Writable.h"
#include <algorithm>
#include <iostream>
#include <set>

template<typename T, typename R>
class ConcurrentTask : public Observable<R> {
public:
    ConcurrentTask(Readable<T> &input, Writable<R> &output, unsigned int threadCount, std::function<R(T)> task)
            : reader(input),
              writer(output),
              taskFunction(task),
              consumersNum(threadCount) {}

    ConcurrentTask(Readable<T> &input, Writable<R> &output, std::function<R(T)> task) : ConcurrentTask(input,
                                                                                                       output,
                                                                                                       std::thread::hardware_concurrency(),
                                                                                                       task) {}

    ConcurrentTask &operator=(const ConcurrentTask &) = delete;

    ConcurrentTask &operator=(ConcurrentTask &&) = delete;

    void registerObserver(Observer<R> &o) override;

    void removeObserver(Observer<R> &o) override;

    void notifyObservers() override;

    void start();

    void join();

    void pauseCalculations();

    void continueCalculations();

    void terminate();

    bool isWorking();

    virtual ~ConcurrentTask() {
        reader.stop();
        writer.stop();
    };

private:

    void producer();

    void consumer();

    void doTask(T taskData);

    void writeResult(R &&result);

    unsigned int consumersNum;

    Readable<T> &reader;
    Writable<R> &writer;

    std::queue<T> producedTasks;
    std::mutex lockerMutex;
    std::mutex writerMutex;
    std::mutex flagsMutex;
    std::condition_variable condVar;

    bool finished = false;
    bool paused = false;
    bool terminated = false;

    std::set<decltype(std::this_thread::get_id())> idPausedThreads;

    size_t unsolvedTasks = 0;

    std::vector<std::thread> pool;

    std::string messageForObservers = "";
    bool observersUpdated = false;

    std::function<R(T)> taskFunction;
};

template<typename T, typename R>
bool ConcurrentTask<T, R>::isWorking() {
    std::lock_guard<std::mutex> lockGuard(flagsMutex);
    if (!(finished && producedTasks.empty() || terminated) || paused) {
        return true;
    }
    if (!observersUpdated && unsolvedTasks == 0) {
        messageForObservers = "Done";
        notifyObservers();
        observersUpdated = true;
    }
    return false;
}


template<typename T, typename R>
void ConcurrentTask<T, R>::terminate() {
    std::lock_guard<std::mutex> lockGuard(flagsMutex);
    terminated = true;
    std::queue<T> empty;
    std::swap(producedTasks, empty);
    finished = true;
}

template<typename T, typename R>
void ConcurrentTask<T, R>::continueCalculations() {
    std::lock_guard<std::mutex> lockGuard(flagsMutex);
    if (paused) {
        paused = false;
        idPausedThreads.clear();
        writer.resume();
        reader.resume();
        condVar.notify_all();
    }
}


template<typename T, typename R>
void ConcurrentTask<T, R>::pauseCalculations() {
    std::lock_guard<std::mutex> lockGuard(flagsMutex);
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
    while (reader.hasNext()) {
        taskData = reader.next();
        std::unique_lock<std::mutex> lock(lockerMutex);
        if (terminated) {
            break;
        }
        while (paused) {
            reader.pause();
            condVar.wait(lock);
        }
        producedTasks.push(taskData);
        unsolvedTasks++;
        condVar.notify_all();
    }
    finished = true;
    reader.stop();
    condVar.notify_all();
}


template<typename T, typename R>
void ConcurrentTask<T, R>::consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(lockerMutex);
        if (!isWorking()) {
            break;
        }
        while (paused) {
            idPausedThreads.insert(std::this_thread::get_id());
            if (idPausedThreads.size() == consumersNum) {
                writer.pause();
            }
            condVar.wait(lock);
        }
        condVar.wait(lock, [this]() {
            return !producedTasks.empty() && !paused || finished;
        });
        while (!producedTasks.empty() && !paused) {
            T taskData = producedTasks.front();
            producedTasks.pop();
            lock.unlock();
            doTask(taskData);
            lock.lock();
            unsolvedTasks--;
        }
    }
    condVar.notify_all();
}


template<typename T, typename R>
void ConcurrentTask<T, R>::doTask(T taskData) {
    writeResult(taskFunction(taskData));
}


template<typename T, typename R>
void ConcurrentTask<T, R>::writeResult(R &&result) {
    std::lock_guard<std::mutex> guard(writerMutex);
    writer.store(std::forward<R>(result));
}


template<typename T, typename R>
void ConcurrentTask<T, R>::removeObserver(Observer<R> &o) {
    this->observers.erase(std::remove(std::begin(this->observers), std::end(this->observers), &o),
                          this->observers.end());
}

template<typename T, typename R>
void ConcurrentTask<T, R>::registerObserver(Observer<R> &o) {
    this->observers.push_back(&o);
}

template<typename T, typename R>
void ConcurrentTask<T, R>::notifyObservers() {
    for (auto item : this->observers) {
        item->update(messageForObservers);
    }
}