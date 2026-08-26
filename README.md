# Smart Parking System Using STM32
A two-floor smart parking management system based on the **STM32 Nucleo-C031C6** microcontroller. The system uses IR sensors to detect vehicle presence, servo motors to control parking gates, and an I2C LCD to display parking-slot availability.
## Project Overview 
The Smart Parking System is designed to automatically monitor parking spaces and control vehicle entry.
The system contains:
* 2 parking floors
* 3 parking slots on each floor
* 6 parking slots in total
* IR sensors for parking-slot detection
* IR sensors for vehicle/gate detection
* Servo motors for automatic gates
* 16×2 I2C LCD display
* STM32 Nucleo-C031C6 microcontroller
* UART interface for debugging
The system checks the availability of parking slots and directs vehicles to an available floor.
## Objectives
* Automatically detect available and occupied parking slots.
* Display the number of available slots on an LCD.
* Automatically open and close the parking gate.
* Manage parking across two floors.
* Prevent vehicles from entering when all parking slots are occupied.
* Develop the complete system using an STM32 microcontroller.
* Demonstrate an embedded-system-based smart parking application.
##  Main Features
### Floor 1
* 3 parking slots
* 3 IR slot sensors
* 1 vehicle/gate IR sensor
* 1 servo motor
### Floor 2
* 3 parking slots
* 3 IR slot sensors
* 1 vehicle/gate IR sensor
* 1 servo motor
### Display
The LCD displays information such as:
```text
SMART PARKING
F1: 2  F2: 3
```
or:
```text
FLOOR 1 FULL
CHECK FLOOR 2
```
When all slots are occupied:
```text
PARKING FULL
NO SPACE
## Hardware Components
| Component           |    Quantity |
| ------------------- | ----------: |
| STM32 Nucleo-C031C6 |           1 |
| IR Sensors          |           8 |
| Servo Motors        |           2 |
| 16×2 I2C LCD        |           1 |
| Jumper Wires        | As required |
| Breadboard          |           1 |
| External 5V Supply  |           1 |
| USB Cable           |           1 |
## System Architecture
```text
                   ┌─────────────────────┐
                   │   STM32 Nucleo      │
                   │      C031C6          │
                   └──────────┬──────────┘
                              │
              ┌───────────────┼───────────────┐
              │               │               │
              ▼               ▼               ▼
        ┌──────────┐    ┌──────────┐    ┌──────────┐
        │ IR       │    │ Servo    │    │ I2C LCD  │
        │ Sensors  │    │ Motors   │    │ Display  │
        └──────────┘    └──────────┘    └──────────┘
              │               │
              ▼               ▼
        Parking Slots      Gate Control
