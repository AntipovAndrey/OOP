#pragma once

template<class R>
class Iterable {
public:
    virtual bool hasNext() = 0;

    virtual R next() = 0;
};