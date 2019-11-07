#include <Arduino.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
//#include "tools/WifiManager.h"
#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ledDriver/NeoPixelBusLedDriver.h"
#include "HttpServer.h"
#include "tools/Logger.h"

ConfigurationProvider * _configuration;
WifiManager _wifiManager;
ShapeHelper * _shapeHelper;
ledDriver::ILedDriver * _ledDriver;
AnimationFactory * _animationFactory;
Animator * _animator;


void setup() 
{
  Log.setup();
  Log.println("setup...");

  _configuration = new ConfigurationProvider();
  _configuration->setup();
  _configuration->loadFromFlash();

  //_wifiManager = new WifiManager(_configuration);
//   _wifiManager.autoConnect("nanoLeaf", "nanoLeaf");
//   while (WiFi.status() != WL_CONNECTED) 
//   {
//       delay(200);
//       Log.print(".");
//   }

  //_wifiManager.preferences.putString("apssid", "nanoLeaf_esp32");
  //_wifiManager.preferences.putString("nanoLeaf_esp32", "nanoLeaf_esp32");
  //_wifiManager.setup();
  _wifiManager.erase();
  _wifiManager.setupScan();
  Log.println("WL-CONNECTED");

//   HTTPServer.setup();

  _shapeHelper = new ShapeHelper(_configuration);
  _shapeHelper->setup();

  _ledDriver = new ledDriver::NeoPixelBusLedDriver(_configuration, _shapeHelper);
  _ledDriver->setup();
  
  Log.println("Creating animations.");

  _animationFactory = new AnimationFactory(_configuration, _shapeHelper, _ledDriver);
  _animationFactory->setup();

  Log.println("Creating animator.");

  _animator = new Animator(_configuration, _animationFactory);
  _animator->setup();
  Log.println("setup finished.");
}


void loop() {

_wifiManager.loop();
	//if (_wifiManager.getState() == Connected) {
		// use the Wifi Stack now connected
  //  Log.println("connected to wifi");
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
  
  //HTTPServer.handle();

  //TODO : manage time correctly
  delay(50);
}