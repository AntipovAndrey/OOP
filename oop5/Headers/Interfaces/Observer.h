#pragma once

#include <string>
#include <vector>

template<class R>
class Observer {
public:
    virtual void update(R message) = 0;
};

template<class R>
class Observable {
public:
    virtual void registerObserver(Observer<R> &o) = 0;

    virtual void removeObserver(Observer<R> &o) = 0;

    virtual void notifyObservers() = 0;

    virtual ~Observable() = default;

protected:
    std::vector<Observer<R> *> observers;
};