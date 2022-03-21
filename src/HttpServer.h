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

#include <WebSocketsServer.h>

class HttpServer
{
public:
	HttpServer() ;
	virtual ~HttpServer();

	void setup(void);
	void handle(void);

  void sendDataToWebSocket();

#ifdef ESP32
  WebServer&          webServer() { return _webServer; }
#else
  ESP8266WebServer&   webServer()  { return _webServer; }
#endif

private:
  void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
  void setConfig();
  void getAnimationList();
  void getInformations();
  void powerOn();
  void powerOff();
  void previewAnimation();
	String getContentType(String filename);
  bool handleFileRead(String path);
  void sendJson(const uint16_t code, JsonDocument& doc);

#ifdef ESP32
  WebServer                 _webServer;
  ESP32HTTPUpdateServer     _httpUpdater;
#else
  ESP8266WebServer          _webServer;
  ESP8266HTTPUpdateServer   _httpUpdater;
#endif
  WebSocketsServer          _webSocketServer;
};

#if !defined(NO_GLOBAL_INSTANCES)
extern HttpServer HTTPServer;
#endif

