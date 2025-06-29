#include <Servo.h>

// LED pins
const int led2 = 2;  // red
const int led3 = 3;  // green
const int led4 = 4;  // yellow

// Button pins
const int buttonYellowPin = 6;
const int buttonGreenPin = 11;
const int buttonRedPin = 12;

// Servo pin
const int servoPin = 10;

Servo myServo;

const int leds[] = { led2, led3, led4 };
const int numLeds = sizeof(leds) / sizeof(leds[0]);

bool lastButtonYellowState = HIGH;
bool lastButtonGreenState = HIGH;
bool lastButtonRedState = HIGH;

void turnOnSelectedLED(int selectedLED) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], leds[i] == selectedLED ? HIGH : LOW);
  }
}

void setup() {

  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(buttonYellowPin, INPUT_PULLUP);
  pinMode(buttonGreenPin, INPUT_PULLUP);
  pinMode(buttonRedPin, INPUT_PULLUP);

  myServo.attach(servoPin);
  myServo.write(0);

  Serial.begin(9600);
}

void loop() {

  bool currentButtonYellow = digitalRead(buttonYellowPin);
  bool currentButtonGreen = digitalRead(buttonGreenPin);
  bool currentButtonRed = digitalRead(buttonRedPin);

  if (lastButtonYellowState == HIGH && currentButtonYellow == LOW) {
    Serial.println("Yellow Button Pressed");
    turnOnSelectedLED(led4);
  }

  if (lastButtonGreenState == HIGH && currentButtonGreen == LOW) {
    Serial.println("Green Button Pressed");
    turnOnSelectedLED(led3);
    myServo.write(90);
  }

  if (lastButtonRedState == HIGH && currentButtonRed == LOW) {
    Serial.println("Red Button Pressed");
    turnOnSelectedLED(led2);
    myServo.write(0);
  }

  lastButtonYellowState = currentButtonYellow;
  lastButtonGreenState = currentButtonGreen;
  lastButtonRedState = currentButtonRed;
}
