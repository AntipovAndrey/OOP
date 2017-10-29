//
// Created by andrey on 29.10.17.
//

#pragma once

#include <string>
#include <vector>

class ITaskObserver {
public:
    virtual void update(std::string message) = 0;
};

class ITaskObservable {
public:
    virtual void registerObserver(ITaskObserver &o) = 0;

    virtual void removeObserver(ITaskObserver &o) = 0;

    virtual void notifyObservers() = 0;

    virtual ~ITaskObservable() = default;

protected:
    std::vector<ITaskObserver *> observers;
};
