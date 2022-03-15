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
        RainbowRunning()
        : _leds(NULL)
        {
        }

        virtual ~RainbowRunning()
        {
        }

        String name() const
        {
            return "Rainbow running";
        }

        uint8_t id() const
        {
            return 15;
        }

        void setup()
        {
        }

        //Called each time before starting animation
        void initialize()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds(); 
            
            _leds = (Color*)calloc(referenceSystem::LinearRef.ledCount(), sizeof(Color));
            
            if (_leds)
            {
                int numberOfLedsByTransitionColor = 0;
                int restNumberOfLedsByTransitionColor = referenceSystem::LinearRef.ledCount() % PixelHelper::FullColorNumber;
                int k=0;
                for (int i = 0; i < PixelHelper::FullColorNumber; ++i)
                {
                    //we compute the number of led in the color transition
                    numberOfLedsByTransitionColor = referenceSystem::LinearRef.ledCount() / PixelHelper::FullColorNumber;
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
                }
            }

            _currentPos = 0;
        }

        //Called at the end of the animation
        virtual void deinitialize()
        {
            free(_leds);
        }
                
        //Determine if the animation can be ended by itself
        virtual bool canFinish() const
        {
            return false;
        }

        //Check if the animation has finished if it can false otherwise
        virtual bool isFinished() const
        {
            return false;
        }

        void loop()
        {
            if (_leds)
            {
                if (_currentPos != referenceSystem::LinearRef.ledCount()) {
                    for (int i = 0 ; i <= _currentPos ; ++i)
                        referenceSystem::LinearRef.setPixel(i, _leds[referenceSystem::LinearRef.ledCount()-_currentPos+i-1]);

                    _currentPos++;
                }
                else {
                    Color firstColor = referenceSystem::LinearRef.getPixel(referenceSystem::LinearRef.ledCount()-1);
                    for (int i = referenceSystem::LinearRef.ledCount()-2 ; i >= 0; --i)
                        referenceSystem::LinearRef.setPixel(i+1, referenceSystem::LinearRef.getPixel(i));
                    referenceSystem::LinearRef.setPixel(0, firstColor); 
                }

                referenceSystem::LinearRef.driveLeds();
            }
        }

    private:

        Color * _leds;
        int _currentPos;
        DividedCounter _divider;
};

}