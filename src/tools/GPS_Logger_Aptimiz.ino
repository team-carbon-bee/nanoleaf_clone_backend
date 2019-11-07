#include <SPIFFS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include "ESP32httpUpdateServer.h"
#include "ESP8266FtpServer.h"
#include "JsonConfiguration.h"
#include "Files.h" 
#include <driver/adc.h>
#include <Regexp.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "WiFiManager.h"



#define LED_PIN 14
const int buttonPin = 39;     // the number of the pushbutton pin
const int powerPin = 21;     
int buttonState = 0;         // variable for reading the pushbutton status
int powerState = 0;         // variable for reading the pushbutton status

char* cert = "-----BEGIN CERTIFICATE-----"\
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ"\
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD"\
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX"\
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y"\
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy"\
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr"\
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr"\
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK"\
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu"\
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy"\
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye"\
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1"\
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3"\
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92"\
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx"\
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0"\
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz"\
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS"\
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp"\
"-----END CERTIFICATE-----";


Files file(SPIFFS);

WebServer server(80);
ESP32HTTPUpdateServer   httpUpdater(true);



FtpServer ftpSrv;
TaskHandle_t Task1;
TaskHandle_t Task2;

char fileName[50];
char Data [100];
char date [6];

// Set these to your desired credentials.
const char *ssid = "Logger";
const char *password = "AptimizLogger";

RTC_DATA_ATTR int Mode;


// called for each match
void match_callback_GGA  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms)      // MatchState in use (to get captures)
  {
  char cap [30];   // must be large enough to hold captures
  memset(cap, 0, sizeof(cap));
  
  Serial.println("Matched GGA");
  ms.GetCapture(cap, 5);
  if (cap[0] != '1') {
    Serial.println("gps unlock");
    return;
  }
  Serial.println("gps lock");
  strcpy(Data, "{\"lat\":\"");
  ms.GetCapture(cap,2);
  if (strchr(cap, 'S') != NULL){
    strcat(Data, "-");
  }
  ms.GetCapture(cap, 1);
  float latitude = atof(cap);
  latitude = int(latitude/100)+(latitude/100-int(latitude/100))/0.6;
  dtostrf(latitude, 6, 14, cap);
  strcat(Data, cap);
  strcat(Data, "\",\"long\":\"");
  ms.GetCapture(cap,4);
  if (strchr(cap, 'W') != NULL){
    strcat(Data, "-");
  }
  ms.GetCapture(cap,3);
  float longitude = atof(cap);
  longitude = int(longitude/100)+(longitude/100-int(longitude/100))/0.6;
  dtostrf(longitude, 6, 14, cap);
  strcat(Data, cap);
  

  Serial.print("Data = ");
  Serial.println(Data);
  
}  // end of match_callback 

// called for each match
void match_callback_RMC  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms)      // MatchState in use (to get captures)
  {
  char cap [20];   // must be large enough to hold captures
  if (strlen(Data) == 0){
    return;
  }
  ms.GetCapture(cap,9);
  strcat(Data, "\",\"createdAt\":\"20");
  strncat(Data, &cap[4], 2);
  strcat(Data, "-");
  strncat(Data, &cap[2], 2);
  strcat(Data, "-");
  strncat(Data, &cap[0], 2);
  strcat(Data, "T");
  ms.GetCapture(cap,0);
  strncat(Data, &cap[0], 2);
  strcat(Data, ":");
  strncat(Data, &cap[2], 2);
  strcat(Data, ":");
  strncat(Data, &cap[4], 2);
  ms.GetCapture(cap,1);
  strcat(Data, cap);
  strcat(Data,"Z\"}");
  ms.GetCapture(cap,9);
  strcat(fileName, "/log_");
  strncat(fileName, &cap[4], 2);
  strncat(fileName, &cap[2], 2);
  strncat(fileName, &cap[0], 2);
  
  ms.GetCapture(cap,0);
  strncat(fileName, &cap[0], 2);
  // strncat(fileName, &cap[2], 2);
  // strncat(fileName, &cap[4], 2);
  strcat(fileName, ".txt");
  // strcat(fileName, ".txt");
  // fileName = String("/log_"+String(cap)+".txt");  
}  // end of match_callback 

