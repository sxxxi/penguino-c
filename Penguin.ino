#include <SoftwareSerial.h>
#include <SD.h>             
#include <TMRpcm.h>           
#include <SPI.h>
#include "headers/action_registry.h"

#define SD_ChipSelectPin 4
#define rxPin 5
#define txPin 6
#define baud 9600

SoftwareSerial mySerial(rxPin, txPin); // RX, TX
action_registry *registry;
TMRpcm tmrpcm;

//===<ENTRY>=======================================
void setup() {
  setupBluetooth();
  // setupSpeaker();
  setupHandlers();
}

void loop() { 
  serve();  
}



//===<BT SETUP>====================================
void setupBluetooth() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(baud);
  mySerial.begin(baud);
  Serial.println("AT commands: OK");
}



//===<REQUEST HANDLERS>============================
int feed(char *foodName) {
  Serial.print("Eating: ");
  Serial.println(foodName);
}

int displayText(char * text) {
  Serial.print("Display: ");
  Serial.println(text);
}

void setupHandlers() {
  registry = new_action_registry(2);
  add_entry(registry, new_registry_entry("feed", feed));
  add_entry(registry, new_registry_entry("displayText", displayText));
}

//===<REQUEST PROCESSOR>===========================
void serve() {
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
void setupSpeaker() {
  tmrpcm.speakerPin = 9;
  
  pinMode(13, OUTPUT); //LED Connected to analog pin 0
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail"); 
    return;   // don't do anything more if not
  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.play("test.wav");
}



