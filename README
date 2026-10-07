# Line Following Robot — PID Control

A compact **5-sensor PID line-following robot** built around an **Arduino Nano/Uno**, **L298N motor driver**, and two DC gear motors. The robot reads the line position from five IR sensors and continuously adjusts the left/right motor speeds using PID control for smoother tracking.

> **Synaptix Robotics**  
> *INNOVATE | AUTOMATE | INSPIRE*

## Features

- 5-channel IR line sensor array
- PID-based steering correction
- Differential speed control for left/right motors
- L298N dual H-bridge motor driver
- Adjustable base speed and PID gains
- 50 ms control-loop sampling interval
- Integral windup protection
- Higher PWM frequency (~3.9 kHz) for smoother low-speed motor operation
- Arduino Nano / Uno compatible pin mapping

## Hardware

| Component | Quantity | Notes |
|---|---:|---|
| Arduino Nano or Uno | 1 | ATmega328P recommended |
| L298N motor driver | 1 | Dual DC motor driver |
| 5-channel IR sensor array | 1 | Digital outputs, active LOW |
| N20/DC gear motors | 2 | Match both motors mechanically |
| Wheels | 2 | Suitable for motor shaft |
| 2S 18650 battery pack | 1 | 7.4 V nominal; use protected cells/BMS |
| Robot chassis | 1 | 2-wheel drive |
| Jumper wires | — | Male/female as required |

## Circuit Diagram

![Circuit Diagram](docs/circuit_diagram.png)

> **Power note:** The motor supply and logic supply must share a common GND. Do not connect the motor battery directly to a 5 V sensor or logic pin. Use a regulated 5 V supply for the Arduino/sensors, or a suitable regulated output from your motor-driver/power system.

## Pin Connections

### Motor Driver — L298N

| L298N Pin | Arduino Pin | Function |
|---|---|---|
| ENA | D9 | Left motor PWM |
| IN1 | D8 | Left motor direction |
| IN2 | D7 | Left motor direction |
| ENB | D10 | Right motor PWM |
| IN3 | D12 | Right motor direction |
| IN4 | D11 | Right motor direction |

### IR Sensor Array

The code assumes **LOW = line detected**.

| Sensor | Arduino Pin | Weight |
|---|---|---:|
| S1 | D2 | +7 |
| S2 | D3 | +3 |
| S3 | D4 | 0 |
| S4 | D5 | -3 |
| S5 | D6 | -7 |

The physical left/right orientation depends on how the sensor board is mounted. If the robot steers in the opposite direction, swap the sensor orientation or invert the error weights.

## Power Connections

Recommended arrangement:

```text
2S 18650 Battery Pack (7.4 V nominal)
            │
            ├──> L298N motor supply (12V/Vs terminal)
            │
            └──> Regulated 5 V supply ──> Arduino + IR sensors

Battery GND ─────────────┬──> L298N GND
                         ├──> Arduino GND
                         └──> Sensor GND
```

**Important:** A 2S 18650 pack is about 8.4 V when fully charged. Verify your exact L298N module's regulator configuration and the allowable input range of your Arduino/power regulator before connecting it.

## How the PID Control Works

The five sensors are assigned weighted positions:

```text
S5       S4       S3       S2       S1
-7       -3        0       +3       +7
 |        |        |        |        |
 +--------+--------+--------+--------+
                  LINE
```

For every control cycle, the program averages the weights of all sensors that detect the line:

```text
error = weighted sensor position / number of active sensors
```

Then PID calculates a steering correction:

```text
correction = Kp × error + Ki × integral + Kd × derivative
```

The correction is applied differentially:

```text
leftSpeed  = baseSpeed + correction
rightSpeed = baseSpeed - correction
```

This causes the robot to speed up one motor and slow down the other when the line moves away from the center.

## Default Parameters

```cpp
int baseSpeed = 65;
int minSpeed = 0;
int maxSpeed = 255;

float Kp = 5.4;
float Ki = 0.001;
float Kd = 0.99;

float integralLimit = 30.0;
unsigned long samplingRate = 50; // ms
```

### PID Tuning

| Symptom | Try |
|---|---|
| Robot reacts too slowly | Increase `Kp` |
| Robot oscillates left/right | Reduce `Kp` or increase `Kd` |
| Robot overshoots corners | Increase `Kd` gradually |
| Robot has steady bias | Increase `Ki` slightly |
| Motors are too slow | Increase `baseSpeed` carefully |
| Robot loses the line at high speed | Reduce `baseSpeed` or retune PID |

Start with small changes and test one parameter at a time.

## Getting Started

1. Install the **Arduino IDE**.
2. Open `src/Line_Following_Robot.ino`.
3. Select **Arduino Nano** or **Arduino Uno** as appropriate.
4. For classic Nano boards, select the correct processor/bootloader option.
5. Build the circuit using the pin tables above.
6. Confirm that all grounds are common.
7. Upload the sketch.
8. Place the robot on a suitable high-contrast track.
9. Tune `baseSpeed`, `Kp`, `Ki`, and `Kd` for your chassis and track.

The robot waits **5 seconds after power-up** before starting the motors.

## Repository Structure

```text
Line-Following-Robot/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   └── Line_Following_Robot.ino
├── docs/
│   ├── circuit_diagram.png
│   └── wiring.md
└── images/
```

## Safety & Build Notes

- Keep the robot lifted off the ground during the first motor test.
- Verify motor polarity before running the PID loop.
- Never power motors directly from an Arduino GPIO pin.
- Use an appropriate battery, BMS/protection, fuse where appropriate, and regulated logic supply.
- L298N modules have significant voltage drop; choose the motor/battery combination accordingly.
- Disconnect power before changing wiring.

## License

This project is released under the MIT License. See [LICENSE](LICENSE).

## Author

**Synaptix Robotics**  
Robotics • AI • IoT • R&D • Customized Engineering • SaaS & Web Development

Website: https://www.synaptixrobotics.com/
