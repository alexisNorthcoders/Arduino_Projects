#include "IRremote.h"
#include <Servo.h>

int receiver = 6; // IR receiver pin

// LED pins
const int led2 = 2;
const int led3 = 3;
const int led4 = 4;

// Servo pin
const int servoPin = 10;

Servo myServo;

bool led4Blinking = false;
unsigned long previousMillis = 0;
const long interval = 500;  // blink interval in milliseconds
bool led4State = LOW;

const int leds[] = {led2, led3, led4};
const int numLeds = sizeof(leds) / sizeof(leds[0]);

void turnOnSelectedLED(int selectedLED) {
  for (int i = 0; i < numLeds; i++) {
    if (leds[i] == selectedLED) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }
}

IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;

void translateIR()
{
  if (irrecv.decodedIRData.flags)
  {
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  }
  else
  {
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }

  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xBA45FF00: // POWER
      Serial.println("POWER button - LEDs off");
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
      digitalWrite(led4,LOW);
      led4Blinking = false;
      break;
      
    case 0xF30CFF00: // 1
      Serial.println("1 -> LED 2 ON");
      turnOnSelectedLED(led2);
      break;

    case 0xE718FF00: // 2
      Serial.println("2 -> LED 3 ON");
      turnOnSelectedLED(led3);
      break;

    case 0xA15EFF00: // 3
      Serial.println("3 -> LED 4 ON");
      turnOnSelectedLED(led4);
      break;

    case 0xF708FF00: // 4
      Serial.println("4 -> LED 2 OFF");
      digitalWrite(led2, LOW);
      break;

    case 0xE31CFF00: // 5
      Serial.println("5 -> LED 3 OFF");
      digitalWrite(led3, LOW);
      break;

    case 0xA55AFF00: // 6
      Serial.println("6 -> LED 4 START BLINKING");
      led4Blinking = true;
      break;

    case 0xBF40FF00: // PLAY/PAUSE
      Serial.println("Servo button pressed");
      myServo.write(0);
      delay(500);
      myServo.write(180);
      delay(500);
      myServo.write(0);
      break;

    default:
      Serial.println("Other button");
      break;
  }

  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();

  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  myServo.attach(servoPin);
}

void loop()
{
    unsigned long currentMillis = millis();

  if (led4Blinking) {
    if (currentMillis - previousMillis >= interval) {
      
      previousMillis = currentMillis;

      led4State = !led4State;
      digitalWrite(led4, led4State);
    }
  }
  if (irrecv.decode())
  {
    translateIR();
    irrecv.resume();
  }
}
