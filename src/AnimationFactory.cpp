#include "animation/RunningLight.h"
#include "animation/FullFading.h"
#include "animation/FullColoredFading.h"
#include "animation/IndependantRandomFading.h"

#include "AnimationFactory.h"

AnimationFactory::AnimationFactory(ConfigurationProvider & configuration, ShapeHelper & shapeHelper, 
                                   PixelHelper & pixelHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper), _ledDriver(ledDriver), 
      _linearReferenceSystem(_configuration, _shapeHelper, _pixelHelper, _ledDriver)
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
    
    _animations.Append(new animation::FullFading(_linearReferenceSystem));
    _animations.Append(new animation::FullColoredFading(_linearReferenceSystem));
    _animations.Append(new animation::IndependantRandomFading(_linearReferenceSystem));
    _animations.Append(new animation::RunningLight(_linearReferenceSystem));
    //New animations come here !!!!

    //setup all animations
    if (_animations.moveToStart())
    {
        do 
        {
            _animations.getCurrent()->setup();
        } while(_animations.next());
    }
}

LinkedList<animation::IAnimation*> & AnimationFactory::animations()
{
    return _animations;
}