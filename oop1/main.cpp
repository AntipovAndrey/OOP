#include <iostream>
#include "IniParser.h"

using namespace std;

int main() {
    try {
        IniParser iniParser;
        iniParser.initialize("lab01.ini");
        cout << iniParser.getValue<string>("DEMODS", "FileName") << endl;
        cout << iniParser.toString() << endl;
        cout << iniParser.getValue<string>("DEMODS", "FileNames") << endl;
    } catch (NoSuchParameterException &e) {
        cout << e.what() << endl;
    }
    return 0;
}