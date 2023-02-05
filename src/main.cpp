#include <Arduino.h>
#include "FS.h" 
#include "SPIFFS.h"
#define STORAGE_FILE "/secrets"

#define MAXLENGTH 20
#define DATAFIELDS 5

// sequence is as follows: ssid, ssid password, mqttserver ip, mqtt server user, mqtt user password
// please not the length limitation to 20 characters. If you need more, increase MAXLENGTH, DATAFIELDS respectively, set force refresh to true and recompile
const char dataBuffer[DATAFIELDS][MAXLENGTH] = {"SSID","WLAN PASSWORD","MQTT SERVER IP","MQTT USERNAME","MQTT PASSWORD"};

bool forceRefresh = false;  // set this flag to true to force rewriting of the secrets file

void setup() { 

  Serial.begin(115200);

  char fsData[DATAFIELDS][MAXLENGTH];
  bool writeFile = false;

  if (!SPIFFS.begin()) { // check file system exists
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if secrets file exists and size is correct
  if (SPIFFS.exists(STORAGE_FILE)) {
      File f = SPIFFS.open(STORAGE_FILE, "r");
      if (f) {
        if (f.readBytes((char *)fsData, (MAXLENGTH * DATAFIELDS)) != (MAXLENGTH * DATAFIELDS)) {
          Serial.println("read error ! removing old file."); 
          SPIFFS.remove(STORAGE_FILE);
          writeFile = true;
        }
        else {
          Serial.println("reading stored information ...");
          
          Serial.print("SSID: ");
          Serial.println(fsData[0]);
          
          Serial.print("PASSWORD: ");
          Serial.println(fsData[1]);
          
          Serial.print("MQTT SERVER: ");
          Serial.println(fsData[2]);
          
          Serial.print("MQTT USERNAME: ");
          Serial.println(fsData[3]);
          
          Serial.print("MQTT PASSWORD: ");
          Serial.println(fsData[4]);

          // add code for additional fields here
        }
        f.close();
      }
  } else writeFile = true;

  if(writeFile || forceRefresh) {
    if(forceRefresh) {
      Serial.println("forcing refresh ...");
      if(SPIFFS.exists(STORAGE_FILE)) {
        Serial.print("removing old file."); 
        SPIFFS.remove(STORAGE_FILE);
        Serial.println("...deleted.");
      }
    }
    // store data
    File f = SPIFFS.open(STORAGE_FILE, "w");
    if (f) {
      f.write((const unsigned char *)dataBuffer, (MAXLENGTH * DATAFIELDS));
      f.close();
      Serial.println("writing of data done.");
    } else {
      Serial.println("Error creating file.");
    }
  }
}
  
void loop() {
  // put your main code here, to run repeatedly:
}