char* readGPSLine()
{
  static char tmp[250];
  char *ptr;
  ptr = tmp;
  memset(tmp, 0, sizeof(tmp));
  Serial1.readBytesUntil('\r', tmp, sizeof(tmp));
  while (*ptr != '$') ptr++;
  return ptr;
}


void setup() 
{
  Serial.begin(115200);
  Serial.println(String(F("Aptimiz_logger - Build: ")) + F(__DATE__) + " " +  F(__TIME__));
  pinMode(LED_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(powerPin, INPUT);
  powerState = digitalRead(powerPin);
  buttonState = digitalRead(buttonPin);

  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(50);
  digitalWrite(LED_PIN, LOW);

  int read_raw;
  

  pinMode(35, INPUT);

  float VBAT = (float)(analogRead(35)); // LiPo battery voltage in volts  
  Serial.print("Battery_voltage : ");
  Serial.println(VBAT*3.3/4096*2);
  
  // Configuration.restoreDefault();
  Configuration.setup();
  Serial.println(Configuration._token);
  
  if (Mode == 1) {
    Mode=0;
    WiFiManager wifiManager;

    WiFiManagerParameter custom_sleepTime("sleepTime", "Sleep Time", String(Configuration._sleepTime).c_str(), 3);
    WiFiManagerParameter custom_id("id", "id", Configuration._id.c_str(), 100);
    WiFiManagerParameter custom_apiKey("apiKey", "apiKey", Configuration._apiKey.c_str(), 100);
    WiFiManagerParameter custom_token("token", "token", Configuration._token.c_str(), 100);

    wifiManager.addParameter(&custom_sleepTime);
    wifiManager.addParameter(&custom_id);
    wifiManager.addParameter(&custom_apiKey);
    wifiManager.addParameter(&custom_token);
    String SSID = "Aptimiz"+Configuration._token;
    Serial.println(SSID);

    wifiManager.autoConnect(SSID.c_str(),"Aptimiz26");

    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }
    Serial.println("WL-CONNECTED");
    Configuration._sleepTime = atoi(custom_sleepTime.getValue());
    Configuration._id = custom_id.getValue();
    Configuration._apiKey = custom_apiKey.getValue();
    Configuration._token = custom_token.getValue();
    Configuration.saveConfig();

    String URL = "https://dev.aptimiz.com/api/auth/activityRecord";
    file.sendFiles(URL); // Send all location files to URL


    MDNS.begin("aptimiz-logger"); 
    MDNS.addService("http", "tcp", 80);
    // // modify config with http://172.16.10.41/set?id=yourId&sleeptime=15&apikey=toto25
    server.on("/set", HTTP_GET, [&]() 
    {
      if (server.hasArg("sleeptime")) {
        Serial.print("Set sleepTime to: ");
        Serial.println(server.arg("sleeptime"));
        Serial.print("Old sleep time is : ");
        Serial.println(Configuration._sleepTime);
        Configuration._sleepTime=(server.arg("sleeptime")).toInt();
        Serial.print("New sleep time is : ");
        Serial.println(Configuration._sleepTime);
      }
      if (server.hasArg("id")) {
        Serial.print("Set id to: ");
        Serial.println(server.arg("id"));
        Serial.print("Old id is : ");
        Serial.println(Configuration._id);
        Configuration._id=(server.arg("id"));
        Serial.print("New id is : ");
        Serial.println(Configuration._id);
      }
      if (server.hasArg("apikey")) {
        Serial.print("Set Api key to: ");
        Serial.println(server.arg("apikey"));
        Serial.print("Old Api key is : ");
        Serial.println(Configuration._apiKey);
        Configuration._apiKey=(server.arg("apikey"));
        Serial.print("New Api key is : ");
        Serial.println(Configuration._apiKey);
      }
      if (server.hasArg("token")) {
        Serial.print("Set token to: ");
        Serial.println(server.arg("token"));
        Serial.print("Old token is : ");
        Serial.println(Configuration._token);
        Configuration._apiKey=(server.arg("token"));
        Serial.print("New token is : ");
        Serial.println(Configuration._token);
      }
      Configuration.saveConfig();
      sleep(0.1);
      Configuration.readConfig();
      sendJson(200, Configuration._json);  //Response to the HTTP request
    });

    disableCore0WDT();
    httpUpdater.setup(&server, String("/"));
    server.begin();
    Serial.println("server began");

    // String Reading = file.readFiles();
    // Serial.println(Reading);
    MDNS.addService("ftp", "tcp", 21);
    ftpSrv.begin("FtpAptimiz","FtpAptimiz");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
    Serial.println("ftp began");
    //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(
                      Task1code,   /* Task function. */
                      "Task1",     /* name of task. */
                      10000,       /* Stack size of task */
                      NULL,        /* parameter of the task */
                      1,           /* priority of the task */
                      &Task1,      /* Task handle to keep track of created task */
                      0);          /* pin task to core 0 */
  }
  else {
    if ((powerState == HIGH && buttonState == HIGH && Mode != 2) ||(powerState == LOW && buttonState == LOW)){
      Mode=1;
      ESP.deepSleep(1);
    }
    else{
      Mode=2;
      Serial1.begin(9600, SERIAL_8N1, 12, 15);
      Serial1.setTimeout(1000);
      //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
      xTaskCreatePinnedToCore(
                        Task2code,   /* Task function. */
                        "Task2",     /* name of task. */
                        10000,       /* Stack size of task */
                        NULL,        /* parameter of the task */
                        1,           /* priority of the task */
                        &Task2,      /* Task handle to keep track of created task */
                        1);          /* pin task to core 1 */
    }
  }
}

