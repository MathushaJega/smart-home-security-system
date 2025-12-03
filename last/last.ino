#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID module connections
#define RSTPIN 9
#define SSPIN 10

// Initialize RFID reader and LCD display
MFRC522 rc(SSPIN, RSTPIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C address 0x27, 16 columns, 2 rows

// Sensor and actuator pin definitions
const int IR1_PIN = 2;                 // IR sensor 1 connected to digital pin 2
const int IR2_PIN = 3;                 // IR sensor 2 connected to digital pin 3
const int FLAME_SENSOR_PIN = 4;        // Flame sensor connected to pin 4
const int BUZZER_PIN = 8;              // Buzzer connected to pin 8
const int VIBRATION_SENSOR_PIN = 6;    // Vibration (glass break) sensor on pin 6
const int LOCK_PIN = 7; 
int readsuccess; // Stores result of RFID read

bool allGood = true;

// List of authorized RFID card UIDs (change according to your tags)
byte defcard[][4] = {
  {0x63, 0xFD, 0x50, 0x27},
  {0x73, 0xDB, 0x42, 0x0D}
};
int N = 2; // Number of authorized cards
byte readcard[4]; // Stores UID of the scanned card

bool buzzer_active = false; // Flag to control buzzer

void setup() {
  Serial.begin(9600);

  // Set pin modes
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR1_PIN, INPUT_PULLUP);
  pinMode(IR2_PIN, INPUT_PULLUP);
  pinMode(FLAME_SENSOR_PIN, INPUT_PULLUP);
  pinMode(VIBRATION_SENSOR_PIN, INPUT_PULLUP);

  // Make sure buzzer is OFF initially
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize LCD and turn on backlight
  Wire.begin();  // Start I2C communication
  lcd.init();
  lcd.backlight();

  // Initialize SPI and RFID
  SPI.begin();
  rc.PCD_Init();
  rc.PCD_DumpVersionToSerial(); // Print RFID reader details

  Serial.println(F("Scan Access Card to see Details"));

  // Show system is ready on LCD
  lcd.setCursor(1, 0);
  lcd.print("System Active!");
}

void loop() {
  // Read IR sensor values
  bool ir1_status = digitalRead(IR1_PIN);
  bool ir2_status = digitalRead(IR2_PIN);

  // Human detection logic: both IR sensors must be triggered
  bool human_detected = ir1_status == LOW && ir2_status == LOW;


  bool flame_detected = digitalRead(FLAME_SENSOR_PIN) == LOW;

  bool vibe_detected = digitalRead(VIBRATION_SENSOR_PIN) == LOW;

  allGood = !human_detected && !flame_detected && !vibe_detected;

  if(allGood){

  }


  if (human_detected) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("HUMAN DETECTED");
    readsuccess = getid(); // Try to read RFID tag
    if (readsuccess) {
      int match = 0;

      // Compare the scanned UID with authorized UIDs
      for (int i = 0; i < N; i++) {
        Serial.print("Testing Against Authorized card no: ");
        Serial.println(i + 1);
        if (!memcmp(readcard, defcard[i], 4)) {
          match++;
        }
      }

      if (match) {
        // Card is authorized
        Serial.println("CARD AUTHORIZED");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(" AUTHORIZED");
        digitalWrite(LOCK_PIN, HIGH);
        delay(3000);
        allGood = true;

      } else {
        // Unauthorized card
        Serial.println("CARD NOT Authorized");
        digitalWrite(BUZZER_PIN, HIGH);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(" UNAUTHORIZED");

        delay(2000);
        digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer after warning
      }
    }
  }

  // Flame sensor detection
  if (flame_detected) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("FLAME DETECTED");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000); // Delay for buzzer sound
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Vibration (glass breaking) detection
  if (vibe_detected) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("FORCED DETECTED");


    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000); // Delay for buzzer sound
    digitalWrite(BUZZER_PIN, LOW);
  }


  delay(50); // Small delay to reduce CPU load
}

// Function to read the UID of the RFID card
int getid() {
  // Check if new card is present
  if (!rc.PICC_IsNewCardPresent()) {
    return 0;
  }

  // Read card serial
  if (!rc.PICC_ReadCardSerial()) {
    return 0;
  }

  // Print UID to Serial
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  for (int i = 0; i < 4; i++) {
    readcard[i] = rc.uid.uidByte[i]; // Store UID
    Serial.print(readcard[i], HEX);
  }

  Serial.println("");
  Serial.println("Now Comparing with Authorized cards");

  rc.PICC_HaltA(); // Halt card communication
  return 1;
}
