# SOS-PHONE

# Arduino SOS Mobile Phone 📱🆘

A fully functional, custom-built mobile phone powered by Arduino. While it includes standard phone capabilities like making calls and sending SMS messages, its primary focus is personal safety: it features a dedicated SOS system that instantly broadcasts your live GPS location to predefined emergency contacts.

## ✨ Features

* **One-Touch SOS Emergency Alert:** Instantly fetches GPS coordinates and sends an emergency SMS with a Google Maps link to family members.
* **Live GPS Tracking:** Uses the NEO-6M module for highly accurate location data.
* **Basic Telephony:** Make and receive phone calls via the GSM network.
* **SMS Capabilities:** Send and read text messages.
* **Intuitive Interface:** Navigable UI utilizing an I2C LCD/OLED display and an I2C keypad.

## 🛠️ Hardware Components

To build this project, you will need the following components:

* **Microcontroller:** Arduino (Uno, Nano, or Mega depending on your pin/memory requirements)
* **GSM/GPRS Module:** SIM800L (Requires a stable power supply, often external 3.7V - 4.2V)
* **GPS Module:** NEO-6M GPS Module
* **Display:** I2C Display (e.g., 16x2 LCD or 128x64 OLED)
* **Input:** I2C Keyboard/Keypad
* **Power Supply:** Lithium-ion battery (18650) with a boost converter or a dedicated power module for the SIM800L.
* **Audio:** Micro speaker and electret microphone (for SIM800L audio pins).

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

1. `SoftwareSerial` (Built-in) - For communicating with SIM800L and NEO-6M (if not using hardware serial).
2. `TinyGPS++` - For parsing the NMEA data from the NEO-6M GPS.
3. `LiquidCrystal_I2C` or `Adafruit_SSD1306` - Depending on your specific I2C display.
4. `Keypad_I2C` - For reading the I2C keyboard inputs.

## ⚠️ Important Considerations

* **2G Network Availability:** The SIM800L is a 2G module. Ensure your local telecom providers still support 2G networks, as many countries are sunsetting them.
* **GPS Lock:** The NEO-6M requires a clear view of the sky to get an initial satellite lock. This can take a few minutes on a cold start.
* **Power Management:** GSM modules are highly sensitive to power fluctuations. A voltage drop will cause the SIM800L to reset during a call or SMS transmission. 

## 👨‍💻 Author

**Aritra Barman**
* GitHub: [@<Aritra709>](https://github.com/<Aritra709>)

## 📝 License

This project is open-source and available under the [MIT License](LICENSE).
