#pragma once

namespace animation
{

class IAnimation
{
    public:
        //Called after the animation is instanciated
        virtual void setup() = 0;

        //Called each time before starting animation
        virtual void initialize() = 0;

        //Called at the end of the animation
        virtual void deinitialize() = 0;

        //Loop of the animation
        virtual void loop() = 0;

        //Name of the animation
        virtual String name() const = 0;

        //Determine if the animation can be ended by itself
        virtual bool canFinish() const = 0;

        //Check if the animation has finished if it can false otherwise
        virtual bool isFinished() const = 0;
};

}