# SOS-PHONE

# Arduino SOS Mobile Phone 📱🆘

A fully functional, custom-built mobile phone powered by Arduino. While it includes standard phone capabilities like making calls and sending SMS messages, its primary focus is personal safety: it features a dedicated SOS system that instantly broadcasts your live GPS location to predefined emergency contacts.

## ✨ Features

* **One-Tap SOS Emergency Alert:** Instantly fetches GPS coordinates and sends an emergency SMS with a Google Maps link to family members.
* **Live GPS Tracking:** Uses the NEO-6M module for highly accurate location data.
* **Basic Telephony:** Make and receive phone calls via the GSM network.
* **SMS Capabilities:** Send and read text messages.
* **Intuitive Interface:** Navigable UI utilizing an I2C LCD display and an I2C keypad.


### 📸 Photos and Video demonstration:

<p align="center">
  <img src="path/to/your/home-screen-screenshot.png" alt="Phone Home Screen" width="45%">
  &nbsp; &nbsp;
  <img src="path/to/your/sos-alert-screenshot.png" alt="SOS Alert Function" width="45%">
</p>

### 🎥 Video Demonstration
*(Click the image below to watch the video demonstration. Replace `YOUR_VIDEO_ID` with your actual YouTube video ID, or link directly to an MP4 file in your repo)*

[![SOS Phone Demonstration](https://img.youtube.com/vi/YOUR_VIDEO_ID/maxresdefault.jpg)](https://youtu.be/YOUR_VIDEO_ID)



## 🛠️ Hardware Components

To build this project, you will need the following components:

* **Microcontrollers:** ATmega328P And Arduino Pro mini.
* **GSM/GPRS Module:** SIM800L (Requires a stable power supply, often external 3.7V - 4.2V).
* **GPS Module:** NEO-6M GPS Module.
* **Display:** I2C Display (e.g., 16x2 LCD or 128x64 OLED).
* **Input:** I2C Keypad.
* **Power Supply:** Two lithium-ion batteries.
* **Audio:** Pam8403 module and Micro speaker and electret microphone (for SIM800L audio pins). 
* **Bluetooth:** KCX-BT003 for Bluetooth audio.
* **NFC:** MFRC522 Module to use the Mastercard for the password.
* **RTC:"" An RTC module to store the time.
* **SD Card R/W:** Any SD card reader and writer module and a tf player.
* **Charger:** TP4056 module.

## 🔌 Wiring Overview

*Note: The SIM800L module can draw up to 2A during transmission. Do not power it directly from the Arduino's 5V pin. Use a dedicated buck converter or battery.*

| Component | Arduino Pin | Notes |
| :--- | :--- | :--- |
| **SIM800L TX** | SoftwareSerial RX (e.g., Pin 10) | Requires logic level shifting if using 5V Arduino |
| **SIM800L RX** | SoftwareSerial TX (e.g., Pin 11) | Requires logic level shifting if using 5V Arduino |
| **NEO-6M TX** | SoftwareSerial RX (e.g., Pin 4) | Or use Hardware Serial if on Arduino Mega |
| **NEO-6M RX** | SoftwareSerial TX (e.g., Pin 5) | |
| **I2C Display (SDA)**| SDA (A4 on Uno/Nano) | |
| **I2C Display (SCL)**| SCL (A5 on Uno/Nano) | |
| **I2C Keypad (SDA)** | SDA (A4 on Uno/Nano) | Connect in parallel with Display |
| **I2C Keypad (SCL)** | SCL (A5 on Uno/Nano) | Connect in parallel with Display |

## 💻 Software & Libraries

Ensure you have the latest version of the Arduino IDE installed. This project relies on the following libraries (which can be installed via the Arduino Library Manager):

**Core & Communication:**
* `SPI.h` (Built-in) - Required for SD Card and RFID module.
* `Wire.h` (Built-in) - For I2C components (Display, Keyboard, RTC).
* `SoftwareSerial.h` (Built-in) - For SIM800L and NEO-6M communication.

**Display & Input:**
* `LiquidCrystal_I2C` - For the I2C display.
* `Adafruit_PCF8574` - I2C Port Expander used for the keypad.
* `Keypad` and `Keypad_I2C` - For reading matrix keyboard inputs.

**Sensors & Time:**
* `TinyGPS++` - For parsing NMEA data from the GPS module.
* `uRTCLib` - For managing the Real-Time Clock module.

**Storage & Peripherals:**
* `EEPROM.h` (Built-in) - Onboard memory for saving settings or contacts.
* `SD.h` (Built-in) - For reading/writing to the SD card.
* `MFRC522` - For the RFID reader.

**Power Management:**
* `avr/sleep.h` & `avr/power.h` (Built-in for AVR) - To optimize battery life by sleeping unused peripherals.


## ⚠️ Important Considerations

* **2G Network Availability:** The SIM800L is a 2G module. Ensure your local telecom providers still support 2G networks, as many countries are sunsetting them.
* **GPS Lock:** The NEO-6M requires a clear view of the sky to get an initial satellite lock. This can take a few minutes on a cold start.
* **Power Management:** GSM modules are highly sensitive to power fluctuations. A voltage drop will cause the SIM800L to reset during a call or SMS transmission. 

## 👨‍💻 Author

**Aritra Barman**
* GitHub: [@Aritra709](https://github.com/Aritra709)

## 📝 License

This project is open-source and available under the [MIT License](LICENSE).
