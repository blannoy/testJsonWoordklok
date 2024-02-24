#define DEBUG true
// #define DEBUGMEM true

#include <Arduino.h>
#include <utils.h>
#include "filesystem.h"
#include "clockface.h"
#include "configuration.h"
#include "ledcode.h"





void setup() {
  Serial.begin(115200);
  initMemLog();
  filesystemSetup();
  configurationSetup();
  Serial.println("Got config");
  reportmem("after config");
  printConfig(*config);
}

void loop() {
  //ledShowClockface();
  Serial.println("Check");
  reportmem("done");
  delay(1000);
  // put your main code here, to run repeatedly:
}
