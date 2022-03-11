#pragma once

#include <inttypes.h>

#ifdef ESP32
  #include "tools/WiFiManager.h"
  #include <ESP8266FtpServer.h>
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
  FtpServer _ftpServer;
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NetworkManager Network;
#endif

