#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>

#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"

#define LED_PIN_NUMBER  2

ConfigurationProvider _configuration;
WifiManager _wifiManager;

ShapeHelper _shapeHelper(_configuration);
PixelHelper _pixelHelper(_configuration);
Adafruit_NeoPixel * _ledDriver;
AnimationFactory * _animationFactory;
Animator * _animator;

void setup() 
{
  Serial.begin(115200);
  Serial.println("setup...");
  _configuration.setup();
  _configuration.loadFromFlash();
  _pixelHelper.setup();
  //_wifiManager.erase();// This will erase the stored passwords
	//_wifiManager.setupScan();
  Serial.printf("ledModel = %d\n", _configuration.parameters().ledModel);
  
  _ledDriver = new Adafruit_NeoPixel(_shapeHelper.ledCount(), LED_PIN_NUMBER, _configuration.parameters().ledModel + NEO_KHZ800);
  _ledDriver->clear();
  _ledDriver->begin();
  //TODO : manage RGBW strips
  _animationFactory = new AnimationFactory(_configuration, _shapeHelper, _pixelHelper, _ledDriver);
  _animationFactory->setup();

  _animator = new Animator(_configuration, _animationFactory);
  _animator->setup();
  Serial.println("setup finished.");
}

void loop() {
  //_wifiManager.loop();
	//if (_wifiManager.getState() == Connected) {
		// use the Wifi Stack now connected
  //  Serial.println("connected to wifi");
    _animator->loop();
	//}
  
  
  //TODO : manage time correctly
  delay(20);
}