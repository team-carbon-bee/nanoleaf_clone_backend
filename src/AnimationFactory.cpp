#include "animation/RunningLight.h"
#include "animation/FullFading.h"
#include "animation/FullColoredFading.h"
#include "animation/IndependantRandomFading.h"
#include "animation/FullColoredShapeFading.h"
#include "animation/RandomColoredShapes.h"
#include "animation/Fireworks.h"

#include "AnimationFactory.h"

AnimationFactory::AnimationFactory(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, 
                                   PixelHelper * pixelHelper, Adafruit_NeoPixel * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _pixelHelper(pixelHelper), _ledDriver(ledDriver)
{
}

AnimationFactory::~AnimationFactory()
{
}

void AnimationFactory::setup()
{
    Serial.println("AnimationFactory::setup()");
    
    _linearReferenceSystem = new referenceSystem::LinearReferenceSystem(_configuration, _shapeHelper, _pixelHelper, _ledDriver);
    _shapeReferenceSystem = new referenceSystem::ShapeReferenceSystem(_configuration, _shapeHelper, _pixelHelper, _ledDriver);
    //we configure all reference systems
    _linearReferenceSystem->setup();
    _shapeReferenceSystem->setup();
    //we create all animations
    _animations.Append(new animation::FullColoredFading(_linearReferenceSystem));
    _animations.Append(new animation::IndependantRandomFading(_linearReferenceSystem));
    _animations.Append(new animation::FullFading(_linearReferenceSystem));
       
    
    
    
    _animations.Append(new animation::RandomColoredShapes(_shapeReferenceSystem)); 
    _animations.Append(new animation::FullColoredShapeFading(_shapeReferenceSystem)); 
    _animations.Append(new animation::RunningLight(_linearReferenceSystem));
    _animations.Append(new animation::Fireworks(_linearReferenceSystem));

    //New animations come here !!!!
/*
    Serial.println("Setup animations");
    //setup all animations
    if (_animations.moveToStart())
    {
        do 
        {
            _animations.getCurrent()->setup();
        } while(_animations.next());
    }*/
}

LinkedList<animation::IAnimation*> & AnimationFactory::animations()
{
    return _animations;
}

void AnimationFactory::clearAnimationObject()
{
    //in shape ref system
    _shapeReferenceSystem->clearAnimationObject();
}