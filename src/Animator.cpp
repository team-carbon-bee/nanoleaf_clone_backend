#include "tools/Logger.h"

#include "ConfigurationProvider.h"

#include "ledDriver/NeoPixelBusLedDriver.h"
#include "AnimationFactory.h"

#include "Animator.h"

Animator::Animator()
    : _currentAnimation(NULL), _timeRemaining(0), _animationCanChange(false), _currentAnimationIndex(0)
{
}

Animator::~Animator()
{
}

void Animator::setup()
{
    _ledDriver = new ledDriver::NeoPixelBusLedDriver();
    _ledDriver->setup();
    _ledDriver->setBrightness(Configuration.parameters().maxBrightness);
    _ledDriver->clear();
    
    Log.println("Creating animations.");

    GlobalAnimationFactory.setup(_ledDriver);

    Log.println("Creating animator.");

    if (Configuration.parameters().animationMethod == ConfigurationProvider::kStatic)
    {
        if (not Configuration.parameters().animationList.empty())
        {
            _animationCanChange = false;
            _currentAnimation = getAnimationById(Configuration.parameters().animationList.front());
        }
        else
        {
            _animationCanChange = false;
            _currentAnimation = nullptr;
        }
    } 
    else
    {
        _animationCanChange = true;
        _currentAnimation = getAnimationByMethod(Configuration.parameters().animationMethod);
    }   
}

void Animator::handle()
{
    if (_currentAnimation != NULL)
    {
        if ((_animationCanChange) && ((_currentAnimation->isFinished()) || (_timeRemaining <= 0)))
        {
            //release the previous one
            _currentAnimation->deinitialize();
            //Before init animation, we have to remove all previously malloc objects
            GlobalAnimationFactory.clearAnimationObject();
            
            //Two possibilities, the last one was a "non system animation" and the next one has to be fade off
            // The last one was already fade off and we can choose anoter new animation
            if (_currentAnimation == &_fadingOffAnimation)
            {
                _currentAnimation = getAnimationByMethod(Configuration.parameters().animationMethod);
            }
            else
            {
                _currentAnimation = &_fadingOffAnimation;
            }
           
            Serial.printf("Starting new animation %s (%d)\n", _currentAnimation->name().c_str(), _currentAnimation->id());
            _currentAnimation->initialize();
            //restart timer
            _timeRemaining = Configuration.parameters().animationDuration;
        }

        //Serial.println("looping animation");
        _currentAnimation->loop();

        _timeRemaining -= Configuration.parameters().speed;
    }
}

animation::IAnimation * Animator::getAnimationById(const uint8_t & animationId)
{
    if (GlobalAnimationFactory.animations().moveToStart())
    {
        do
        {
            if (GlobalAnimationFactory.animations().getCurrent()->id() == animationId)
            {
               return GlobalAnimationFactory.animations().getCurrent();
            }
        } while (GlobalAnimationFactory.animations().next());
    }
    Serial.printf("Unable to find animation with id %d\n", animationId);
    return NULL;
}

animation::IAnimation * Animator::getAnimationByMethod(const ConfigurationProvider::EAnimationSelectionMethod & animationMethod)
{
    if (animationMethod == ConfigurationProvider::kRandom)
    {
        int pos = random(0, GlobalAnimationFactory.animations().size());
        GlobalAnimationFactory.animations().at(pos);
        return GlobalAnimationFactory.animations().getCurrent();
    }
    else if (animationMethod == ConfigurationProvider::kSequential)
    {
        if (GlobalAnimationFactory.animations().at(_currentAnimationIndex))
        {
            _currentAnimationIndex++;
            if (_currentAnimationIndex >= GlobalAnimationFactory.animations().size())
                _currentAnimationIndex = 0;
            return GlobalAnimationFactory.animations().getCurrent();
        }
    }
    else
    {
        Serial.printf("Unable to find animation method\n");
    }

    return NULL;
}

#if !defined(NO_GLOBAL_INSTANCES) 
Animator GlobalAnimator;
#endif