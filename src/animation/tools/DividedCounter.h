#pragma once

#include <Arduino.h>

namespace animation
{

class DividedCounter
{
    public:
        DividedCounter() : _divider(0), _counter(0)
        {
        }

        virtual ~DividedCounter()
        {
        }

        void configure(const int divider)
        {
            _divider = divider;
            _counter = 0;
        }

        //return true 1 time by divider
        bool step()
        {
            _counter++;
            if (_counter == _divider)
            {
                _counter = 0;
                return true;
            }
            return false;
        }

        int counter() const 
        {
            return _counter;
        }

    private:
        int _divider;
        int _counter;

};

}