#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// LCD pins
const int rsPin = 12;
const int enPin = 11;
const int d4Pin = 5;
const int d5Pin = 4;
const int d6Pin = 3;
const int d7Pin = 2;

// Initialize LiquidCrystal object
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);

// Pin for wire
const int dataPin = 8;

// Setup a OneWire instance to communicate with any OneWire device
OneWire oneWire(dataPin);

// oneWire reference to sensor
DallasTemperature temperatureSensors(&oneWire);

// Current temperature
float currentTemperatureCelsius = 0.0;

// Min temperature 
float minTemp = 0.0;
// Max temperature 
float maxTemp = 0.0;



void printDeviceAddressToLCD(DeviceAddress address) {
    for (uint8_t byteIndex = 0; byteIndex < 8; byteIndex++) {
        if (address[byteIndex] < 16) {
            lcd.print("0");
        }
        lcd.print(address[byteIndex], HEX);
    }
}

void setup(void) {
    lcd.begin(16, 2);
    lcd.print("by Rostislav M.");
    lcd.setCursor(0, 1);
    lcd.print("Temp control");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Start serial communication
    Serial.begin(9600);

    // Start the temperature sensors
    temperatureSensors.begin();

    // Prompt to enter min temperature
    Serial.println("Enter minimum temperature:");
    while (!Serial.available());
    minTemp = Serial.parseFloat();
    Serial.println(minTemp);
    
  // Prompt to enter max temperature
    Serial.println("Enter maximum temperature:");
    while (!Serial.available());
    maxTemp = Serial.parseFloat();
    Serial.println(maxTemp);
}

void loop(void) {
    delay(750);
    // Request readings from connected temperature sensors
    temperatureSensors.requestTemperatures();
    delay(100);

    // Number of devices connected to OneWire
    int numberOfSensors = temperatureSensors.getDeviceCount();
    Serial.print("Number of sensors found: ");
    Serial.println(numberOfSensors);
    
    lcd.clear();
    lcd.print("Found ");
    lcd.print(numberOfSensors);
    lcd.print(" sensors");
    delay(2000);

    // Loop through all connected sensors
    for (int index = 0; index < numberOfSensors; index++) {
        // Fetch the temperature for the current sensor
        float temperatureCelsius = temperatureSensors.getTempCByIndex(index);

        // Buffer to hold the device address
        DeviceAddress deviceAddress;
        if (temperatureSensors.getAddress(deviceAddress, index)) {
            lcd.clear();

            // Print the sensor address 
            lcd.setCursor(0, 0);
            printDeviceAddressToLCD(deviceAddress);

            // Print the temperature
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temperatureCelsius);
            lcd.print((char)223);
            lcd.print("C");

            // Check whether temperature is outside the set range
            if (temperatureCelsius < minTemp || temperatureCelsius > maxTemp) {
                lcd.setCursor(0, 1);
                lcd.print("Out of range!");
                digitalWrite(LED_BUILTIN, HIGH);
            } else {
                // Turn off the built-in LED if temperature is within range
                digitalWrite(LED_BUILTIN, LOW);
            }
            // Delay before next sensor
            delay(750);
        }
    }
}
