#pragma once

#include <Arduino.h>

#include "IAnimation.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class FullColoredShapeFading : public IAnimation
{
    public:
        FullColoredShapeFading()
            : _currentShape(NULL)
        {
        }

        virtual ~FullColoredShapeFading()
        {
        }

        std::string name() const
        {
            return "Full colored shape fading";
        }

        void setup()
        {
        }

        //Called each time before starting animation
        void initialize()
        {
            referenceSystem::ShapeRef.clear();
            referenceSystem::ShapeRef.driveLeds();
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
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                //We manage to get randomly a new shape (not the last one)
                _currentShape = referenceSystem::ShapeRef.getRandomShapeExcept(_currentShape);
                //We take the color of the first pixel of the random shape to set the source
                Color src = referenceSystem::ShapeRef.getShape(_currentShape)->pixels()[0];
                Color dst = PixelHelper::getRandomFullColorExcept(src);
                                
                _fade.configure(src, dst, FadingDuration);
            }
            
            Color c = _fade.step();
            referenceSystem::ShapeRef.fill(_currentShape, c);
            referenceSystem::ShapeRef.driveLeds();
        }

    private:
        Fade _fade;
        Shape * _currentShape;
        static const int FadingDuration = 50;
};

}