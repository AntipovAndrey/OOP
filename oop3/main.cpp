#include <iostream>
#include "Wav.h"

int main() {
    try {
        Wav testWav("0.wav");

        std::cout << testWav.getDescription() << std::endl;

        testWav.applyReverberationByChannel(0.07, 0.6, Wav::leftChannel);

        testWav.applyReverberationByChannel(0.05, 0.5, Wav::rightChannel);

        testWav.cutLastSeconds(20);

        testWav.cutFirstSeconds(2);

        std::cout << testWav.getDescription() << std::endl;

        testWav.makeWavFile("lol.wav");

        testWav.makeMono();

        testWav.makeWavFile("lol_mono.wav");

    } catch (WavException &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}