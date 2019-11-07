#include "animation/RunningLight.h"
#include "animation/FullFading.h"
#include "animation/FullColoredFading.h"
#include "animation/FullColoredShapeFading.h"
#include "animation/RandomColoredShapes.h"
#include "animation/Fireworks.h"
#include "animation/PaintingLight.h"
#include "animation/ShapeVanishing.h"
#include "animation/RainbowRunning.h"

#include "AnimationFactory.h"

AnimationFactory::AnimationFactory(ConfigurationProvider * configuration, ShapeHelper * shapeHelper, 
                                   ledDriver::ILedDriver * ledDriver)
    : _configuration(configuration), _shapeHelper(shapeHelper), _ledDriver(ledDriver)
{
}

AnimationFactory::~AnimationFactory()
{
}

void AnimationFactory::setup()
{    
    _linearReferenceSystem = new referenceSystem::LinearReferenceSystem(_configuration, _ledDriver);
    _shapeReferenceSystem = new referenceSystem::ShapeReferenceSystem(_configuration, _shapeHelper, _ledDriver);
    //we configure all reference systems
    _linearReferenceSystem->setup();
    _shapeReferenceSystem->setup();
    
    //we create all animations
    _animations.Append(new animation::FullFading(_linearReferenceSystem));
    _animations.Append(new animation::FullColoredFading(_linearReferenceSystem));
    _animations.Append(new animation::RunningLight(_linearReferenceSystem));

    _animations.Append(new animation::RandomColoredShapes(_shapeReferenceSystem)); 


    _animations.Append(new animation::FullColoredShapeFading(_shapeReferenceSystem));
    //New animations come here !!!!
    _animations.Append(new animation::ShapeVanishing(_shapeReferenceSystem)); 
    _animations.Append(new animation::Fireworks(_linearReferenceSystem));
    _animations.Append(new animation::PaintingLight(_linearReferenceSystem)); 
    _animations.Append(new animation::RainbowRunning(_linearReferenceSystem));
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