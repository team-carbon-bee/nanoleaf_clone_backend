#pragma once

#include <Arduino.h>

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
        Fireworks()
        {
        }

        virtual ~Fireworks()
        {
        }

        String name() const
        {
            return "Fireworks";
        }

        void setup()
        {
            referenceSystem::LinearRef.clear();
            referenceSystem::LinearRef.driveLeds(); 
            _divider.configure(random(1, 3));
        }

        //Called each time before starting animation
        void initialize()
        {
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
            //managing launched rockets
            if (_rockets.moveToStart())
            {
                do
                {
                    referenceSystem::LinearRef.setPixel(_rockets.getCurrent().position, _rockets.getCurrent().fade.step());
                    if (_rockets.getCurrent().fade.isFinished())
                    {
                        _rockets.DeleteCurrent();
                    }
                } while (_rockets.next());
            }
            
            //launch a new rocket ?
            if (_divider.step())
            {
                Rocket r;
                bool ok = false;
                while (not ok)
                {
                    //to be ok the random position mustnt't be in the rockets list
                    r.position = random(referenceSystem::LinearRef.ledCount());
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
                referenceSystem::LinearRef.setPixel(r.position, src);
                r.fade.configure(src, 0x000000, random(ShortestRocketsLightingTime, LongestRocketsLightingTime));

                _rockets.Append(r);

                //launch new rocket in ...
                _divider.configure(random(ShortestDelayBetweenTwoRockets, LongestDelayBetweenTwoRockets));
            }

            referenceSystem::LinearRef.driveLeds();
        }

    private:

        typedef struct 
        {
            int position;
            Fade fade;
        } Rocket;

        LinkedList<Rocket> _rockets;
        Color _backgroundColor;
        DividedCounter _divider;
        static const int TrailLength = 6;
        static const int BrightnessStep = -1 * 255 / TrailLength;
        static const int ShortestDelayBetweenTwoRockets = 2;
        static const int LongestDelayBetweenTwoRockets = 10;
        static const int ShortestRocketsLightingTime = 2;
        static const int LongestRocketsLightingTime = 15;
};

}