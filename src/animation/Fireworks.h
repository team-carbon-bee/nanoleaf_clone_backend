#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/DividedCounter.h"
#include "tools/Fade.h"

namespace animation
{

class Fireworks : public IAnimation
{
    public:
        Fireworks(referenceSystem::LinearReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem)
        {
        }

        virtual ~Fireworks()
        {
        }

        std::string name()
        {
            return "Fireworks";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds(); 
            _divider.configure(random(1, 3));
        }

        void loop()
        {
            Serial.printf("loop\n");
            //managing launched rockets
            if (_rockets.moveToStart())
            {
                do
                {
                    _referenceSystem->setPixel(_rockets.getCurrent().position, _rockets.getCurrent().fade.step());
                    if (_rockets.getCurrent().fade.isFinished())
                    {
                        _rockets.DeleteCurrent();
                        Serial.printf("Suppr rocket, reste %d elem\n", _rockets.size());
                    }
                } while (_rockets.next());
            }
            
            //launch a new rocket ?
            if (_divider.step())
            {
                Serial.printf("new rocket\n");
                Rocket r;
                bool ok = false;
                while (not ok)
                {
                    //to be ok the random position mustnt't be in the rockets list
                    r.position = random(_referenceSystem->ledCount());
                    ok = true;
                    if (_rockets.moveToStart())
                    {
                        do
                        {
                           if (_rockets.getCurrent().position == r.position)
                                ok = false;
                        } while ((_rockets.next()) && (ok));
                    }
                }
                
                Color src = PixelHelper::getRandomFullColor();
                //launch the rocket
                _referenceSystem->setPixel(r.position, src);
                r.fade.configure(src, 0x000000, random(ShortestRocketsLightingTime, LongestRocketsLightingTime));

                _rockets.Append(r);

                //launch new rocket in ...
                _divider.configure(random(ShortestDelayBetweenTwoRockets, LongestDelayBetweenTwoRockets));
            }

            _referenceSystem->driveLeds();
        }

    private:

        typedef struct 
        {
            int position;
            Fade fade;
        } Rocket;

        referenceSystem::LinearReferenceSystem * _referenceSystem;
        LinkedList<Rocket> _rockets;
        Color _backgroundColor;
        DividedCounter _divider;
        static const int TrailLength = 6;
        static const int BrightnessStep = -1 * 255 / TrailLength;
        static const int ShortestDelayBetweenTwoRockets = 10;
        static const int LongestDelayBetweenTwoRockets = 20;
        static const int ShortestRocketsLightingTime = 2;
        static const int LongestRocketsLightingTime = 10;
};

}