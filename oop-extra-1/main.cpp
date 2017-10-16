#include <iostream>
#include "RegistersParser.h"

using namespace std;

int main() {
    std::cout << "Test for RegistersParser" << std::endl;

    RegistersParser rp;

    //preamble
    rp.process(" R0 (INIT)	0x000090");
    rp.process("R0	0x000010");
    rp.process("R2	0x000200");
    rp.process("R3	0x000306");
    rp.process("R4	0x0004D0");
    rp.process("R5	0x00055B");
    rp.process("R6	0x000600");
    rp.process("R12	0x000C51");
    rp.process("R13	0x000D04");

    rp.process("R256	0x010018");
    rp.process("R257	0x010155");
    rp.process("R258	0x010255");
    rp.process("R259	0x010307");
    rp.process("R260	0x010400");


    // postamble
    rp.process("R8189	0x1FFD00");
    rp.process("R8190	0x1FFE00");
    rp.process("R8191	0x1FFF53");

    vector<vector<string>> v = rp.getParsedData();

    for_each(v.begin(), v.end(), [](vector<string> inner) {
        for_each(inner.begin(), inner.end(), [](string s) { cout << s << "  "; });
        cout << endl;
    });

    return 0;
}