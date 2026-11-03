[README (2).md](https://github.com/user-attachments/files/34342575/README.2.md)
# Wind Power Station

A small desk-fan wind turbine I built to measure how much power a DC motor generator makes, and how the electrical load, turbine placement, and blade design change that power.

**Main results**
- Built a generator model from my data (**2.44 V open-circuit, 142 Ω internal resistance**) that predicted the best load and matched four new loads within **0.2–6%**
- The best load was **132 Ω**, close to the generator's internal resistance, which is what maximum power transfer predicts
- Moving the turbine in front of the fan's blade ring instead of its center hub raised power by about **40%** (11.2 → 15.5 mW)
- Bigger homemade blades made **42–63% less power** because they spun slower, and P = V²/R accounts for the whole drop

---

## Hardware

| Part | What I used |
|---|---|
| Generator | EUDAX small DC motor with its stock 4-blade propeller (~6 cm) |
| Sensor | HiLetgo INA219 current/voltage sensor (I soldered the header pins) |
| Microcontroller | Arduino Mega 2560 |
| Wind source | Clip-on desk fan, low and high settings |
| Wind speed | S8606 handheld anemometer |
| Loads | Red LED and resistors (10 Ω to 1 kΩ) |

### Wiring

| INA219 | Connects to |
|---|---|
| Vcc | Mega 5V |
| Gnd | Ground rail |
| SCL | Mega pin 21 |
| SDA | Mega pin 20 |
| Vin+ | Motor wire 1 |
| Vin− | Load |

The other end of the load, motor wire 2, and Mega GND all go to a shared ground rail.

### Test setup
- Propeller 4 in from the fan, fan facing straight on, ceiling fan off
- Wind speed at the propeller: **2.71 m/s** (low) and **3.26 m/s** (high)
- The Arduino averages 20 readings per row and calculates power as V × I

---

## Results

### 1. Load test: finding the best load
Fan on high, stock propeller, resistors from 10 Ω to 1 kΩ (about 23 readings each).

| Load | Voltage | Current | Power |
|---|---|---|---|
| 10 Ω | 0.161 V | 15.45 mA | 2.49 mW |
| 100 Ω | 1.007 V | 10.07 mA | 10.2 mW |
| **132 Ω** (220 ∥ 330) | 1.242 V | 9.41 mA | **11.7 mW** |
| 220 Ω | 1.482 V | 6.73 mA | 10.0 mW |
| 330 Ω | 1.709 V | 5.16 mA | 8.84 mW |
| 1 kΩ | 2.166 V | 2.14 mA | 4.64 mW |

![Power vs load](power_vs_load.png)

- **Ohm's law check:** V ÷ I matched every resistor's value, including 132 Ω for the 220 Ω and 330 Ω resistors in parallel.
- **Repeatability:** I measured 100 Ω again later and got 10.1 mW vs. 10.2 mW the first time, a 1% difference.
- **Generator model:** From the 100 Ω and 220 Ω runs I modeled the motor as a voltage source with internal resistance: **2.44 V and 142 Ω**. The 2.44 V agrees with the 2.49 V I measured separately with a multimeter with no load connected.

| Load | Model predicted | Measured | Difference |
|---|---|---|---|
| 10 Ω | 2.58 mW | 2.49 mW | 4% |
| 132 Ω | 10.5 mW | 11.2 mW (avg of 2 runs) | 6% |
| 330 Ω | 8.82 mW | 8.84 mW | 0.2% |
| 1 kΩ | 4.57 mW | 4.64 mW | 1.5% |

### 2. Blade test: does a bigger rotor help?
Fan on high, 132 Ω load. I tested the stock propeller at the start and again at the end to make sure conditions didn't change (15.6 and 15.4 mW, 1.2% apart).

| Propeller | Rotor size | Weight | Voltage | Power | vs. stock |
|---|---|---|---|---|---|
| Stock | ~6 cm | 4.0 g | 1.43 V | **15.5 mW** | — |
| Design A: 4 flat cardboard blades | ~17 cm | 9.7 g | 1.09 V | 9.0 mW | −42% |
| Design B: longer curved, twisted blades | ~25 cm | 13.1 g | 0.87 V | 5.8 mW | −63% |

![Blade comparison](blade_comparison.png)

At a fixed load, voltage tracks how fast the motor spins, so Design A spun about 76% and Design B about 61% as fast as stock. Because P = V²/R, that predicts 58% and 37% of stock power, which matches what I measured. The bigger rotors were heavier and turned slower, and this small motor needs speed to make voltage. Real wind turbines solve this with a **gearbox** that speeds up big, slow blades for the generator.

Limitation: I didn't test a flat version of Design B, so I can't separate the effect of the curve from the effect of the extra length.

---

## What I learned
1. **Loading effect:** the motor's voltage drops when it powers something (2.49 V with nothing connected → 1.89 V with an LED).
2. **Maximum power transfer:** too little resistance gives current but no voltage, too much gives voltage but no current, and the most power comes near the generator's own resistance.
3. **Setup matters as much as hardware:** controlling the distance and fan angle raised power 2.7×, and moving the turbine away from the fan's center hub added another ~40%.
4. **Measurement resolution:** the INA219's default range was too coarse for milliwatts, so I switched to a finer calibration and averaged 20 samples per reading.
5. **Repeat surprising results:** a peak reading that looked 12% high turned out to be within normal run-to-run spread once I repeated it.
6. **Match the rotor to the generator:** a bigger rotor doesn't help if it makes the motor spin slower.

## Next steps
- Use gears to pair large blades with the motor
- Measure wind speed at the new turbine position
- Send live readings to my phone over WiFi with an ESP32

---

## Arduino code
```cpp
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
const int SAMPLES = 20;

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
    pSum += v * i;          // power = volts x milliamps
    delay(25);
  }
  Serial.print(vSum / SAMPLES, 3); Serial.print(", ");
  Serial.print(iSum / SAMPLES, 2); Serial.print(", ");
  Serial.println(pSum / SAMPLES, 2);
}
```
Requires the Adafruit INA219 library. Board: Arduino Mega 2560, Serial Monitor at 115200 baud.
