
#include <iostream>
#include "FileStreamFactorization.h"

int main() {

    FileStreamFactorization factorization("test.txt", "out.txt");
    factorization.process();

    return 0;
}