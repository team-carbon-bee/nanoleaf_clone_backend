#pragma once

#include <Arduino.h>

namespace referenceSystem {

class IReferenceSystem
{
    public:
        virtual void clear() = 0;
        virtual void driveLeds()=0;
};

}