void sendJson(const uint16_t code, JsonDocument& doc)
{
  WiFiClient client = server.client();

  // Write Header
  client.print(F("HTTP/1.0 "));
  client.print(code);
  client.println(F(" OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Access-Control-Allow-Origin: *"));
  client.print(F("Content-Length: "));
  client.println(measureJsonPretty(doc));
  client.println(F("Connection: close"));
  client.println();

  // Write JSON document
  serializeJsonPretty(doc, client);
}

// the loop function runs over and over again forever
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  digitalWrite(LED_PIN, HIGH);
  for(;;){
    server.handleClient();
    ftpSrv.handleFTP();
    vTaskDelay(10);
   }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(50);
    digitalWrite(LED_PIN, LOW);   
    char *line = readGPSLine();
    Serial.println(line);    
    MatchState ms(line);
    ms.GlobalMatch("\$GPGGA,([0-9\.]*),(\-?[0-9\.]*),([NS]?),(\-?[0-9\.]*),([WE]?),([0-9]?),([0-9]*),(\-?[0-9\.]*),(\-?[0-9\.]*),(M?),(\-?[0-9\.]*),(M?),,[0-9]*\*([0-9a-fA-F]+)", match_callback_GGA);
    ms.GlobalMatch("\$GPRMC,([0-9]*)([0-9\.]*),([AV]),(\-?[0-9\.]*),([NS]?),(\-?[0-9\.]*),([WE]?),(\-?[0-9.]*),(\-?[0-9.]*),([0-9]*),(\-?[0-9.]*),([EW]?),.?\*([0-9a-fA-F]+)", match_callback_RMC);
    if (strlen(Data) > 60){
      Serial.println("fileName = ");
      Serial.println(fileName);
      Serial.print("uid = ");
      Serial.println(Configuration._id);
      if (file.findFiles("/", 0, fileName)==0){
        file.writeFile(fileName, "{\"id\":\"");
        file.appendFile(fileName, Configuration._id.c_str());
        file.appendFile(fileName,"\",\"token\":\"");
        file.appendFile(fileName, Configuration._token.c_str());
        file.appendFile(fileName,"\",\"location\":[\r\n");
        file.appendFile(fileName, Data);
        file.appendFile(fileName,"]}");
      }
      else
      {
        file.appendFile(fileName, ",\r\n", 2);
        file.appendFile(fileName, Data);
        file.appendFile(fileName,"]}");
      }
      memset(Data, 0, sizeof(Data));
      ESP.deepSleep(Configuration._sleepTime*1000000);
    }
    vTaskDelay(10);
  }
}

void loop() {
}
