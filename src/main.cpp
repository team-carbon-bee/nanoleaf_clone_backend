#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ledDriver/NeoPixelBusLedDriver.h"

ConfigurationProvider * _configuration;
WifiManager _wifiManager;
ShapeHelper * _shapeHelper;
ledDriver::ILedDriver * _ledDriver;
AnimationFactory * _animationFactory;
Animator * _animator;


void setup() 
{
  Serial.begin(115200);
  Serial.println("setup...");
  _configuration = new ConfigurationProvider();
  _configuration->setup();
  _configuration->loadFromFlash();
  _shapeHelper = new ShapeHelper(_configuration);
  _shapeHelper->setup();

  _ledDriver = new ledDriver::NeoPixelBusLedDriver(_configuration, _shapeHelper);
  _ledDriver->setup();
  
  Serial.println("Creating animations.");

  _animationFactory = new AnimationFactory(_configuration, _shapeHelper, _ledDriver);
  _animationFactory->setup();

  Serial.println("Creating animator.");

  _animator = new Animator(_configuration, _animationFactory);
  _animator->setup();
  Serial.println("setup finished.");

  //  strip.Begin();
  //  strip.Show();
}
int i = 0;
bool inc = true;
void loop() {
  //_wifiManager.loop();
	//if (_wifiManager.getState() == Connected) {
		// use the Wifi Stack now connected
  //  Serial.println("connected to wifi");
   _animator->loop();
	//}
  
  //_ledDriver->clear();
  /*
  for (int n = 0; n < 21 * 3; ++n)
      //_ledDriver->setPixelColor(n, Adafruit_NeoPixel::Color(i, 0, i));
      strip.SetPixelColor(n, RgbColor(0, i, 0));
  if (inc)
      i += 20;
  else
      i -= 20;

  if (i > 255)
  {
      i = 255;
      inc = false;
  }
  else if (i < 0)
  {
      i = 0;
      inc = true;
  }
  
  strip.Show();*/
  
  

  //TODO : manage time correctly
  delay(50);
}