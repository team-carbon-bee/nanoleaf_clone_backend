#pragma once

#include <Arduino.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"
#include "tools/DividedCounter.h"

namespace animation
{

class RainbowRunning : public IAnimation
{
    public:
        RainbowRunning(referenceSystem::LinearReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem), _leds(NULL)
        {
        }

        virtual ~RainbowRunning()
        {
        }

        std::string name()
        {
            return "Rainbow running";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds(); 
            
            _leds = (Color*)calloc(0, _referenceSystem->ledCount() * sizeof(Color));
            
            if (_leds)
            {
                int numberOfLedsByTransitionColor = 0;
                int restNumberOfLedsByTransitionColor = _referenceSystem->ledCount() % PixelHelper::FullColorNumber;
                int k=0;
                for (int i = 0; i < PixelHelper::FullColorNumber; ++i)
                {
                    //we compute the number of led in the color transition
                    numberOfLedsByTransitionColor = _referenceSystem->ledCount() / PixelHelper::FullColorNumber;
                    if (restNumberOfLedsByTransitionColor)
                    {
                        numberOfLedsByTransitionColor++;
                        restNumberOfLedsByTransitionColor--;
                    }
                    Fade f;
                    if (i == PixelHelper::FullColorNumber-1)
                        f.configure(PixelHelper::FullColorList[i], PixelHelper::FullColorList[0], numberOfLedsByTransitionColor);
                    else
                        f.configure(PixelHelper::FullColorList[i], PixelHelper::FullColorList[i + 1], numberOfLedsByTransitionColor);
                    
                    for (int j = 1; j <= numberOfLedsByTransitionColor; ++j)
                    {
                        _leds[k++] = f.step();
                    }
                    // Serial.printf("Number of transition: %d, reste: %d, k: %d\n\r", numberOfLedsByTransitionColor, restNumberOfLedsByTransitionColor, k);
                }
            }
            // for (int i = 0 ; i < _referenceSystem->ledCount() ; ++i)
            //     Serial.printf("leds[%d]=%06x, ", i, _leds[i]);

            _currentPos = 0;
        }

        void loop()
        {
            if (_leds)
            {
                if (_currentPos != _referenceSystem->ledCount()) {
                    for (int i = 0 ; i <= _currentPos ; ++i)
                        _referenceSystem->setPixel(i, _leds[_referenceSystem->ledCount()-_currentPos+i-1]);

                    _currentPos++;
                }
                else {
                    Color firstColor = _referenceSystem->getPixel(_referenceSystem->ledCount()-1);
                    for (int i = _referenceSystem->ledCount()-2 ; i >= 0; --i)
                        _referenceSystem->setPixel(i+1, _referenceSystem->getPixel(i));
                    _referenceSystem->setPixel(0, firstColor); 
                }

                _referenceSystem->driveLeds();
            }
        }

    private:

        referenceSystem::LinearReferenceSystem * _referenceSystem;
        Color * _leds;
        int _currentPos;
        DividedCounter _divider;
};

}