#pragma once

#include <Arduino.h>

class ShapeProvider
{
    public:
        ShapeProvider();
        virtual ~ShapeProvider();

        void loadFromFlash();
        void load();

    private:
        void parseJson();

};