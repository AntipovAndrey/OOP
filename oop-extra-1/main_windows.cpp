/**
 *  Main program file for windows (vs compiler)
 *  Usage - <program_name> <file_name>
 *
 *
 */

#include <fstream>
#include <iostream>
#include "Headers/RegistersConverter.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect Format" << std::endl;
        exit(4);
    }
    RegistersConverter converter(argv[1]);

    if (!converter.parseFile()) {
        exit(3);
    }

    int returnStatusIni = 0;
    returnStatusIni = converter.saveAsIni() ? 0 : 1;
    int returnStatusHeader = 0;
    returnStatusHeader = converter.saveAsHeader() ? 0 : 1;
    return returnStatusIni + returnStatusHeader;
}
