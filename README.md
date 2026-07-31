# 🌱 PlantAlive

PlantAlive is an automated plant watering system designed to help users keep their plants healthy with minimal effort. Using environmental sensors and an ESP32, the system continuously monitors plant conditions and automatically waters the plant when the soil becomes too dry.

## Features

- 🌡️ Monitors temperature and humidity (DHT11)
- 🌱 Measures soil moisture
- ☀️ Detects ambient light level
- 💧 Automatically activates the water pump when soil moisture is too low
- 📺 Displays live sensor readings on a 0.92" OLED display
- ♻️ Built with a recycled toy housing and 3D-printed components

## Hardware

- ESP32 Development Board
- DHT11 Temperature & Humidity Sensor
- Soil Moisture Sensor
- Light Sensor (LDR)
- Mini Water Pump
- 0.92" OLED Display (I2C)
- USB Power Source / Power Bank

## How It Works

The ESP32 reads sensor values every 2 seconds.

- Temperature and humidity are measured using the DHT11 sensor.
- Soil moisture is converted into a percentage using calibrated values.
- Light intensity is measured using the light sensor.
- Sensor readings are displayed on the OLED screen.
- If soil moisture drops below **10%**, the pump turns on automatically.
- Once soil moisture reaches **45%**, the pump stops.

## Usage

> **Warning**
>
> Only the water pump should be submerged in water. Keep the main device above the water level.

1. Insert the soil moisture sensor into the soil until just before the Dupont wire connection.
2. Fill the water container until the pump head is fully submerged.
3. Connect the USB cable to a power bank or other USB power source.
4. The OLED screen will display:
   - Temperature
   - Humidity
   - Soil Moisture
   - Light Level
5. When soil moisture falls below **10%**, the pump automatically waters the plant until the moisture reaches **45%**.

## Software

- Arduino IDE
- C++

## Libraries

- Wire
- Adafruit GFX
- Adafruit SSD1306
- DHT Sensor Library