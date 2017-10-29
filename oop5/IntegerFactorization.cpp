//
// Created by andrey on 28.10.17.
//

#include <cmath>
#include <iostream>
#include <algorithm>
#include "IntegerFactorization.h"

IntegerFactorization::IntegerFactorization(uint64_t number) throw(IllegalArgumentException) : initialNumber(number) {
    if (number < 2)
        throw IllegalArgumentException("Number lesser than 2 cannot be factorized : " + std::to_string(number));
}

void IntegerFactorization::calculate() {
    uint32_t squareRoot = static_cast<uint32_t> (floorl(sqrtl(initialNumber)));
    uint64_t number = initialNumber;
    for (uint32_t i = 2; i <= squareRoot; i++) {
        while (number % i == 0) {
            factorizedData.push_back(i);
            number /= i;
        }
    }

    if (number != 1) {
        factorizedData.push_back(static_cast<int32_t>(number));
    }

    calculated = true;
}

std::string IntegerFactorization::toString() const throw(NotCalculatedException) {

    assertCalculated();

    std::string toReturn = "{\n\t\"number\": " + std::to_string(initialNumber) + ",\n\t\"factors\": [";

    std::string factors;
    for (auto num : factorizedData) {
        factors.append(std::to_string(num) + ", ");
    }

    factors.pop_back();
    factors.pop_back();


    toReturn.append(factors + "]\n}");

    return toReturn;
}

void IntegerFactorization::printFactorizedData() const throw(NotCalculatedException) {
    std::cout << toString() << std::endl;
}

bool IntegerFactorization::chechFactorization() const throw(NotCalculatedException) {

    uint64_t result = 1;

    for (auto num : factorizedData) {
        result *= num;
    }

    return result == initialNumber;
}

bool IntegerFactorization::isCalculated() const {
    return calculated;
}

void IntegerFactorization::assertCalculated() const throw(NotCalculatedException) {
    if (!isCalculated())
        throw NotCalculatedException("Number " + std::to_string(initialNumber) + " has not calculated yet");
}
