#pragma once

namespace animation
{

class IAnimation
{
    public:
        virtual void setup() = 0;
        virtual void loop() = 0;
};

}