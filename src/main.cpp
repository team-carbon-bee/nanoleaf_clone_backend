#include <Arduino.h>
#include "tools/WiFiManager.h"
//#include "tools/WifiManager.h"
#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ledDriver/NeoPixelBusLedDriver.h"
#include "NetworkManager.h"
#include "tools/Logger.h"

void setup() 
{
  Log.setup();
  Log.println("setup...");

  Configuration.setup();
  Configuration.loadFromFlash();

  Network.setup();

  GlobalAnimator.setup();

  Log.println("setup finished.");
}

int l = 0;

void loop() 
{
  if (l >= Configuration.parameters().speed)
  {
    GlobalAnimator.handle();
    l = 0;
  }
  Network.handle();

  ++l;
  delay(1);
}