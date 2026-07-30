#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <EEPROM.h>
#include <SD.h>
#include <MFRC522.h>

#define SD_CS_PIN 4      // Chip select pin for SD card module
#define RFID_CS_PIN 10   // Chip select pin for MFRC522 RFID module
#define RFID_RST_PIN 5   // Reset pin for MFRC522 RFID module
#define OUTPUT_PIN 9

String allowedUIDs[] = {
  "chjv68y", // Example UID 1
  "edr44g4",// Example UID 2
};


const int RXPin = 3;        // Connect the GPS TX pin to Arduino pin 3
const int TXPin = 2;        // Connect the GPS RX pin to Arduino pin 2
const int requestPin = 8;   // Pin for triggering data transmission
const int chipSelect = 4;  // Pin for the SD card module's chip select
int numOfKeys = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); // Calculate the number of keys

bool sentData = false;      // Flag to track if data has been sent

MFRC522 mfrc522(RFID_CS_PIN, RFID_RST_PIN); // Create MFRC522 instance
SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);       // Use hardware serial for communication
  gpsSerial.begin(9600);    // Use software serial for GPS module

  pinMode(requestPin, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  
  // Initialize the SD card
  SPI.begin();
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  mfrc522.PCD_Init();
  Serial.println("RFID module initialized.");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uidString += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uidString += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print("RFID card detected! UID: ");
    Serial.println(uidString);
    mfrc522.PICC_HaltA();

    bool matchFound = false;

    // Check if the detected UID matches any of the allowed keys
    for (int i = 0; i < numOfKeys; i++) {
      if (uidString.equals(allowedUIDs[i])) {
        matchFound = true;
        break; // Exit the loop if a match is found
      }
    }

    if (matchFound) {
      digitalWrite(OUTPUT_PIN, HIGH);
      Serial.println("UID matched. Setting OUTPUT_PIN HIGH.");
      delay(500);
      digitalWrite(OUTPUT_PIN, LOW);
    } else {
      digitalWrite(OUTPUT_PIN, LOW);
      Serial.println("UID didn't match. OUTPUT_PIN remains LOW.");
    }

    // Delay to avoid rapid UID checks (adjust as needed)
    delay(1000);
  }


  
  if (digitalRead(requestPin) == HIGH && !sentData) {
    
sendLatLongData();

    
    sentData = true; // Set the flag to true after sending data
  }

  if (digitalRead(requestPin) == LOW) {
    sentData = false; // Reset the flag when pin becomes low
  }

  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        // Save latitude and longitude data to EEPROM only if not already sent
        if (!sentData) {
          saveFloatToEEPROM(0, latitude);
          saveFloatToEEPROM(4, longitude);
        }
      }
    }
  }

   while (Serial.available() > 0) {
     String input = Serial.readStringUntil('\n'); // Read a line from serial
     /////Serial.println(input);

  // Check if the line contains the expected markers and extract the data
  if (input.indexOf("+91") != -1 && input.indexOf("(@)") != -1 ) {
    
      delay(100);

      // Store the number and message in the SD card using extractedNumber
      writeToSDCard(input);
    }
  }
 }

// Function to write data to the SD card

void writeToSDCard(String number) {
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    
    dataFile.println(number);
    dataFile.close();
    /////Serial.println("Data written to SD card.");
  } else {
    /////Serial.println("Error opening data.txt");
  }
}


// Rest of your code...

// Function to save float data to EEPROM
void saveFloatToEEPROM(int address, float value) {
  long scaledValue = value * 10000000; // Scale the value
  byte* p = (byte*)(void*)&scaledValue;
  for (unsigned int i = 0; i < sizeof(scaledValue); i++) {
    EEPROM.write(address + i, *p++);
  }
}

// Function to read float data from EEPROM
float readFloatFromEEPROM(int address) {
  long scaledValue = 0;
  byte* p = (byte*)(void*)&scaledValue;
  for (unsigned int i = 0; i < sizeof(scaledValue); i++) {
    *p++ = EEPROM.read(address + i);
  }
  return scaledValue / 10000000.0; // Convert back to float with decimal point
}

// Function to send latitude and longitude data
void sendLatLongData() {
  delay(100); // Wait for stability
  float latitude = readFloatFromEEPROM(0);
  float longitude = readFloatFromEEPROM(4);

  // Send latitude and longitude data to the receiver Pro Mini
  Serial.print("L:"); // Start marker for latitude
  Serial.print(latitude, 8); // Print latitude with 8 decimal places
  Serial.print(":"); // Separator between latitude and longitude
  Serial.print(longitude, 8); // Print longitude with 8 decimal places
  Serial.println(":E"); // End marker for longitude
}
