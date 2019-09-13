#include "animation/RunningLight.h"

#include "AnimationFactory.h"

AnimationFactory::AnimationFactory(ConfigurationProvider & configuration, ShapeHelper & shapeHelper, 
                                   Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _ledDriver(ledDriver), 
      _linearReferenceSystem(_configuration, _shapeHelper, _ledDriver)
{
}

AnimationFactory::~AnimationFactory()
{
}

void AnimationFactory::setup()
{
    //we configure all reference systems
    _linearReferenceSystem.setup();

    //we create all animations
    _animations.Append(new animation::RunningLight(_linearReferenceSystem));
    //New animations come here !!!!

    //setup all animations
    if (_animations.moveToStart())
    {
        _animations.getCurrent()->setup();
    }
}

LinkedList<animation::IAnimation*> & AnimationFactory::animations()
{
    return _animations;
}