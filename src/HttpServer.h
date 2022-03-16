#pragma once

#include <inttypes.h>
#include <ArduinoJson.h>

#ifdef ESP32
  #include <WebServer.h>
  #include "tools/ESP32HTTPUpdateServer.h"
#else // ESP8266
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPUpdateServer.h>
#endif

class HttpServer
{
public:
	HttpServer() ;
	virtual ~HttpServer();

	void setup(void);
	void handle(void);

	String getContentType(String filename);
  bool handleFileRead(String path);

#ifdef ESP32
  WebServer&          webServer();
#else
  ESP8266WebServer&   webServer();
#endif

  void sendJson(const uint16_t code, JsonDocument& doc);

private:
  void sendCors();
  void setConfig();
  void getAnimationList();
  void getInformations();
#ifdef ESP32
  WebServer                 _webServer;
  ESP32HTTPUpdateServer     _httpUpdater;
#else
  ESP8266WebServer          _webServer;
  ESP8266HTTPUpdateServer   _httpUpdater;
#endif
};

#if !defined(NO_GLOBAL_INSTANCES)
extern HttpServer HTTPServer;
#endif

