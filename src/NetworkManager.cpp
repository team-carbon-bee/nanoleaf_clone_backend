#include <ArduinoOTA.h>

#include "HttpServer.h"
#include "tools/Logger.h"
#include "ConfigurationProvider.h"

#include "NetworkManager.h"
/********************************************************/
/******************** Public Method *********************/
/********************************************************/

#define NTP_SERVERS "0.fr.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
#define UTC_OFFSET  +1

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

void NetworkManager::setup()
{
  _wifiManager.autoConnect(Configuration.parameters().hostname.c_str(), Configuration.parameters().hostname.c_str());
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

      /* Initialize OTA Server */
    Log.println("Arduino OTA activated");

    // Port defaults to 3232
    ArduinoOTA.setPort(3232);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname(Configuration.parameters().hostname.c_str());

    ArduinoOTA.onStart([&]() {
        Log.println("Arduino OTA: Start updating");
    });

    ArduinoOTA.onEnd([]() {
        Log.println("Arduino OTA: End");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Log.print("Arduino OTA Progress: ");
        Log.print(String(progress / (total / 100)));
        Log.println(" %");
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Log.print("Arduino OTA Error : ");
        Log.print(String(error));
        if (error == OTA_AUTH_ERROR)
            Log.println("Arduino OTA: Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Log.println("Arduino OTA: Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Log.println("Arduino OTA: Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Log.println("Arduino OTA: Receive Failed");
        else if (error == OTA_END_ERROR)
            Log.println("Arduino OTA: End Failed");
    });

    ArduinoOTA.begin(); 
}

void NetworkManager::handle()
{
  HTTPServer.handle();
}

#if !defined(NO_GLOBAL_INSTANCES) 
NetworkManager Network;
#endif
