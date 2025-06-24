# Weather Station (Arduino)

This project implements a simple weather station using an Arduino, an LCD display, buttons, and an analog temperature sensor. It displays either temperature or humidity readings on a 16x2 LCD, depending on the mode selected via button input.

---

## Features

✅ Displays a welcome message on startup  
✅ Switch between Temperature and Humidity display modes using buttons  
✅ Reads and displays temperature from an analog sensor (in °C)  
✅ LCD updates every 1 second  
✅ Button debounce logic to prevent false triggering  

---

## Operation

1️⃣ On startup, the LCD displays **WEATHER STATION**.  
2️⃣ Press the blue button to switch to **Humidity** mode.  
3️⃣ Press the black button to switch to **Temperature** mode.  
4️⃣ Temperature readings are updated on the LCD every second.  
5️⃣ Humidity mode displays a placeholder value (`1.0 %`). (Humidity sensor not yet implemented.)  

---

## TODO

💡 Add a humidity sensor and modify the code to read its value.  
💡 Add data logging via WIFI  
💡 Display min/max values  
