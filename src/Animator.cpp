#include "tools/Logger.h"

#include "ledDriver/NeoPixelBusLedDriver.h"
#include "AnimationFactory.h"

#include "Animator.h"

Animator::Animator()
    : _currentAnimation(NULL)
{
}

Animator::~Animator()
{
}

void Animator::setup()
{
    _ledDriver = new ledDriver::NeoPixelBusLedDriver();
    _ledDriver->setup();
    
    Log.println("Creating animations.");

    GlobalAnimationFactory.setup(_ledDriver);

    Log.println("Creating animator.");
}

void Animator::handle()
{
    if (_currentAnimation == NULL)
    {
        //no animation in progress we chose one
        //TODO select animation
        _currentAnimation = GlobalAnimationFactory.animations().Last();
        //Before init animation, we have to remove all previously malloc objects
        GlobalAnimationFactory.clearAnimationObject();
        _currentAnimation->setup();
    }
    //Serial.println("looping animation");
    _currentAnimation->loop();
}

#if !defined(NO_GLOBAL_INSTANCES) 
Animator GlobalAnimator;
#endif