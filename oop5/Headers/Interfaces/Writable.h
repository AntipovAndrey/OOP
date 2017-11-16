#pragma once

#include "Stoppable.h"
#include "Storable.h"

template<class T>
class Writable : public Stoppable, public Storable<T> {
};