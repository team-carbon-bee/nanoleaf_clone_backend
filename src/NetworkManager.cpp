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
  _ftpServer.begin(Configuration.parameters().hostname.c_str(), Configuration.parameters().hostname.c_str()); 
}

void NetworkManager::handle()
{
  HTTPServer.handle();
  _ftpServer.handleFTP();
}

#if !defined(NO_GLOBAL_INSTANCES) 
NetworkManager Network;
#endif
