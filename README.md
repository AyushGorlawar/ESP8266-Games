
# ESP8266 Games Webserver

This project transforms your ESP8266 into a lightweight web server that hosts interactive games. Users can connect to the ESP8266 via a web browser and enjoy playing games such as **Dino Run**, **Tic Tac Toe**, **Memory Game**, and **Sudoku**.

---

## Features
- **Dino Run**: A classic endless runner game where players control a dinosaur to jump over obstacles.
- **Tic Tac Toe**: A 2-player game to test strategy and planning.
- **Memory Game**: A fun game to challenge your memory by matching pairs of cards.
- **Sudoku Game**: Solve randomly generated Sudoku puzzles of varying difficulty.

---

## Prerequisites

### Hardware
- ESP8266 module (e.g., NodeMCU or ESP-12E)
- USB cable for programming
- Wi-Fi connectivity

### Software
- Arduino IDE (or a similar ESP8266-compatible IDE)
- ESP8266 Board Manager installed in the Arduino IDE
- Required libraries:
  - `ESPAsyncWebServer`
  - `ESPAsyncTCP`
  - `LittleFS`

### Game Files
- HTML, CSS, and JavaScript files for each game.
- Store these files in the ESP8266’s flash memory using LittleFS or SPIFFS.

---

## Installation

### Step 1: Set Up the ESP8266 Environment
1. Install the **ESP8266 Board Manager** in Arduino IDE.
   - Go to `File > Preferences`.
   - Add the following URL to the **Additional Board Manager URLs**:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Open `Tools > Board > Boards Manager` and install the **ESP8266** platform.

2. Install the required libraries using the Library Manager:
   - Open `Sketch > Include Library > Manage Libraries`.
   - Search for and install the following libraries:
     - **ESPAsyncWebServer**
     - **ESPAsyncTCP**
     - **LittleFS**

---

### Step 2: Prepare Game Files
1. Create a folder named `data` in your project directory.
2. Place the HTML, CSS, and JavaScript files for the games in the `data` folder.
3. Use the **LittleFS Data Upload Tool** to upload the `data` folder contents to the ESP8266.

---

### Step 3: Flash the ESP8266
1. Open the provided Arduino sketch file (`ESP8266_Games_Webserver.ino`) in Arduino IDE.
2. Configure the Wi-Fi credentials in the sketch:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
   ```
3. Select the correct board and port:
   - Go to `Tools > Board > NodeMCU 1.0 (ESP-12E Module)` (or your specific ESP8266 board).
   - Select the correct COM port under `Tools > Port`.
4. Click **Upload** to flash the sketch to the ESP8266.

---

### Step 4: Access the Webserver
1. After flashing, open the Serial Monitor to find the ESP8266's IP address.
2. Connect to the ESP8266's Wi-Fi or ensure it is on the same network as your device.
3. Open a web browser and enter the IP address to access the games.

---

## Gameplay Instructions
- **Dino Run**: Press spacebar or tap to make the dinosaur jump over obstacles.
- **Tic Tac Toe**: Players take turns marking spaces on a 3x3 grid to form a row, column, or diagonal.
- **Memory Game**: Flip cards to find matching pairs.
- **Sudoku**: Fill the grid with numbers 1-9 without repeating in rows, columns, or 3x3 sub-grids.

---

## Project Structure
```
ESP8266_Games_Webserver/
│
├── ESP8266_Games_Webserver.ino
└── README.md
```

---

## Troubleshooting
- If the webserver doesn’t load, check:
  - Wi-Fi credentials in the code.
  - Serial Monitor for error messages.
  - LittleFS upload for missing files.
- Verify libraries are properly installed in the Arduino IDE.

---

## Credits
Developed using the ESP8266 platform and open-source libraries. Games implemented using HTML, CSS, and JavaScript.

