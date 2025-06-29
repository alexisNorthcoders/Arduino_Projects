# Arduino LEDs and Servo with Buttons

This Arduino project lets you control three LEDs and a servo motor using three push buttons.

## Features

- Control 3 LEDs (red, green, yellow) with individual buttons
- Move the servo to 0° or 90° using the red and green buttons

## Pin Connections

| Component      | Arduino Pin |
|----------------|-------------|
| Red LED        | 2           |
| Green LED      | 3           |
| Yellow LED     | 4           |
| Yellow Button  | 6           |
| Servo          | 10          |
| Green Button   | 11          |
| Red Button     | 12          |

## How it works

- Press the **Yellow Button** to turn on the yellow LED.
- Press the **Green Button** to turn on the green LED and move the servo to 90°.
- Press the **Red Button** to turn on the red LED and move the servo to 0°.

Only one LED is on at a time.