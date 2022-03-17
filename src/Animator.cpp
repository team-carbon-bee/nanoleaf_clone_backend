#include "tools/Logger.h"

#include "ConfigurationProvider.h"

#include "ledDriver/NeoPixelBusLedDriver.h"
#include "AnimationFactory.h"

#include "Animator.h"

Animator::Animator()
    : _enabled(true), _currentAnimation(nullptr), _timeRemaining(0), _animationCanChange(false), _currentAnimationIndex(0)
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

    //Getting params from configuration
    _animationSelectionMethod = Configuration.parameters().animationMethod;
    _animationList = Configuration.parameters().animationList;
}

void Animator::handle()
{
    //if animation is enabled
    if (_enabled)
    {
        if (_animationSelectionMethod == ConfigurationProvider::kStatic)
        {
            if (not _animationList.empty())
            {
                //if there is no animation for the moment we select the first of the list
                if (_currentAnimation == nullptr)
                {
                    _currentAnimation = getAnimationById(_animationList.front());
                    _currentAnimation->initialize();
                }

                if ((_currentAnimation->isFinished()) || (_timeRemaining <= 0))
                {
                    //we are in statuc animation but a finshable animation has been selected
                    //we have to restart it
                    _timeRemaining = Configuration.parameters().animationDuration;
                    //we deinit the last run
                    _currentAnimation->deinitialize();
                    //and init the new run
                    _currentAnimation->initialize();
                }
                
            }
            else
            {
                _animationCanChange = false;
                _currentAnimation = nullptr;
            }
        } 
        else
        {
            //random and sequential follow same schema
            if (_currentAnimation == nullptr)
            {
                _currentAnimation = getAnimationByMethod(Configuration.parameters().animationMethod);
                _currentAnimation->initialize();
            }

            if (_currentAnimation->isFinished() || (_timeRemaining <= 0))
            {
                //release the previous one
                _currentAnimation->deinitialize();
                //Before init animation, we have to remove all previously malloc objects
                GlobalAnimationFactory.clearAnimationObject();
                
                //Two possibilities, the last one was a "non system animation" and the next one has to be fade off
                // The last one was already fade off and we can choose anoter new animation
                if (_currentAnimation == &_fadingOffAnimation)
                {
                    _currentAnimation = getAnimationByMethod(_animationSelectionMethod);
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
        }   

        if (_currentAnimation != nullptr)
        {
            _currentAnimation->loop();
            _timeRemaining -= Configuration.parameters().speed;
        }
    }

    if ((_currentAnimation == &_fadingOffAnimation) && (not _currentAnimation->isFinished()))
    {
        _currentAnimation->loop();
    }
}

void Animator::enabled(const bool value)
{
    if (value)
    {
        _enabled = true;
    }
    else
    {
        _enabled = false;
        //we se animation to fadeout
        _currentAnimation = &_fadingOffAnimation;
        _currentAnimation->initialize();
    }
}

bool Animator::enabled() const
{
    return _enabled;
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
        //we roll a dice to choose an id in animationList
        int pos = random(0, Configuration.parameters().animationList.size());
        GlobalAnimationFactory.animations().at(Configuration.parameters().animationList[pos]);
        return GlobalAnimationFactory.animations().getCurrent();
    }
    else if (animationMethod == ConfigurationProvider::kSequential)
    {
        if (GlobalAnimationFactory.animations().at(Configuration.parameters().animationList[_currentAnimationIndex]))
        {
            _currentAnimationIndex++;
            if (_currentAnimationIndex >= Configuration.parameters().animationList.size())
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