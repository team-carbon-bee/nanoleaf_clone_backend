#pragma once

#include <Arduino.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/DividedCounter.h"

namespace animation
{

class RunningLight : public IAnimation
{
    public:
        RunningLight()
        {
        }

        virtual ~RunningLight()
        {
        }

        std::string name() const
        {
            return "Running lights";
        }

        void setup()
        {
           
        }

        //Called each time before starting animation
        void initialize()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds(); 
            
            Color foreColor = PixelHelper::getRandomFullColor();
            _backgroundColor = PixelHelper::getRandomFullColorExcept(foreColor);

            for (int i = 0; i < TrailLength; ++i)
            {
                Light l;
                l.position = i;
                //more we advance in the strip more the light is brighten
                l.color = PixelHelper::brightenPixel(foreColor, (TrailLength - i - 1) * BrightnessStep);
                _trail.Append(l);
            }

            _divider.configure(random(1, 3));
            _turns = 0;
            _numberOfTurns = random(MinNumberOfTurns, MaxNumberOfTurns);
        }

        //Called at the end of the animation
        virtual void deinitialize()
        {
            _trail.Clear();
        }
                
        //Determine if the animation can be ended by itself
        virtual bool canFinish() const
        {
            return true;
        }

        //Check if the animation has finished if it can false otherwise
        virtual bool isFinished() const
        {
            return (_turns >= _numberOfTurns);
        }

        void loop()
        {
            if ((not isFinished()) && (_divider.step()))
            {
                //we draw the background
                referenceSystem::LinearRef.fill(_backgroundColor);
                if (_trail.moveToStart())
                {
                    do
                    {
                        referenceSystem::LinearRef.setPixel(_trail.getCurrent().position, _trail.getCurrent().color);
                        //go ahead for next time
                        _trail.getCurrent().position++;
                        if (_trail.getCurrent().position >= referenceSystem::LinearRef.ledCount())
                        {
                            //instead of setting to 0 we make following computation to allow 
                            //other steps than one when go ahead
                            _trail.getCurrent().position = _trail.getCurrent().position - referenceSystem::LinearRef.ledCount();
                            _turns++;
                        }
                    } while (_trail.next());
                }

                referenceSystem::LinearRef.driveLeds();
            }
        }

    private:

        typedef struct 
        {
            int position;
            Color color;
        } Light;
        
        LinkedList<Light> _trail;
        Color _backgroundColor;
        DividedCounter _divider;
        int _turns;
        int _numberOfTurns;
        static const int TrailLength = 6;
        static const int BrightnessStep = -1 * 255 / TrailLength;
        static const int MinNumberOfTurns = 3;
        static const int MaxNumberOfTurns = 10;
};

}