#include <IRremote.hpp>

// ---------------- PINS ----------------
#define IR_RECEIVE_PIN 2

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define ENA 5
#define ENB 6

// ---------------- IR COMMANDS ----------------

// Movement
#define FORWARD_CODE 0x09
#define BACKWARD_CODE 0x07
#define RIGHT_CODE 0x43
#define LEFT_CODE 0x44
#define STOP_CODE 0x45

// Speed buttons
#define SPEED1_CODE 0x0C
#define SPEED2_CODE 0x18
#define SPEED3_CODE 0x5E
#define SPEED4_CODE 0x08
#define SPEED5_CODE 0x1C
#define SPEED6_CODE 0x5A
#define SPEED7_CODE 0x42
#define SPEED8_CODE 0x52
#define SPEED9_CODE 0x4A

// ---------------- SPEED ----------------

int speedLevel = 5;   // Start at 50%

int getSpeed() {
  // 1 = 10%, 9 = 90%
  return map(speedLevel, 1, 9, 26, 230);
}

// ---------------- MOTION STATE ----------------

enum Motion {
  STOPPED,
  FORWARD_MOTION,
  BACKWARD_MOTION,
  LEFT_TURN,
  RIGHT_TURN
};

Motion currentMotion = STOPPED;

// ---------------- MOTOR FUNCTIONS ----------------

// Your confirmed working direction logic
void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ---------------- APPLY CURRENT MOTION ----------------

void applyMotion() {

  int normalSpeed = getSpeed();

  // Differential steering speed
  int turnSpeed = normalSpeed * 0.4;

  if (turnSpeed < 20) {
    turnSpeed = 20;
  }

  switch (currentMotion) {

    case FORWARD_MOTION:
      forward();
      analogWrite(ENA, normalSpeed);
      analogWrite(ENB, normalSpeed);
      break;

    case BACKWARD_MOTION:
      backward();
      analogWrite(ENA, normalSpeed);
      analogWrite(ENB, normalSpeed);
      break;

    case LEFT_TURN:
      if (currentMotion == LEFT_TURN) {
        // This state is used for differential steering
        // when moving, or pivoting when stopped.
      }
      break;

    case RIGHT_TURN:
      if (currentMotion == RIGHT_TURN) {
        // Handled separately below.
      }
      break;

    case STOPPED:
      stopCar();
      break;
  }
}

// ---------------- MOVEMENT COMMANDS ----------------

void moveForward() {
  currentMotion = FORWARD_MOTION;
  forward();

  int s = getSpeed();
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void moveBackward() {
  currentMotion = BACKWARD_MOTION;
  backward();

  int s = getSpeed();
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

// ---------------- LEFT ----------------

void turnLeft() {

  int s = getSpeed();

  if (currentMotion == FORWARD_MOTION) {

    // Moving forward:
    // Left side slower, right side normal

    forward();

    analogWrite(ENA, s);
    analogWrite(ENB, s * 0.4);

  }
  else if (currentMotion == BACKWARD_MOTION) {

    // Moving backward:
    // Left side slower, right side normal

    backward();

    analogWrite(ENA, slowSpeed);
    analogWrite(ENB, s * 0.4);

  }
  else {

    // Stationary = pivot left
    // Strong pivot turn

    left();

    int turnSpeed = 204;  // ~80% PWM

    analogWrite(ENA, turnSpeed);
    analogWrite(ENB, turnSpeed);

    // Remain logically stopped so another LEFT/RIGHT
    // after releasing the button still behaves as a pivot.
    currentMotion = STOPPED;
  }
}

// ---------------- RIGHT ----------------

void turnRight() {

  int s = getSpeed();

  if (currentMotion == FORWARD_MOTION) {

    // Moving forward:
    // Right side slower, left side normal

    forward();

    analogWrite(ENA, s * 0.4);
    analogWrite(ENB, s);

  }
  else if (currentMotion == BACKWARD_MOTION) {

    // Moving backward:
    // Right side slower, left side normal

    backward();

    analogWrite(ENA, s * 0.4);
    analogWrite(ENB, s);

  }
  else {

    // Stationary = pivot right
    // Strong pivot turn

    right();

    int turnSpeed = 204;  // ~80% PWM

    analogWrite(ENA, turnSpeed);
    analogWrite(ENB, turnSpeed);

    currentMotion = STOPPED;
  }
}

// ---------------- STOP ----------------

void doStop() {
  currentMotion = STOPPED;
  stopCar();
}

// ---------------- SPEED ----------------

void setSpeed(int newLevel) {

  speedLevel = newLevel;

  int pwm = getSpeed();

  Serial.print("SPEED: ");
  Serial.print(speedLevel);
  Serial.print(" (");
  Serial.print(speedLevel * 10);
  Serial.print("%) PWM=");
  Serial.println(pwm);

  // IMPORTANT:
  // Immediately apply the new speed if the car is moving.

  if (currentMotion == FORWARD_MOTION ||
      currentMotion == BACKWARD_MOTION) {

    applyMotion();
  }
}

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopCar();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR CAR READY");
  Serial.println("Speed starts at 50%");
}

// ---------------- LOOP ----------------

void loop() {

  if (IrReceiver.decode()) {

    uint8_t command = IrReceiver.decodedIRData.command;

    // Ignore invalid/zero commands
    if (command != 0x00) {

      Serial.print("Command: 0x");
      Serial.println(command, HEX);

      // ---------- MOVEMENT ----------

      if (command == FORWARD_CODE) {

        Serial.println("FORWARD");
        moveForward();

      }
      else if (command == BACKWARD_CODE) {

        Serial.println("BACKWARD");
        moveBackward();

      }
      else if (command == LEFT_CODE) {

        Serial.println("LEFT");
        turnLeft();

      }
      else if (command == RIGHT_CODE) {

        Serial.println("RIGHT");
        turnRight();

      }
      else if (command == STOP_CODE) {

        Serial.println("STOP");
        doStop();

      }

      // ---------- SPEED ----------

      else if (command == SPEED1_CODE) {
        setSpeed(1);
      }
      else if (command == SPEED2_CODE) {
        setSpeed(2);
      }
      else if (command == SPEED3_CODE) {
        setSpeed(3);
      }
      else if (command == SPEED4_CODE) {
        setSpeed(4);
      }
      else if (command == SPEED5_CODE) {
        setSpeed(5);
      }
      else if (command == SPEED6_CODE) {
        setSpeed(6);
      }
      else if (command == SPEED7_CODE) {
        setSpeed(7);
      }
      else if (command == SPEED8_CODE) {
        setSpeed(8);
      }
      else if (command == SPEED9_CODE) {
        setSpeed(9);
      }
      else {
        Serial.println("UNKNOWN");
      }
    }

    IrReceiver.resume();
  }
}
