// Wind Power Station - INA219 power logger
// Board: Arduino Mega 2560 (SDA = pin 20, SCL = pin 21)
// Prints averaged voltage (V), current (mA) and power (mW) every ~0.5 s.

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
const int SAMPLES = 20;   // readings averaged per printed line

void setup() {
  Serial.begin(115200);
  if (!ina219.begin()) {
    Serial.println("INA219 not found. Check wiring.");
    while (1) delay(10);
  }
  ina219.setCalibration_16V_400mA();   // finer resolution for small currents
  Serial.println("Voltage_V, Current_mA, Power_mW");
}

void loop() {
  float vSum = 0, iSum = 0, pSum = 0;
  for (int n = 0; n < SAMPLES; n++) {
    float v = ina219.getBusVoltage_V() + ina219.getShuntVoltage_mV() / 1000.0;
    float i = ina219.getCurrent_mA();
    vSum += v;
    iSum += i;
    pSum += v * i;          // power calculated directly: volts x milliamps
    delay(25);
  }
  Serial.print(vSum / SAMPLES, 3); Serial.print(", ");
  Serial.print(iSum / SAMPLES, 2); Serial.print(", ");
  Serial.println(pSum / SAMPLES, 2);
}
