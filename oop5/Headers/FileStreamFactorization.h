#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "IntegerFactorization.h"
#include "Interfaces/Observer.h"
#include "ConcurrentTask.h"


class FileStreamFactorization : public Observer<std::string> {
public:
    FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename);

    void process();

    void enableDialog(bool enabled);

    void setThreadsCount(unsigned int count);

    FileStreamFactorization &operator=(FileStreamFactorization &&) = delete;

    FileStreamFactorization &operator=(const FileStreamFactorization &) = delete;

private:

    template<class T>
    class FileWriter : public Writable<T> {
    public:
        explicit FileWriter(const std::string &name) : output(name), name(name) {};

        void stop() override {
            output.flush();
            output.close();
        };

        void pause() override {
            if (!paused) {
                stop();
                paused = true;
            }
        };

        void resume() override {
            if (paused) {
                output.open(name, std::ofstream::app);
                paused = false;
            }
        };

        void store(T &&data) override {
            output << data << std::endl;
        };

    private:
        std::ofstream output;

        std::string name;

        bool paused = false;
    };

    template<class R>
    class FileReader : public Readable<R> {
    public:
        explicit FileReader(const std::string &name) : input(name) {};

        bool hasNext() override {
            if (input >> temp) return true;
            return false;
        };

        R next() override {
            return temp;
        };

        void stop() override {
            input.close();
        };

        void pause() override {};

        void resume() override {};

    private:
        std::ifstream input;

        uint64_t temp{};
    };

    const std::string inputFilename;
    const std::string outputFilename;

    bool dialogEnabled = false;

    void startDialog(ConcurrentTask<uint64_t, std::string> &task);

    void update(std::string message) override;

    unsigned int threadsNumber = std::thread::hardware_concurrency();
};