#include <iostream>
#include "StatisticMultiset.h"

using namespace std;

int main() {
    StatisticMultiset<float> statisticMultiset = {2, 6, 9, 233};

    for (int i = 4; i < 30; ++i) {
        statisticMultiset.addNum(static_cast<const float>(sin(i) * cos(2 * i)));
    }

    vector<float> floats = {1, 2, 3, 4};

    statisticMultiset.addNum(floats);

    cout << statisticMultiset.getCountUnder(0.4) << endl;
    statisticMultiset.addNum(0.2);
    cout << statisticMultiset.getCountUnder(0.4) << endl;
    cout << statisticMultiset.getCountUnder(0.1) << endl;
    cout << statisticMultiset.getCountUnder(0.2) << endl;
    cout << statisticMultiset.getCountUnder(0.5) << endl;
    cout << statisticMultiset.getCountUnder(0.4) << endl;
    cout << statisticMultiset.getCountUnder(0.5) << endl;
    statisticMultiset.addNums(statisticMultiset);
    cout << statisticMultiset.getCountAbove(10) << endl;

    statisticMultiset.addNumsFromFile("numset");

    cout << statisticMultiset.getCountAbove(10) << endl;

    return 0;
    }