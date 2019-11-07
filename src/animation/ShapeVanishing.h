#pragma once

#include <Arduino.h>

#include <LinkedList.h>

#include "IAnimation.h"
#include "referenceSystem/ShapeReferenceSystem.h"
#include "PixelHelper.h"
#include "tools/Fade.h"

namespace animation
{

class ShapeVanishing : public IAnimation
{
    public:
        ShapeVanishing(referenceSystem::ShapeReferenceSystem * referenceSystem)
        : _referenceSystem(referenceSystem), _currentShape(NULL)
        {
        }

        virtual ~ShapeVanishing()
        {
        }

        std::string name()
        {
            return "Shape vanishing";
        }

        void setup()
        {
            _referenceSystem->clear();
            _referenceSystem->driveLeds();
            //we copy all shapes into hidden part
            if (_referenceSystem->shapeList().moveToStart())
            {
                do
                {
                    _hidden.Append(_referenceSystem->shapeList().getCurrent());
                } while (_referenceSystem->shapeList().next());
            }
        }

        void loop()
        {
            if ((not _fade.isConfigured()) || (_fade.isFinished()))
            {
                //if it is just finished we have to put the last current shape in the right list
                if (_currentShape != NULL)
                {
                    if (_orderVanish)
                        _hidden.Append(_currentShape);
                    else
                        _visible.Append(_currentShape);
                    _currentShape = NULL;
                }

                //we have to decide if a shape has to vanish or appear
                if (_hidden.size() == 0)
                {
                    _orderVanish = true;
                }
                else
                {
                    float ratio = ((float)_visible.size())/((float)_hidden.size());
                    Serial.printf("_visible.size() = %d, _hidden.size() = %d, ratio = %f", _visible.size(), _hidden.size(), ratio);
                    if (ratio < MinVisibleRatio)
                    {
                        //we have to make appear
                        Serial.println("not enough items");
                        _orderVanish = false;
                    }
                    else if (ratio > MaxVisibleRatio)
                    {
                        Serial.println("Too much items");
                        _orderVanish = true;
                    }
                    else
                    {
                        //we take a random decision
                        _orderVanish = (bool)random(2);
                        Serial.printf("Random decision %d\n", (int)_orderVanish);
                    }
                }

                if (_orderVanish)
                {
                    //we choose a random element in visible list
                    _visible.at(random(_visible.size()));
                    _currentShape = _visible.getCurrent();
                    //we remove from visible list
                    _visible.DeleteCurrent();
                    Color src = _referenceSystem->getShape(_currentShape)->pixels()[0];
                    Color dst = 0;
                    int time = random(FadingSmallestDuration, FadingLongestDuration);
                    Serial.println("vanish order");
                    Serial.printf("from : #%06x, to : #%06x, in %d ms\n", src, dst, time);
                    _fade.configure(src, dst, time);
                }
                else
                {
                    //we have to make an element appear
                    //we choose a random element in hidden list
                    _hidden.at(random(_hidden.size()));
                    _currentShape = _hidden.getCurrent();
                    //we remove from hidden list
                    _hidden.DeleteCurrent();
                    Color src = 0;
                    Color dst = PixelHelper::getRandomFullColorExcept(src);
                    int time = random(FadingSmallestDuration, FadingLongestDuration);
                    Serial.println("appear order");
                    Serial.printf("from : #%06x, to : #%06x, in %d ms\n", src, dst, time);
                    _fade.configure(src, dst, time);
                }
            }
            Color c = _fade.step();
            _referenceSystem->fill(_currentShape, c);
            _referenceSystem->driveLeds();
        }

    private:
        referenceSystem::ShapeReferenceSystem * _referenceSystem;
        static const int FadingSmallestDuration = 5;
        static const int FadingLongestDuration = 20;
        LinkedList<Shape *> _hidden;
        LinkedList<Shape *> _visible;
        Shape * _currentShape;
        static const int MinVisibleRatio = .2;
        static const int MaxVisibleRatio = 1 - MinVisibleRatio;
        Fade _fade;
        bool _orderVanish;
};

}