//
// Created by andrey on 28.10.17.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "IntegerFactorizationExceptions.h"

class IntegerFactorization {
public:

    explicit IntegerFactorization(uint64_t number) throw(IllegalArgumentException);

    void calculate();

    std::string toString() const throw(NotCalculatedException);

    void printFactorizedData() const throw(NotCalculatedException);

private:

    bool chechFactorization() const throw(NotCalculatedException);

    const uint64_t initialNumber;

    bool calculated = false;

    std::vector<uint32_t> factorizedData;

    bool isCalculated() const;

    void assertCalculated() const throw(NotCalculatedException);
};


