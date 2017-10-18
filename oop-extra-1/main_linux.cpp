/**
 *   Main program file only for linux
 *   Usage:         <executable_name> [parameters]
 *   Parameters:    -f source - set source file name
 *                  -h file - set file name of output header, default - <sourceName>.h
 *                  -i file - set file name of output ini file, default - <sourceName>.ini
 *                  Without -h and -i arguments you can set source file name without -f : <executable_name> <file>
 *
 *
 *
 */

#include <iostream>
#include <getopt.h>
#include "Headers/RegistersConverter.h"

int main(int argc, char *argv[]) {

    std::string fileName, headerName, iniName;
    bool customHeaderName = false, customIniName = false;

    if (argc > 2) {
        int optResult = 0;
        while ((optResult = getopt(argc, argv, "f:h:i:")) != -1) {
            switch (optResult) {
                case 'f':
                    fileName = std::string(optarg);
                    break;
                case 'h':
                    headerName = std::string(optarg);
                    customHeaderName = true;
                    break;
                case 'i':
                    iniName = std::string(optarg);
                    customIniName = true;
                    break;
                default:
                    std::cerr << "Incorrect syntax" << std::endl;
                    break;
            };
        };
    } else {
        if (argc == 2) {
            fileName = std::string(argv[argc - 1]);
        } else {
            std::cerr << "Too few arguments" << std::endl;
            std::cout << "Usage:\t\t<executable_name> [parameters]\n"
                    "Parameters:\n\t\t-f source - set source file name\n"
                    "\t\t-h file - set file name of output header, default - <sourceName>.h\n"
                    "\t\t-i file - set file name of output ini file, default - <sourceName>.ini\n"
                    "\t\tWithout -h and -i arguments you can set source file name without -f : <executable_name> <file>"
                      << std::endl;
            exit(1);
        }
    }

    RegistersConverter converter(fileName);

    if (!converter.parseFile()) {
        exit(3);
    }

    int returnStatusIni = 0;

    if (customIniName) {
        returnStatusIni = converter.saveAsIni(iniName) ? 0 : 1;
    } else {
        returnStatusIni = converter.saveAsIni() ? 0 : 1;
    }

    int returnStatusHeader = 0;

    if (customHeaderName) {
        returnStatusHeader = converter.saveAsHeader(headerName) ? 0 : 1;
    } else {
        returnStatusHeader = converter.saveAsHeader() ? 0 : 1;
    }

    return returnStatusIni + returnStatusHeader;
}