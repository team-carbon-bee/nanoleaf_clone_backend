#pragma once

#include <Arduino.h>

#include <LinkedList.h>
#include "IAnimation.h"
#include "referenceSystem/LinearReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/DividedCounter.h"

namespace animation
{

class PaintingLight : public IAnimation
{
    public:
        PaintingLight()
        {
        }

        virtual ~PaintingLight()
        {
        }

        std::string name() const
        {
            return "Painting lights";
        }

        void setup()
        {
        }

        //Called each time before starting animation
        void initialize()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds(); 
            
            _foreColor = PixelHelper::getRandomFullColor();
            _backgroundColor = PixelHelper::getRandomFullColorExcept(_foreColor);
            _curPos = 0;
            _divider.configure(random(1, 3));
        }

        //Called at the end of the animation
        virtual void deinitialize()
        {
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
            if (_divider.step())
            {
                //we draw the painted part
                for (int i = 0;  i < _curPos; ++i)
                    referenceSystem::LinearRef.setPixel(i, _foreColor);
                //we draw the background
                for (int i = _curPos; i < referenceSystem::LinearRef.ledCount(); ++i)
                    referenceSystem::LinearRef.setPixel(i, _backgroundColor);

                _curPos++;
                if (_curPos >= referenceSystem::LinearRef.ledCount())
                {
                    _curPos = 0;
                    _backgroundColor = _foreColor;
                    _foreColor = PixelHelper::getRandomFullColorExcept(_backgroundColor);
                }

                referenceSystem::LinearRef.driveLeds();
            }
        }

    private:
        Color _foreColor;
        Color _backgroundColor;
        int _curPos;
        DividedCounter _divider;
};

}