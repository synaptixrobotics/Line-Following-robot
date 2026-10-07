// ============================================================
// Synaptix Robotics - PID Line Following Robot
// Arduino Nano / Uno compatible
// 5-Channel IR Sensor + L298N + 2 DC Gear Motors
// ============================================================

// -------------------- Motor Pins --------------------
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 10
#define IN3 12
#define IN4 11

// -------------------- Sensor Pins --------------------
#define S1 2
#define S2 3
#define S3 4
#define S4 5
#define S5 6

// -------------------- Speed Limits --------------------
int baseSpeed = 65;
int minSpeed = 0;
int maxSpeed = 255;

// -------------------- PID Parameters --------------------
float Kp = 5.4;
float Ki = 0.001;
float Kd = 0.99;

float integralLimit = 30.0;

float lastError = 0;
float integral = 0;

// Control loop interval in milliseconds
unsigned long lastTime = 0;
unsigned long samplingRate = 50;

void setup() {
  // Motor outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensor inputs
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  // Motors off during the startup delay
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  // Forward motor direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  // ENA = D9 and ENB = D10 use Timer1.
  // Increase PWM frequency from ~490 Hz to ~3.9 kHz.
  // Timer1 is not used by millis(), so the control loop timing remains valid.
  TCCR1B = (TCCR1B & 0b11111000) | 0x02;

  // 5-second safety/start delay
  delay(5000);
  lastTime = millis();
}

// ------------------------------------------------------------
// Calculate the line position error from the five IR sensors.
// Sensors are assumed ACTIVE-LOW: LOW = line detected.
// S5 ... S1 are weighted from left to right.
// ------------------------------------------------------------
float getError() {
  int activeCount = 0;
  float weightedSum = 0;

  if (digitalRead(S5) == LOW) { weightedSum += -7; activeCount++; }
  if (digitalRead(S4) == LOW) { weightedSum += -3; activeCount++; }
  if (digitalRead(S3) == LOW) { weightedSum +=  0; activeCount++; }
  if (digitalRead(S2) == LOW) { weightedSum +=  3; activeCount++; }
  if (digitalRead(S1) == LOW) { weightedSum +=  7; activeCount++; }

  // If no sensor sees the line, keep the previous error.
  return (activeCount > 0) ? (weightedSum / activeCount) : lastError;
}

// ------------------------------------------------------------
// PID controller
// ------------------------------------------------------------
float PID(float error, float dt) {
  integral += error * dt;
  integral = constrain(integral, -integralLimit, integralLimit);

  float derivative = (error - lastError) / dt;

  return (Kp * error) + (Ki * integral) + (Kd * derivative);
}

void loop() {
  unsigned long currentTime = millis();

  // Run the control loop every samplingRate milliseconds.
  if (currentTime - lastTime < samplingRate) {
    return;
  }

  float dt = (currentTime - lastTime) / 1000.0;
  float error = getError();
  float correction = PID(error, dt);

  // Prevent PID correction from exceeding the available speed range.
  int maxSteeringVal = maxSpeed - baseSpeed;
  int steeringVal = constrain(round(correction), -maxSteeringVal, maxSteeringVal);

  // Differential steering.
  int leftSpeed = constrain(baseSpeed + steeringVal, minSpeed, maxSpeed);
  int rightSpeed = constrain(baseSpeed - steeringVal, minSpeed, maxSpeed);

  moveForward(leftSpeed, rightSpeed);

  lastError = error;
  lastTime = currentTime;
}

// ------------------------------------------------------------
// Drive both motors forward at independent PWM speeds.
// ------------------------------------------------------------
void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
