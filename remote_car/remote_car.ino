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
#define RIGHT_CODE 0x44
#define LEFT_CODE 0x43
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

int speedLevel = 5;  // Start at 50%

int getSpeed() {
  return map(speedLevel, 1, 9, 26, 230);
}

// ---------------- TIMING ----------------

// How long we allow the car to continue after
// the last valid IR command/repeat.
const unsigned long SIGNAL_TIMEOUT = 350;

unsigned long lastMovementSignal = 0;

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

// These are your confirmed working directions.

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
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- APPLY SPEED ----------------

void applyCurrentSpeed() {

  int s = getSpeed();

  switch (currentMotion) {

    case FORWARD_MOTION:
      forward();
      analogWrite(ENA, s);
      analogWrite(ENB, s);
      break;

    case BACKWARD_MOTION:
      backward();
      analogWrite(ENA, s);
      analogWrite(ENB, s);
      break;

    default:
      break;
  }
}

// ---------------- MOVEMENT ----------------

void moveForward() {

  currentMotion = FORWARD_MOTION;
  lastMovementSignal = millis();

  forward();

  int s = getSpeed();

  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void moveBackward() {

  currentMotion = BACKWARD_MOTION;
  lastMovementSignal = millis();

  backward();

  int s = getSpeed();

  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

// ---------------- LEFT ----------------

void turnLeft() {

  lastMovementSignal = millis();

  int s = getSpeed();

  // If the car is moving, make a differential turn.
  if (currentMotion == FORWARD_MOTION) {

    forward();

    // Left slower, right normal.
    analogWrite(ENA, s * 0.4);
    analogWrite(ENB, s);

    return;
  }

  if (currentMotion == BACKWARD_MOTION) {

    backward();

    // Left slower, right normal.
    analogWrite(ENA, s * 0.4);
    analogWrite(ENB, s);

    return;
  }

  // Stationary = strong pivot.
  currentMotion = LEFT_TURN;

  left();

  analogWrite(ENA, 204);  // 80%
  analogWrite(ENB, 204);  // 80%
}

// ---------------- RIGHT ----------------

void turnRight() {

  lastMovementSignal = millis();

  int s = getSpeed();

  if (currentMotion == FORWARD_MOTION) {

    forward();

    // Right slower, left normal.
    analogWrite(ENA, s);
    analogWrite(ENB, s * 0.4);

    return;
  }

  if (currentMotion == BACKWARD_MOTION) {

    backward();

    // Right slower, left normal.
    analogWrite(ENA, s);
    analogWrite(ENB, s * 0.4);

    return;
  }

  // Stationary = strong pivot.
  currentMotion = RIGHT_TURN;

  right();

  analogWrite(ENA, 204);  // 80%
  analogWrite(ENB, 204);  // 80%
}

// ---------------- STOP ----------------

void doStop() {

  currentMotion = STOPPED;
  stopCar();

  Serial.println("STOP");
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

  // If currently driving straight, immediately change speed.
  if (currentMotion == FORWARD_MOTION ||
      currentMotion == BACKWARD_MOTION) {

    applyCurrentSpeed();
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

  // ---------------- IR ----------------

  if (IrReceiver.decode()) {

    uint8_t command = IrReceiver.decodedIRData.command;
    uint8_t flags = IrReceiver.decodedIRData.flags;

    bool isRepeat =
      (flags & IRDATA_FLAGS_IS_REPEAT);

    // Ignore invalid command 0x00 unless it is a repeat.
    if (command != 0x00 || isRepeat) {

      // Movement command or repeat
      if (command == FORWARD_CODE) {

        if (!isRepeat) {
          Serial.println("FORWARD");
        }

        moveForward();
      }

      else if (command == BACKWARD_CODE) {

        if (!isRepeat) {
          Serial.println("BACKWARD");
        }

        moveBackward();
      }

      else if (command == LEFT_CODE) {

        if (!isRepeat) {
          Serial.println("LEFT");
        }

        turnLeft();
      }

      else if (command == RIGHT_CODE) {

        if (!isRepeat) {
          Serial.println("RIGHT");
        }

        turnRight();
      }

      else if (command == STOP_CODE) {

        doStop();
      }

      // Speed buttons are NOT movement commands.
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

      else if (!isRepeat) {
        Serial.print("UNKNOWN COMMAND: 0x");
        Serial.println(command, HEX);
      }
    }

    IrReceiver.resume();
  }


  // ---------------- FAILSAFE ----------------

  // If we are in any movement state and haven't
  // heard from the remote recently, stop.

  if (currentMotion != STOPPED) {

    if (millis() - lastMovementSignal > SIGNAL_TIMEOUT) {

      Serial.println("IR SIGNAL LOST - STOP");

      currentMotion = STOPPED;
      stopCar();
    }
  }
}
