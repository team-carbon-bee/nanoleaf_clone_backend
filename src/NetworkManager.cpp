#include <ArduinoOTA.h>

#include "HttpServer.h"
#include "tools/Logger.h"
#include "ConfigurationProvider.h"

#include "NetworkManager.h"
/********************************************************/
/******************** Public Method *********************/
/********************************************************/

// NTP and Timezone
#define NTP_SERVERS "0.fr.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
// List of timezone: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

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

    Log.print("Update NTP...");

    struct tm timeinfo;
    configTzTime(TIMEZONE, NTP_SERVERS);
    getLocalTime(&timeinfo);

    Log.println(" Done !");
    Log.print("Date Time: ");
    Log.println(asctime(&timeinfo));

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
