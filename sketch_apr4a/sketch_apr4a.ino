#include <Arduino.h>
#include<SPI.h>
#include<MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Define pins
const int IR1_PIN = 2;    // PD2
const int IR2_PIN = 3;    // PD3
const int LED_PIN = 11;   // PB3
const int FLAME_LED_PIN = 13;  // PB5
const int VIBRATION_LED_PIN = 12; // PB4
const int FLAME_SENSOR_PIN = 4;  // PD4
const int VIBRATION_SENSOR_PIN = 6; // PD6
const int PIR_SENSOR_PIN = 5;   // PD5
const int BUZZER_PIN = 10;      // PB2
#define RSTPIN 9
#define SSPIN 10
MFRC522 rc(SSPIN, RSTPIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
    // Set outputs (LEDs + buzzer)
    pinMode(LED_PIN, OUTPUT);
    pinMode(FLAME_LED_PIN, OUTPUT);
    pinMode(VIBRATION_LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    // Set inputs (sensors)
    pinMode(FLAME_SENSOR_PIN, INPUT_PULLUP);
    pinMode(VIBRATION_SENSOR_PIN, INPUT_PULLUP);
    pinMode(PIR_SENSOR_PIN, INPUT_PULLUP);
    pinMode(IR1_PIN, INPUT_PULLUP);
    pinMode(IR2_PIN, INPUT_PULLUP);
    // Initialize all outputs to LOW
    digitalWrite(LED_PIN, LOW);
    digitalWrite(FLAME_LED_PIN, LOW);
    digitalWrite(VIBRATION_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    }
    void loop() {
    bool buzzer_active = false;
    bool led_active = false;
    
    // Read sensor states
    bool pir_status = digitalRead(PIR_SENSOR_PIN);
    bool ir1_status = digitalRead(IR1_PIN);
    bool ir2_status = digitalRead(IR2_PIN);

    // IR Sensor Logic
    if (ir1_status == LOW && ir2_status == LOW) {
        // Both IR sensors triggered
        led_active = true;
        buzzer_active = true;
    }
    // All other cases (one or none triggered)
    // LED and buzzer remain OFF (default state)

    // Set LED state
    digitalWrite(LED_PIN, led_active ? HIGH : LOW);

    // Check Flame sensor
    if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
        digitalWrite(FLAME_LED_PIN, !digitalRead(FLAME_LED_PIN)); // Toggle LED
        buzzer_active = true;
        delay(200); // Blink delay
    } else {
        digitalWrite(FLAME_LED_PIN, LOW);
    }

    // Check Vibration sensor
    if (digitalRead(VIBRATION_SENSOR_PIN) == LOW) {
        digitalWrite(VIBRATION_LED_PIN, !digitalRead(VIBRATION_LED_PIN)); // Toggle LED
        buzzer_active = true;
        delay(200); // Blink delay
    } else {
        digitalWrite(VIBRATION_LED_PIN, LOW);
    }

    // Buzzer control
    digitalWrite(BUZZER_PIN, buzzer_active ? HIGH : LOW);

    delay(50); // Small delay to reduce CPU load
}