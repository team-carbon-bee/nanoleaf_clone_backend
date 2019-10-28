#pragma once

#include <Arduino.h>

namespace referenceSystem {

class IReferenceSystem
{
    public:
        virtual void setup() = 0;
        virtual void clear() = 0;
        virtual void driveLeds()=0;
};

}