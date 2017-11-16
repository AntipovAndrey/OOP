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

    IntegerFactorization() = default;

    IntegerFactorization(const IntegerFactorization &) = default;

    IntegerFactorization(IntegerFactorization &&) noexcept = default;

    IntegerFactorization &operator=(const IntegerFactorization &) = default;

    IntegerFactorization &operator=(IntegerFactorization &&) noexcept = default;

    void calculate();

    std::string toString() const throw(NotCalculatedException);

    void printFactorizedData() const throw(NotCalculatedException);

private:

    bool checkFactorization() const throw(NotCalculatedException);

    void assertCalculated() const throw(NotCalculatedException);

    bool isCalculated() const;

    uint64_t initialNumber = 0;

    bool calculated = false;

    std::vector<uint64_t> factorizedData;
};