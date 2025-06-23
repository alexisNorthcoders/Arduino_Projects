# IR Remote Controlled LEDs and Servo

This Arduino project allows you to control three LEDs and a servo motor using an IR remote. Each button on the remote can turn LEDs on/off, start blinking, or move the servo.

## Features

- **Control 3 LEDs**: Turn on/off individual LEDs using IR remote buttons.
- **Blinking LED**: Start blinking LED 4 with a remote button.
- **Servo Control**: Move a servo motor between 0° and 180° using the remote.
- **IR Code Handling**: Supports IR repeat codes and prints received codes to the Serial Monitor.

## Hardware Required

- Arduino board (Uno, Nano, etc.)
- IR receiver module (connected to pin 6)
- IR remote control
- 3 LEDs (connected to pins 2, 3, and 4)
- 3 resistors (220Ω recommended)
- Servo motor (connected to pin 10)
- Jumper wires and breadboard

## Pin Connections

| Component      | Arduino Pin |
|----------------|-------------|
| IR Receiver    | 6           |
| LED 2          | 2           |
| LED 3          | 3           |
| LED 4          | 4           |
| Servo          | 10          |

## IR Remote Button Mapping

| Button         | Action                      |
|----------------|-----------------------------|
| POWER          | Turn all LEDs off           |
| 1              | Turn on LED 2               |
| 2              | Turn on LED 3               |
| 3              | Turn on LED 4               |
| 4              | Turn off LED 2              |
| 5              | Turn off LED 3              |
| 6              | Start blinking LED 4        |
| PLAY/PAUSE     | Move servo (0° → 180° → 0°) |

## How to Use

1. **Wire the components** as described above.
2. **Upload** the arduino_IR_leds_servo.ino sketch to your Arduino.
3. **Open the Serial Monitor** at 9600 baud to view IR codes and debug messages.
4. **Use your IR remote** to control the LEDs and servo as per the button mapping.

## Dependencies

- [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) library
- [Servo](https://www.arduino.cc/en/Reference/Servo) library

Install these libraries via the Arduino Library Manager if needed.

---

**Note:** If your remote uses different codes, use the Serial Monitor to read the codes and update the `switch` statement accordingly.