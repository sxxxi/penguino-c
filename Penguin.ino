#include <SoftwareSerial.h>
#include <SD.h>             
#include <TMRpcm.h>           
#include <SPI.h>
#include "headers/action_registry.h"
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

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
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100);
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
  String s = String(text);
  int lines = s.length() / 9;
  int blankRows = lines - 9;
  int startRow = blankRows / 2;
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(30, startRow * 24, text, &Font24, WHITE, BLACK);}

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