## Parking Layout
### Floor 1
```text
       FLOOR 1
 ┌─────────┬─────────┬─────────┐
 │ SLOT 1  │ SLOT 2  │ SLOT 3  │
 │   IR1   │   IR2   │   IR3   │
 └─────────┴─────────┴─────────┘
              GATE
               │
             IR GATE
               │
             SERVO
### Floor 2
       FLOOR 2
 ┌─────────┬─────────┬─────────┐
 │ SLOT 1  │ SLOT 2  │ SLOT 3  │
 │   IR1   │   IR2   │   IR3   │
 └─────────┴─────────┴─────────┘
              GATE
               │
             IR GATE
               │
             SERVO
## Pin Configuration
The following pin assignment can be used as the starting configuration in STM32CubeMX.
| Device            | STM32 Pin | Mode       |
| ----------------- | --------- | ---------- |
| Floor 1 Slot 1 IR | PA0       | GPIO Input |
| Floor 1 Slot 2 IR | PA1       | GPIO Input |
| Floor 1 Slot 3 IR | PA4       | GPIO Input |
| Floor 1 Gate IR   | PB0       | GPIO Input |
| Floor 1 Servo     | PA8       | TIM1 PWM   |
| Floor 2 Slot 1 IR | PB1       | GPIO Input |
| Floor 2 Slot 2 IR | PB2       | GPIO Input |
| Floor 2 Slot 3 IR | PB3       | GPIO Input |
| Floor 2 Gate IR   | PB4       | GPIO Input |
| Floor 2 Servo     | PA9       | TIM1 PWM   |
| LCD SCL           | PB6       | I2C        |
| LCD SDA           | PB7       | I2C        |
| UART TX           | PA2       | UART       |
| UART RX           | PA3       | UART       |
> **Note:** Verify the available alternate functions and pin mapping in STM32CubeMX for the exact Nucleo-C031C6 configuration before physically wiring the second servo PWM channel.
## Working Principle
1. The STM32 initializes GPIO, I2C, UART and PWM peripherals.
2. The IR sensors continuously monitor the six parking slots.
3. The STM32 determines whether each slot is occupied or free.
4. The LCD displays the number of available slots.
5. When a vehicle approaches the gate, the gate IR sensor detects it.
6. The controller checks Floor 1.
7. If Floor 1 has an available slot, the Floor 1 servo opens the gate.
8. If Floor 1 is full, the controller checks Floor 2.
9. If Floor 2 has an available slot, the Floor 2 gate is opened.
10. If both floors are full, the LCD displays `PARKING FULL`.
11. When the vehicle passes the gate, the servo closes the gate.
12. Slot occupancy is continuously updated.
## Control Algorithm
START
  │
  ▼
Initialize STM32
  │
  ├── GPIO
  ├── I2C
  ├── UART
  └── PWM
  │
  ▼
Read IR Sensors
  │
  ▼
Calculate Free Slots
  │
  ▼
Display Slot Status
  │
  ▼
Vehicle Detected?
  │
 ┌┴───────────────┐
 NO               YES
 │                 │
 │                 ▼
 │          Check Floor 1
 │                 │
 │          ┌──────┴──────┐
 │          │             │
 │        SPACE          FULL
 │          │             │
 │          ▼             ▼
 │      Open Gate    Check Floor 2
 │                        │
 │                 ┌──────┴──────┐
 │                 │             │
 │               SPACE          FULL
 │                 │             │
 │                 ▼             ▼
 │             Open Gate    PARKING FULL
 │
 └──────────────► Repeat
## Software Requirements
* STM32CubeIDE
* STM32CubeMX
* STM32 HAL Library
* ST-LINK
* USB Cable
* Serial Terminal (optional)
## Project Structure
smart-parking-stm32/
├── README.md
│
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── lcd_i2c.h
│   │   └── parking.h
│   │
│   └── Src/
│       ├── main.c
│       ├── lcd_i2c.c
│       └── parking.c
│
├── Drivers/
│   └── STM32 HAL Drivers
│
├── Documentation/
│   ├── block-diagram.md
│   ├── circuit-diagram.md
│   ├── pin-configuration.md
│   ├── working-principle.md
│   └── testing.md
│
├── Hardware/
│   ├── components.md
│   └── wiring.md
│
├── Images/
│   ├── block-diagram.png
│   ├── circuit-diagram.png
│   └── prototype.jpg
│
├── .gitignore
└── LICENSE
## STM32CubeIDE Configuration
The project uses:
* MCU: **STM32C031C6**
* GPIO inputs for IR sensors
* I2C1 for LCD
* Timer PWM for servo motors
* UART for debugging
### PWM Servo Control
A typical hobby servo uses approximately:
50 Hz PWM frequency
20 ms period
Typical pulse widths:
~1.0 ms → 0°
~1.5 ms → 90°
~2.0 ms → 180°
The exact values should be calibrated for the servo being used.
## Example LCD Output
### Normal Condition
SMART PARKING
F1:2 F2:3
### Floor 1 Full
```text
FLOOR 1 FULL
CHECK FLOOR 2
### All Slots Full
```text
PARKING FULL
NO SPACE
---
## Testing
The system should be tested under the following conditions:
| Test                             | Expected Result             |
| -------------------------------- | --------------------------- |
| No vehicles                      | All slots show available    |
| One slot occupied                | Available count decreases   |
| Three Floor-1 slots occupied     | Floor 1 shows FULL          |
| Floor 1 full + Floor 2 available | Vehicle directed to Floor 2 |
| All six slots occupied           | PARKING FULL                |
| Vehicle detected at gate         | Gate servo opens            |
| Vehicle passes gate              | Gate servo closes           |
---
## Future Improvements
Possible future versions can include:
* IoT monitoring
* Blynk mobile application
* ESP32 Wi-Fi connectivity
* Cloud-based parking monitoring
* RFID-based vehicle identification
* Automatic number-plate recognition
* Mobile parking reservation
* OLED display
* Real-time parking statistics
* Web dashboard
* Solar-powered parking system
---
## Project Images
Add your actual hardware photographs inside the `Images` folder.
Recommended images:
```text
Images/
├── prototype.jpg
├── circuit.jpg
├── stm32-board.jpg
├── lcd-output.jpg
└── parking-model.jpg
```
---
## Author
**V. Mohan Vamsi Krishna**
ECE | Embedded Systems | IoT | STM32
---
## Project
If you find this project useful, consider giving the repository .

This project is intended for educational and academic purposes.
