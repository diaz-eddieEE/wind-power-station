# Wind Power Station

A small wind turbine test rig to measure how much power a DC-motor generator produces, and to see how blade design changes that output.

<img width="1512" height="2016" alt="wind-fan-light" src="https://github.com/user-attachments/assets/6fa1cde2-f9b2-470a-8f04-69c8c7186001" />

   *Test rig: DC motor generator with the stock propeller in front of a desk fan, lighting a red LED.* 
   
## Goal
Measure voltage, current, and power from a small wind turbine at different wind speeds, then design blades that beat the stock propeller.

## Hardware
- Small DC motor used as a generator
- Arduino Uno and ESP32
- INA219 voltage/current sensor (arriving this week)
- KAIWEETS HT118A multimeter, handheld anemometer
- Box fan as the wind source

## Test log

### Oct 18, 2026: First generator test (stock propeller)
| Fan setting | No-load voltage | Voltage across red LED | LED |
|---|---|---|---|
| High | 2.49 V | 1.89 V | Bright |
| Low | 2.10 V | 1.78 V | Dim |

**What I learned**
- The motor's voltage drops when it powers a load (2.49 V to 1.89 V). Drawing current makes the motor harder to spin, and the small propeller can't hold its speed.
- The LED's voltage barely changes between fan settings, but its brightness does. The difference is mostly current, which a multimeter in voltage mode can't show. Next step: measure current and power with the INA219.
- As a generator, the motor's polarity depends on spin direction, not wire color.

### Oct 18, 2026: ESP32 WiFi test
- Set up the ESP32 in the Arduino IDE and scanned nearby WiFi networks.
- Wrote a small web server on the ESP32. An LED on pin D2 turns on and off from buttons on my phone's browser.
- This is the first step toward showing live windmill readings on my phone.

## Next steps
- [ ] Log voltage, current and power with the INA219
- [ ] Measure wind speed at each fan setting
- [ ] Design and test larger cardboard blades against the stock propeller
- [ ] Send live readings to a phone over WiFi with the ESP32
