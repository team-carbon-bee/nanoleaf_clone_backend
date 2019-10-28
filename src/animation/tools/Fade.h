#pragma once

#include <Arduino.h>

#include "PixelHelper.h"

namespace animation
{

class Fade
{
    public:
        Fade() : _configured(false), _finished(false), _target(0), _currentStep(0)
        {
        }

        Fade(const Fade & copy)
        : _configured(copy._configured),
        _finished(copy._finished),
        _target(copy._target),
        _currentStep(copy._currentStep),
        _currentRed(copy._currentRed),
        _currentGreen(copy._currentGreen),
        _currentBlue(copy._currentBlue),
        _currentWhite(copy._currentWhite),
        _redQuantum(copy._redQuantum),
        _greenQuantum(copy._greenQuantum),
        _blueQuantum(copy._blueQuantum),
        _whiteQuantum(copy._whiteQuantum)
        {
        }
        
        virtual ~Fade()
        {
        }

        void configure(const Color src, const Color dst, const int duration)
        {
            _currentRed = PixelHelper::getRed(src);
            _currentGreen = PixelHelper::getGreen(src);
            _currentBlue = PixelHelper::getBlue(src);
            _currentWhite = PixelHelper::getWhite(src);

            _target = dst;
            _currentStep = duration;

            //we compute quantum on each step for each color
            _redQuantum = ((float)(PixelHelper::getRed(dst) - _currentRed)) / duration;
            _greenQuantum = ((float)(PixelHelper::getGreen(dst) - _currentGreen)) / duration;
            _blueQuantum = ((float)(PixelHelper::getBlue(dst) - _currentBlue)) / duration;
            _whiteQuantum = ((float)(PixelHelper::getWhite(dst) - _currentWhite)) / duration;

            printf("%f %f %f %f\n", _redQuantum, _greenQuantum, _blueQuantum, _whiteQuantum);

            _finished = false;
            _configured = true;
        }

        Color step()
        {
            if (not _finished)
            {
                _currentRed += _redQuantum;
                _currentGreen += _greenQuantum;
                _currentBlue += _blueQuantum;
                _currentWhite += _whiteQuantum;
                
                _currentStep--;
                if (_currentStep == 0)
                    _finished = true;
            }

            return value();
        }

        Color value() const
        {
            return PixelHelper::colorFromRgbw((uint8_t)_currentRed, (uint8_t)_currentGreen, (uint8_t)_currentBlue, (uint8_t)_currentWhite);
        }

        bool isConfigured() const 
        {
            return _configured;
        }

        bool isFinished() const
        {
            return _finished;
        }

        Color target() const
        {
            return _target;
        }

    private:
        bool _configured;
        bool _finished;
 
        Color _target;
        int _currentStep;

        float _currentRed;
        float _currentGreen;
        float _currentBlue;
        float _currentWhite;

        float _redQuantum;
        float _greenQuantum;
        float _blueQuantum;
        float _whiteQuantum;

};

}