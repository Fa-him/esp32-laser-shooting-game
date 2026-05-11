# IoT Based Laser Shooting Game using ESP32 and LDR Sensors

A low-cost embedded systems project that combines an ESP32 target board, LDR-based laser detection, LED target indicators, buzzer feedback, and a local web dashboard for score and timer monitoring.

This was developed as a team project for a Microprocessor, Microcontrollers and Embedded System sessional course. My main contribution focused on the **main target board**, including component integration, ESP32 wiring, and the Arduino/ESP32 code for game logic, hit detection, buzzer feedback, scoring, timer control, Wi-Fi server, and web interface.

> Note: This is an educational prototype for electronics, embedded systems, and IoT learning. Use only safe, low-power laser modules and avoid pointing lasers at eyes.

---

## Project Overview

The system has two main parts:

1. **Laser Transmitter / Gun Module**
   - Uses a 555 timer circuit in monostable mode.
   - Generates a short laser pulse when the push button is pressed.

2. **ESP32 Target Board / Receiver Module**
   - Uses LDR sensors to detect laser hits.
   - Uses LEDs to indicate active targets.
   - Uses a buzzer for hit and game feedback.
   - Hosts a local web page through ESP32 Wi-Fi.
   - Displays score, remaining time, waiting state, and game-over status.

---

## Main Features

- ESP32-based target board
- 5 LED target indicators
- 5 LDR laser-detection inputs
- Buzzer feedback for hit/start/end signals
- Random target selection
- 2-minute game timer
- 3-second target timeout
- Start mode: hit Target 3 to begin
- Local web dashboard using ESP32 web server
- `/data` endpoint for live JSON game data
- `/restart` endpoint to restart the game

---

## Hardware Components

### Main Target Board

- ESP32 DevKit board
- 5 × LDR sensors
- 5 × LEDs
- 5 × LED current-limiting resistors
- Voltage-divider resistors for LDR inputs
- Active buzzer
- Breadboard and jumper wires
- USB cable for ESP32 power/programming

### Laser Transmitter Module

- 555 timer IC
- Push button switch
- Laser diode/module
- Resistors and capacitors for monostable timing
- NPN transistor driver
- Battery/power supply
- Breadboard and jumper wires

---

## ESP32 Pin Configuration

| Function | ESP32 GPIO Pins |
|---|---|
| LED Targets | GPIO 13, 12, 14, 27, 26 |
| LDR Sensors | GPIO 39, 33, 32, 35, 34 |
| Buzzer | GPIO 4 |

The code uses Target 3 as the start target. In the array, Target 3 is connected to `ldrPins[2]`, which is GPIO 32.

---

## Software Requirements

- Arduino IDE
- ESP32 board support package installed in Arduino IDE
- USB driver for your ESP32 board, if required
- A Wi-Fi network for the ESP32 local web dashboard

---

## How to Run the Project

1. Open the Arduino IDE.
2. Create a new sketch named `esp32_laser_target_game`.
3. Copy the provided `.ino` code into the sketch.
4. Update the Wi-Fi name and password:

```cpp
const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";
```

5. Select the correct ESP32 board from **Tools > Board**.
6. Select the correct COM port from **Tools > Port**.
7. Upload the code to the ESP32.
8. Open the Serial Monitor at `115200` baud.
9. After Wi-Fi connects, copy the IP address shown in the Serial Monitor.
10. Open the IP address in a browser connected to the same Wi-Fi network.
11. Hit Target 3 with the laser to start the game.
12. Shoot the active LED targets to increase the score.

---

## Repository Structure

Suggested GitHub folder structure:

```text
esp32-laser-shooting-game/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   └── esp32_laser_target_game/
│       └── esp32_laser_target_game.ino
│
├── docs/
│   └── project-report.pdf
│
└── images/
    ├── target-board.jpg
    ├── laser-transmitter.jpg
    └── circuit-diagram.png
```

---

## Web Dashboard

The ESP32 hosts a simple local web interface. The dashboard shows:

- Current score
- Remaining time
- Game status
- Game-over screen
- Restart button

Available routes:

| Route | Purpose |
|---|---|
| `/` | Main web dashboard |
| `/data` | Returns live score, time, waiting state, and game-over state as JSON |
| `/restart` | Restarts the game |

Example JSON response from `/data`:

```json
{
  "score": 5,
  "time": 87,
  "gameOver": false,
  "waiting": false
}
```

---

## My Contribution

My main contribution was focused on the ESP32 receiver/target board side of the project. I worked on:

- Main target board component setup
- ESP32 pin configuration
- LDR sensor integration
- LED target indicator wiring
- Buzzer feedback connection
- Arduino/ESP32 coding
- Game start, scoring, timer, restart, and game-over logic
- Local Wi-Fi web server implementation
- Web dashboard interface for live score and timer display

---

## Limitations

- Ambient light can affect LDR readings.
- Threshold values may need manual tuning.
- Breadboard wiring can become unstable.
- The current version supports a limited number of targets.
- The web dashboard works only on the local Wi-Fi network.
- No cloud database or mobile app is included yet.

---

## Future Improvements

- Build a custom PCB for the target board.
- Add calibration mode for automatic LDR threshold adjustment.
- Add multiplayer support.
- Add mobile app or cloud dashboard.
- Add different game modes and difficulty levels.
- Improve enclosure and target-board design.
- Add battery-powered operation.

---

## Safety Notes

- Use only low-power laser modules suitable for educational projects.
- Never aim the laser at eyes, faces, reflective surfaces, or animals.
- Test the system in a controlled indoor environment.
- Supervise use if children or inexperienced users are present.

---

## License

This project is shared for educational purposes. You can add a license such as the MIT License if you want others to reuse or modify your code.
