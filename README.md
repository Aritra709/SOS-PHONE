# SOS-PHONE

# Arduino SOS Mobile Phone 📱🆘

A fully functional, custom-built mobile phone powered by Arduino. While it includes standard phone capabilities like making calls and sending SMS messages, its primary focus is personal safety: it features a dedicated SOS system that instantly broadcasts your live GPS location to predefined emergency contacts.

## ✨ Features

* **One-Tap SOS Emergency Alert:** Instantly fetches GPS coordinates and sends an emergency SMS with a Google Maps link to family members.
* **Live GPS Tracking:** Uses the NEO-6M module for highly accurate location data.
* **Basic Telephony:** Make and receive phone calls via the GSM network.
* **SMS Capabilities:** Send and read text messages.
* **Intuitive Interface:** Navigable UI utilizing an I2C LCD display and an I2C keypad.


### 📸 Photos:

<p align="center">
 <img alt="20260730_200351" src="https://github.com/user-attachments/assets/e9f8289f-0fbb-495f-819f-9e7ddfacbaab" alt="Phone Home Screen" width="35%">
 
 <p align="center">
  <img  alt="Untitled" src="https://github.com/user-attachments/assets/da74df62-6d75-472d-aa4b-a1d00c85d06a" alt="SOS Alert Function" width="45%" />
   &nbsp; &nbsp;
   <img  alt="drg" src="https://github.com/user-attachments/assets/5b42868b-1b7e-485d-9e62-788b759f7e25" alt="SOS Alert Function" width="45%" />
   <img width="1833" height="1549" alt="sef" src="https://github.com/user-attachments/assets/ff454295-ce65-4c90-a890-135b69e618aa" width="5%" />



</p>

### 🎥 Video Demos:
*(Click the image below to watch the video demonstration.)*

<p align="center">
  <!-- BOOT Video -->
  <a href="https://youtube.com/shorts/hF9DGpXGsvw">
    <img src="https://github.com/user-attachments/assets/8fa3188d-1c1c-4250-b4cc-8a0855229937" alt="BOOT" width="30%">
  </a>
  &nbsp;
  <!-- Call/SMS Video -->
  <a href="https://youtube.com/shorts/Tm3HLeo2grg?feature=share">
    <img src="https://github.com/user-attachments/assets/871e1136-ebb0-4a51-ae54-3271140e3a60" alt="Call/SMS" width="30%">
  </a>
  &nbsp;
  <!-- SOS Video -->
  <a href="https://youtube.com/shorts/WKbaUc60OSI?feature=share">
    <img src="https://github.com/user-attachments/assets/117eee3c-9027-4770-bac7-c3eb3e3f89f5" alt="SOS" width="30%">
  </a>
</p>


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
