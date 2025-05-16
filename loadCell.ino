#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

const int HX711_dout = 5; //mcu > HX711 dout pin
const int HX711_sck = 6; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
const float calibrationValue = 696.0;

void setupLoadCell() {
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step

  LoadCell.begin();
  LoadCell.start(stabilizingtime, _tare);

  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}


const int forceRefreshInterval = 10;
float readForce() {
  static boolean newDataReady = 0;
  static uint32_t lastGotData = millis();

  uint32_t currentTime = millis();

  if (currentTime - forceRefreshInterval < lastGotData) {
    return -1;
  }

  // Serial.println("Waiting for load cell");

  // check for new data/start next conversion:
  if (!LoadCell.update()) {
    return -1;
  }

  lastGotData = millis();

  float data = LoadCell.getData();
  // float data = 5.0f;
  Serial.print("Load value: ");
  Serial.println(data);

  return data;
}

