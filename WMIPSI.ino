// DIY WMI pressure display with ESP32 with OLED and 300PSI pressure sensor.

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor config
float inputP1;
int outputP1;
int SensorMaxPSI = 300; // Max pressure sensor can read
int SensorMinRes = 416; // 0.5v value of 12 bit ADC read
int SensorMaxRes = 3750; // 4.5 value of 12 bit ADC read
int SensorRangeRes = SensorMaxRes - SensorMinRes; // Range of values between 0.5v and 4.5v 



void setup() {
  
  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  }
  delay(2000); // Pause for 2 seconds
 
  // 5 Second intro screen
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("WMI Pressure Display");
  display.setCursor(0, 32);
  display.println("By BFGarage");
  display.display();

  delay(5000);
}

void loop() {
  // Read value from ADC
  inputP1 = analogRead(A12); //12 Bit 
  // Maths to ADC vaule into PSI Unit
  outputP1 = ((inputP1 - SensorMinRes)* SensorMaxPSI /SensorRangeRes);

  // Output to OLED display 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("WMI Pressure");
  display.setTextSize(2);
  display.setCursor(0, 32);
  String wmipsi = String(outputP1);
  display.println(wmipsi + " PSI");
  display.display();
  // 0.5 Second delay to stop screen flicker
  delay(500);




}
