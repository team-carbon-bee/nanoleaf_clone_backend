#pragma once

#include <inttypes.h>

#ifdef ESP32
  #include "tools/WiFiManager.h"
#else // ESP8266
  #include <WiFiManager.h>
#endif

class NetworkManager
{
public:
	NetworkManager() ;
	virtual ~NetworkManager();

	void setup();
	void handle();

private:
  WiFiManager _wifiManager;
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NetworkManager Network;
#endif

