# Wiring Guide

## 1. L298N to Arduino

| L298N | Arduino | Purpose |
|---|---|---|
| ENA | D9 | Left motor PWM |
| IN1 | D8 | Left motor direction |
| IN2 | D7 | Left motor direction |
| ENB | D10 | Right motor PWM |
| IN3 | D12 | Right motor direction |
| IN4 | D11 | Right motor direction |
| GND | GND | Common ground |

Connect the left motor to OUT1/OUT2 and the right motor to OUT3/OUT4. If either motor runs in the opposite physical direction, reverse that motor's two output wires.

## 2. Five IR Sensors

| Sensor output | Arduino |
|---|---|
| S1 | D2 |
| S2 | D3 |
| S3 | D4 |
| S4 | D5 |
| S5 | D6 |

For each sensor:

```text
VCC  -> regulated 5 V
GND  -> common GND
OUT  -> assigned Arduino digital pin
```

The sketch expects the sensor output to be **LOW when the line is detected**.

## 3. Battery and Power

Use a suitable 2S battery pack for the motors and a regulated 5 V logic rail. Tie the grounds together.

```text
             +--------------------> L298N motor supply
             |
2S Battery --+
             |
             +--------------------> 5 V regulator ---> Arduino 5V
                                             |
                                             +--------> IR VCC

Battery/Regulator GND ----------------------> Common GND
```

Do not assume every L298N module's onboard 5 V regulator can safely power your complete logic load. Check the specific module and its jumper/regulator configuration.
