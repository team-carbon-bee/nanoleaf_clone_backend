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

AnimationFactory::AnimationFactory()
{
}

AnimationFactory::~AnimationFactory()
{
}

void AnimationFactory::setup(ledDriver::ILedDriver * ledDriver)
{    
    _ledDriver = ledDriver;
    //we configure all reference systems
    referenceSystem::LinearRef.setup(_ledDriver);
    referenceSystem::ShapeRef.setup(_ledDriver);
    
    //we create all animations

    //Using linear reference system
    _animations.Append(new animation::FullFading());
    _animations.Append(new animation::FullColoredFading());
    _animations.Append(new animation::RunningLight());
    _animations.Append(new animation::Fireworks());
    _animations.Append(new animation::PaintingLight()); 
    _animations.Append(new animation::RainbowRunning());

    //Using shape reference system
    _animations.Append(new animation::RandomColoredShapes()); 
    _animations.Append(new animation::FullColoredShapeFading());
    _animations.Append(new animation::ShapeVanishing()); 

    _animations.moveToStart();
    do 
    {
        _animations.getCurrent()->setup();
    } while (_animations.next()); 
}

LinkedList<animation::IAnimation*> & AnimationFactory::animations()
{
    return _animations;
}

void AnimationFactory::clearAnimationObject()
{
    //in shape ref system
    referenceSystem::ShapeRef.clearAnimationObject();
}

#if !defined(NO_GLOBAL_INSTANCES) 
AnimationFactory GlobalAnimationFactory;
#endif