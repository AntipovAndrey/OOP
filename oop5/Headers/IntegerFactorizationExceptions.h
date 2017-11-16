//
// Created by andrey on 28.10.17.
//

#pragma once

#include <stdexcept>

class IntegerFactorizationException : public std::runtime_error {
public:
    explicit IntegerFactorizationException(const std::string &__arg) : runtime_error(__arg) {}
};

class NotCalculatedException : public IntegerFactorizationException {
public:
    explicit NotCalculatedException(const std::string &msg) : IntegerFactorizationException(msg) {};
};

class IllegalArgumentException : public IntegerFactorizationException {
public:
    explicit IllegalArgumentException(const std::string &msg) : IntegerFactorizationException(msg) {};
};