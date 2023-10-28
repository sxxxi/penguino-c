#include <SoftwareSerial.h>
#include <SD.h>             
#include <TMRpcm.h>           
#include <SPI.h>
#include "headers/action_registry.h"

#define ArduinoTxPin 2
#define ArudinoRxPin 3
#define SD_ChipSelectPin 4
#define rxPin 5
#define txPin 6
#define baud 9600

SoftwareSerial mySerial(rxPin, txPin); // RX, TX
SoftwareSerial ArduinoSerial (ArudinoRxPin, ArduinoTxPin);
action_registry *registry;
TMRpcm tmrpcm;

//===<ENTRY>=======================================
void setup() {
  setupSerial();
  setupBluetooth();
  setupHandlers();
}

void loop() { 
  serve();  
}


//===<BT SETUP>====================================
void setupBluetooth() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(baud);
  Serial.println("BT and serial setup");
}



//===<REQUEST HANDLERS>============================
int feed(char *foodName) {
  sendToArduino2("feed", foodName);
  playFile("nomnom.wav");
  Serial.println("Feeding...");
}

int displayText(char *text) {
  String s = String(text);
  if (s.length() > 0) {
    int space = 9 - (s.length() / 14);
    sendToArduino2("text", text);
    Serial.print(space);
    playFile("talk.wav");
  }
}

void setupHandlers() {
  registry = new_action_registry(2);
  add_entry(registry, new_registry_entry("feed", feed));
  add_entry(registry, new_registry_entry("displayText", displayText));
  Serial.println("Handlers setup");
}

//===<REQUEST PROCESSOR>===========================
void serve() {
  if (ArduinoSerial.available()) {
    Serial.println(ArduinoSerial.readString());
  }

  if (mySerial.available() > 0) {

    String mapping = mySerial.readStringUntil('>');
    String args = mySerial.readStringUntil(';');


    action_lambda action = get(registry, mapping.c_str());

    if (action != NULL) {
      action(args.c_str());

    } else {
      Serial.println("Mapping not found.");
    }

  }
}

//===<Audio stuff>==============================================
void playFile(char *fileName) {
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail"); 
    return;   // don't do anything more if not
  }
  tmrpcm.speakerPin = 9;
  tmrpcm.play(fileName);
}

// ==============================


void sendToArduino2(String mapping, String str) {
  mySerial.end();

  pinMode(ArudinoRxPin, INPUT);
  pinMode(ArduinoTxPin, OUTPUT);
  ArduinoSerial.begin(baud);
  Serial.println("Arduino com setup");

  String fin = mapping + ">" + str + ";";

  ArduinoSerial.write(fin.c_str());

  ArduinoSerial.end();
  setupBluetooth();
}


void setupSerial() {
  Serial.begin(baud);
}




