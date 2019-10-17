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
            return Adafruit_NeoPixel::Color((uint8_t)_currentRed, (uint8_t)_currentGreen, (uint8_t)_currentBlue, (uint8_t)_currentWhite);
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
        float _currentRed;
        float _currentGreen;
        float _currentBlue;
        float _currentWhite;

        Color _target;
        int _currentStep;

        float _redQuantum;
        float _greenQuantum;
        float _blueQuantum;
        float _whiteQuantum;

};

}