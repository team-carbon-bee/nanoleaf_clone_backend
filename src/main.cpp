#include <Arduino.h>

#include "ConfigurationProvider.h"

ConfigurationProvider _configuration;

void setup() 
{
  _configuration.loadFromFlash();
}

void loop() {
  
}