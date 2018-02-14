#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-02-13 14:57:09

#include "Arduino.h"
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"
#include "states.h"
#include "constants.h"
#include "ESP8266Ping.h"

bool isConnectedToInternet() ;
void updateInternetConnectionStatus() ;
void Wifi_Send_Command(volatile unsigned char * p, int size) ;
void disconnect() ;
void scan() ;
bool SetDeviceModel() ;
bool connect() ;
bool scanForUpdate() ;
bool scanForUserID() ;
bool uploadSummary(void) ;
bool uploadSummaryOnCancel(void) ;
bool getModel() ;
bool uploadContactInfo(void) ;
bool startWorkout(void) ;
bool uploadWorkout(void) ;
bool sendBeacon(void) ;
void SetLoginDefault(void) ;
byte merge(char MSB, char LSB) ;
void FindChecksum() ;
bool KernelCode() ;
bool AppCode() ;
bool FlashKernel() ;
bool FlashAppCode() ;
void ClearFRAM(void) ;
void ReadFRAM(void) ;
void TestFRAM(void) ;
void setup() ;
void loop() ;


#include "MyocycleWifi.ino"

#endif
