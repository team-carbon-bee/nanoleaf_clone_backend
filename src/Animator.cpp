#include "Animator.h"

Animator::Animator(ConfigurationProvider * configuration, AnimationFactory * animationFactory)
    : _configuration(configuration), _animationFactory(animationFactory), _currentAnimation(NULL)
{
}

Animator::~Animator()
{
}

void Animator::setup()
{
}

void Animator::loop()
{
    if (_currentAnimation == NULL)
    {
        //no animation in progress we chose one
        //TODO select animation
        _currentAnimation = _animationFactory->animations().Last();
        //Before init animation, we have to remove all previously malloc objects
        _animationFactory->clearAnimationObject();
        _currentAnimation->setup();
    }
    //Serial.println("looping animation");
    _currentAnimation->loop();
}