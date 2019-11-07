#include <Arduino.h>
#include "tools/WiFiManager.h"
//#include "tools/WifiManager.h"
#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ledDriver/NeoPixelBusLedDriver.h"
#include "HttpServer.h"
#include "tools/Logger.h"

#define NTP_SERVERS "0.fr.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
#define UTC_OFFSET  +1

ConfigurationProvider * _configuration;
WiFiManager _wifiManager;
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

  _wifiManager.autoConnect("nanoLeaf", "nanoLeaf");
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(200);
      Log.print(".");
  }
  Log.println("WiFi-CONNECTED");

  
#ifdef USE_DST_ADJUST
  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);
  while (!time(nullptr)) {
    delay(50);
  }
#endif

  HTTPServer.setup();

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


void loop() 
{
  Log.println("Main loop");
  _animator->loop();

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
  
  HTTPServer.handle();

  //TODO : manage time correctly
  delay(50);
}