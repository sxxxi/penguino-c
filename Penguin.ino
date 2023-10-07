#include <SoftwareSerial.h>
#include "headers/action_registry.h"
#include "headers/request_filter.h"

#define rxPin 8
#define txPin 9
#define led 6

SoftwareSerial mySerial(rxPin, txPin); // RX, TX
action_registry *registry;



//===<ENTRY>=======================================
void setup() {
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
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("AT commands: OK");
}



//===<REQUEST HANDLERS>============================
int feed(char *foodName) {
  Serial.print("Eating: ");
  Serial.println(foodName);
}

void setupHandlers() {
  registry = new_action_registry(2);
  add_entry(registry, new_registry_entry("feed", feed));
}



//===<REQUEST PROCESSOR>===========================
void serve() {
  if (mySerial.available() > 0) {
    String msg = mySerial.readString();
    
    request_info *info = parseRequestInfo(msg.c_str());
    action_lambda action = get(registry, info->mapping);

    if (action != NULL) {
      action(info->arg);
    } else {
      Serial.println("Mapping not found.");
    }
  }
}



//=================================================
void analogLedControlSetup() {
  pinMode(led, OUTPUT);
}

void analogLedControl(int brightness) {
  if (brightness > 255 || brightness < 0) return;
  analogWrite(led, brightness);
}

