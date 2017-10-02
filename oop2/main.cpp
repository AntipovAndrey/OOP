#include <iostream>
#include "StatisticMultiset.h"

using namespace std;

int main() {
    StatisticMultiset<float> statisticMultiset;


    for (int i = 4; i < 30; ++i) {
        statisticMultiset.addNum(static_cast<const float>(sin(i) * cos(2 * i)));
    }


    cout << statisticMultiset.getCountUnder(0.4) << endl;
    statisticMultiset.addNum(0.2);
    cout << statisticMultiset.getCountUnder(0.4) << endl;
    cout << statisticMultiset.getCountUnder(0.1) << endl;
    cout << statisticMultiset.getCountUnder(0.2) << endl;
    cout << statisticMultiset.getCountUnder(0.5) << endl;
    cout << statisticMultiset.getCountUnder(0.4) << endl;
    cout << statisticMultiset.getCountUnder(0.5) << endl;

    cout << statisticMultiset.getCountAbove(10) << endl;

    statisticMultiset.addNumsFromFile("numset");

    cout << statisticMultiset.getCountAbove(10) << endl;

    return 0;
}