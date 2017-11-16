#pragma once

#include "Iterable.h"
#include "Stoppable.h"

template<class R>
class Readable : public Iterable<R>, public Stoppable {
};