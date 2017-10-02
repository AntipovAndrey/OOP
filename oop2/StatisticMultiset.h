//
// Created by andrey on 09.09.17.
//

#pragma once

#include <set>
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>

template<typename T>
class StatisticMultiset {
public:
    void addNum(const T &value) {
        if (data.empty()) {
            maxValue = value;
            minValue = value;
        } else {
            if (value > maxValue) maxValue = value;
            if (value < minValue) minValue = value;
        }

        updateCaches(value);

        data.insert(value);
        avgIsOutdated = true;
    }

    void addNum(const std::multiset<T> &numbers) {
        for (auto &it : numbers) {
            addNum(it);
        }
    }

    void addNum(const std::vector<T> &numbers) {
        for (auto &it : numbers) {
            addNum(it);
        }
    }

    void addNum(const std::list<T> &numbers) {
        for (auto &it : numbers) {
            addNum(it);
        }
    }

    void addNums(const StatisticMultiset &startSet) {
        addNum(startSet.data);
    }

    void addNumsFromFile(const char *fileName) {
        std::fstream file;
        file.open(fileName);
        if (!file.good()) return;
        T x;
        while (file >> x) {
            addNum(x);
        }
    }

    T getMax() const {
        return maxValue;
    }

    T getMin() const {
        return minValue;
    }

    float getAvg() const {
        if (!avgIsOutdated) {
            return lastAverageResult;
        }
        double average = 0;
        for (auto &val : data) {
            average += val / static_cast<float>(data.size());
        }
        avgIsOutdated = false;
        return static_cast<float>(average);
    }

    int getCountUnder(float threshold) const {
        if (cacheUnder.count(threshold)) {
            return cacheUnder[threshold];
        }

        unsigned int counter = 0;

        for (auto &val : data) {
            if (val < threshold) counter++;
        }

        updateUnderCache(counter, threshold);

        return counter;
    }

    int getCountAbove(float threshold) const {
        if (cacheAbove.count(threshold)) return cacheAbove[threshold];

        unsigned int counter = 0;

        for (auto &val : data) {
            if (val > threshold) counter++;
        }

        updateAboveCache(counter, threshold);

        return counter;
    }

private:
    std::multiset<T> data;
    T minValue, maxValue;
    mutable float lastAverageResult = static_cast<float>(std::nan(""));
    mutable bool avgIsOutdated = true;
    const int cacheSize = 3;
    mutable std::map<T, unsigned int> cacheUnder, cacheAbove;
    mutable std::queue<T> queueUnder, queueAbove;

    void updateUnderCache(unsigned int counter, float threshold) const {
        queueUnder.push(threshold);
        cacheUnder[threshold] = counter;

        if (queueUnder.size() > cacheSize) {
            cacheUnder.erase(queueUnder.front());
            queueUnder.pop();
        }
    }

    void updateAboveCache(unsigned int counter, float threshold) const {
        queueAbove.push(threshold);
        cacheAbove[threshold] = counter;

        if (queueAbove.size() > cacheSize) {
            cacheAbove.erase(queueAbove.front());
            queueAbove.pop();
        }
    }

    void updateCaches(const T &value) {
        for (auto &it : cacheUnder) {
            if (it.first > value && cacheUnder.count(it.first)) {
                cacheUnder[it.first] = cacheUnder[it.first] + 1;
            }
        }

        for (auto &it : cacheAbove) {
            if (it.first < value && cacheAbove.count(it.first)) {
                cacheAbove[it.first] = cacheAbove[it.first] + 1;
            }
        }
    }
};