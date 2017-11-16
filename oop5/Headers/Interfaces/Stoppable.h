#pragma once

class Stoppable {
public:
    virtual void stop() = 0;

    virtual void pause() = 0;

    virtual void resume() = 0;
};