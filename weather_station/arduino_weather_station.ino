#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Button pins
const int blueButtonPin = 2;
const int blackButtonPin = 4;

// Sensor pin
const int temperaturePin = A0;
int reading;
float volts;
float millivolts;
float degreesC;

// analog reference
const float aref_voltage = 1.1;

// Button states
bool prevBlueButtonState = HIGH;
bool currentBlueButtonState;
bool prevBlackButtonState = HIGH;
bool currentBlackButtonState;

// Debounce
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Update every 5 seconds
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;

// Current mode
String currentMode = "NO_MODE";

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WEATHER STATION");

  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(blackButtonPin, INPUT_PULLUP);

  analogReference(INTERNAL);
}

void loop() {

  unsigned long now = millis();

  currentBlueButtonState = digitalRead(blueButtonPin);
  currentBlackButtonState = digitalRead(blackButtonPin);

  if (prevBlueButtonState == HIGH && currentBlueButtonState == LOW) {
    if (now - lastDebounceTime > debounceDelay) {
      displayMessage("HUMIDITY   ");
      currentMode = "HUMIDITY";
      lastDebounceTime = now;
    }
  }

  if (prevBlackButtonState == HIGH && currentBlackButtonState == LOW) {
    if (now - lastDebounceTime > debounceDelay) {
      displayMessage("TEMPERATURE");
      currentMode = "TEMPERATURE";
      lastDebounceTime = now;
    }
  }

  prevBlueButtonState = currentBlueButtonState;
  prevBlackButtonState = currentBlackButtonState;

  if (now - lastUpdate >= updateInterval) {
    reading = analogRead(A0);
    volts = reading * aref_voltage / 1023.0;
    millivolts = 1000 * volts;
    degreesC = millivolts / 10.0;

    if (currentMode == "TEMPERATURE") {
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(degreesC, 1);
      lcd.print((char)223);
      lcd.print("C     ");
    }
    if (currentMode == "HUMIDITY") {
      lcd.setCursor(0, 1);
      lcd.print("HR: ");
      lcd.print(1, 1);
      lcd.print("%           ");
    }

    lastUpdate = now;
  }
}

void displayMessage(const char* message) {
  lcd.clear();
  delay(5);
  lcd.setCursor(0, 0);
  lcd.print(message);
}
