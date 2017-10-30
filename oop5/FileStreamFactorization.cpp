//
// Created by andrey on 28.10.17.
//


#include "FileStreamFactorization.h"


FileStreamFactorization::FileStreamFactorization(const std::string &inputFilename, const std::string &outputFilename)
        : inputFilename(inputFilename), outputFilename(outputFilename) {}

void FileStreamFactorization::process() {
    initialiseStreams();

    ConcurrentTask<uint64_t, std::string> task(input, output, threadsNumber, [](uint64_t number) {
        try {
            IntegerFactorization factorization(number);
            factorization.calculate();
            return factorization.toString();
        } catch (IntegerFactorizationException &exception) {
            std::cerr << exception.what() << std::endl;
            return std::string("");
        }
    });

    task.registerObserver(*this);
    task.start();

    if (dialogEnabled) {
        startDialog(task);
    }

    task.join();

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

void FileStreamFactorization::enableDialog(bool enabled) {
    dialogEnabled = enabled;
}

void FileStreamFactorization::startDialog(ConcurrentTask<uint64_t, std::string> &task) {
    std::string answer;
    while (task.isWorking()) {
        std::cin >> answer;
        if (task.isWorking()) {
            if (answer == "exit") {
                task.terminate();
                return;
            }
            if (answer == "pause") {
                task.pauseCalculations();
            }
            if (answer == "continue") {
                task.continueCalculations();
            }
        }
    }
}

void FileStreamFactorization::setThreadsCount(unsigned int count) {
    if (count >= 1 && count <= 8) {
        threadsNumber = count;
    }
}

void FileStreamFactorization::update(std::string message) {
    if (dialogEnabled) {
        std::cout << message << std::endl;
    }
}

/** some large numbers here
9223372036854775807
9223372036854775807
 */