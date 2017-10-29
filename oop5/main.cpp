
#include <iostream>
#include "FileStreamFactorization.h"

int main() {
    FileStreamFactorization factorization("test.txt", "out.txt");

    std::cout
            << "Do you want to enable dialog between calculations?" << std::endl
            << "You could input [exit|pause|continue]. Enter Y if you want it"
            << std::endl;

    std::string answer;
    std::cin >> answer;
    if (answer == "y" || answer == "Y") factorization.enableDialog(true);


    std::cout
            << "How many threads do you want to use?" << std::endl
            << "Prefer number is: " << std::thread::hardware_concurrency() << ". Enter number between 1 and 8"
            << std::endl;

    std::cin >> answer;
    factorization.setThreadsCount(static_cast<unsigned int>(std::stoul(answer)));

    factorization.process();

    return 0;
}