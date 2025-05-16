#include "slave.h"

const int CALCULATION_TIME_MS = 2000;
const int DELAY_MS = 10;

int rotations = 0;

unsigned long startTime;

void setup() {
  Serial.begin(115200); delay(100);
  Serial.println();
  Serial.println("Starting...");

  steupSlave();

  setupLoadCell();
  setupLaser();

  startTime = millis();
}

void loop() {
  float force = readForce();
  if (force != -1.0f) {
    // forceValues[forceIndex++] = force;
    // Serial.println("Got force: ")
    sendData(force, false);
  }

  if (hasCompletedRotation()) {
    rotations++;
    Serial.print("Roation: ");
    Serial.println(rotations);

    sendData(-1, true);
  }

  // // If more than calculated time ms has passed then do the
  // // calculation
  // if (millis() - startTime >= CALCULATION_TIME_MS) {
  //   // calculate some shit

  //   forceIndex = 0;
  //   startTime = millis();
  // }

  // delay(100);

}



