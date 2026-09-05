# Remote Car (IR Controlled) + Safety Sensors (Planned)

An Arduino-based remote-controlled car using an IR remote for driving + speed control.  
Next step: add **front + rear distance sensors** so the Arduino acts as a **safety layer** that blocks only the dangerous direction (instead of freezing the whole car).

## What’s in this folder

- `remote_car.ino`: main Arduino sketch (IR receiver + dual DC motor driver control).
  - Controls **left** and **right** motor groups (each group can be two motors/wheels).

## Current features (today)

- **IR remote driving**: forward / backward / left / right / stop
- **Speed levels**: 9 steps (10% → 90% mapped to PWM)
- **Differential steering**:
  - While moving: slows one side for a turn
  - While stopped: pivots in place

## Hardware (current)

- **Arduino**: Uno / Nano (or compatible)
- **IR receiver module** (uses the `IRremote` library)
- **Motor driver**: typical dual H-bridge (ex: L298N / similar)
- **4 DC motors**, wired as **two sides**:
  - Left wheels (pair) on one H-bridge channel
  - Right wheels (pair) on the other H-bridge channel
- Battery / power wiring appropriate for your motors + driver

## Pin mapping (current sketch)

From `remote_car.ino`:

| Purpose | Arduino Pin |
|---|---|
| IR Receiver signal | D2 |
| Motor driver IN1 | D8 |
| Motor driver IN2 | D9 |
| Motor driver IN3 | D10 |
| Motor driver IN4 | D11 |
| Motor driver ENA (PWM) | D5 |
| Motor driver ENB (PWM) | D6 |

## IR command mapping (current sketch)

These are the command bytes used by `remote_car.ino` (your remote must match these):

| Action | IR Command |
|---|---|
| Forward | `0x09` |
| Backward | `0x07` |
| Left | `0x44` |
| Right | `0x43` |
| Stop | `0x45` |
| Speed 1..9 | `0x0C, 0x18, 0x5E, 0x08, 0x1C, 0x5A, 0x42, 0x52, 0x4A` |

Tip: if your remote differs, open Serial Monitor at **9600** and press buttons to see what the sketch prints (`Command: 0x..`).

## Planned: collision safety layer (front + rear sensors)

Goal: the **remote still controls the car normally**, but the Arduino blocks motion only when it would cause a collision.

### Front sensor — HC‑SR04 (ultrasonic)

Use it to protect against **forward collisions**.

Example behavior:

- **> 30 cm** → normal forward driving
- **≤ 30 cm** → **block forward movement**
  - Reverse is still allowed
  - Turning/pivoting can remain allowed (tunable)

### Rear sensor — Sharp GP2D12 (IR distance)

Use it to protect against **reverse collisions**.

Example behavior:

- **~20 cm** → reverse normally
- **≤ ~20 cm** → **block reverse movement**
  - Forward is still allowed

### What this means in practice

- **UP + wall in front** → car won’t drive into it  
- **DOWN + wall behind** → car won’t reverse into it  

Most importantly: sensors should **only block the dangerous direction**, rather than making the car completely unresponsive.

## Planned wiring (non-breaking)

Your existing **IR receiver + motor wiring stays the same**. We just add sensors on free pins.

### Suggested pins (Arduino Uno/Nano friendly)

The current sketch uses: `D2, D5, D6, D8, D9, D10, D11`  
Free digital pins typically include: `D3, D4, D7, D12, D13` and analog inputs `A0..A5`.

Recommended additions:

| Component | Pin(s) |
|---|---|
| HC‑SR04 TRIG | D3 |
| HC‑SR04 ECHO | D4 |
| GP2D12 analog output | A0 |
| “Front blocked” LED | D12 (via 220Ω resistor) |
| “Rear blocked” LED | D13 (via 220Ω resistor) |
| Buzzer / small speaker (horn) | D7 (use a transistor driver if needed) |

Notes:

- **HC‑SR04**: power at 5V, GND to GND. (If you ever switch to a 3.3V board, the ECHO pin needs level shifting.)
- **GP2D12**: connect **VCC, GND, and OUT → A0** (keep wiring short; add decoupling capacitor if readings are noisy).
- **Buzzer/speaker**: a piezo buzzer can often be driven directly; a speaker usually needs a transistor/driver.

## Planned software logic (high level)

- Read sensors continuously (or at a fixed interval).
- Determine two boolean “locks”:
  - `frontBlocked` = (frontDistanceCm ≤ 30)
  - `rearBlocked` = (rearDistanceCm ≤ ~20)
- When an IR command asks for motion:
  - If command is **forward** and `frontBlocked` → **ignore forward command / stop forward**
  - If command is **backward** and `rearBlocked` → **ignore backward command / stop reverse**
  - Otherwise → behave normally
- LEDs indicate which direction is currently blocked.
- Horn button on the remote triggers a buzzer/speaker sound (planned).

## Roadmap (next tasks)

- Add HC‑SR04 distance measurement + smoothing (median/average)
- Add GP2D12 analog-to-distance calibration (table or curve fit)
- Implement directional blocking (forward-only / reverse-only)
- Add LED indicators for blocked states
- Add horn sound (buzzer) + optional “warning beeps” when close
- Tune thresholds (30 cm front, ~20 cm rear) based on your chassis speed + stopping distance

## Build / Upload

1. Open `remote_car.ino` in the Arduino IDE
2. Install dependency: `IRremote` (Arduino Library Manager)
3. Select board + port
4. Upload
5. Open Serial Monitor at **9600** for debugging

