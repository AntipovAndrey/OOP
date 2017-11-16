#pragma once

template<class T>
class Storable {
public:
    virtual void store(T &&data) = 0;
};