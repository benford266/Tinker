#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <mcp_can.h>


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

static const int spiClk = 10000000; // 10 MHz
SPIClass hspi(HSPI);

MCP_CAN CAN0(15, &hspi);                           // Set CS to pin 15


bool initial = 1;
bool led_on = false;

bool data_available() {
  return CAN0.checkReceive() == CAN_MSGAVAIL;
}

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

void setup(void) {

startMillis = millis();

//Screen config
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);  // Adding a background colour erases previous text automatically
  tft.drawCentreString("BF Garage",160,110,4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("CAN Display",160,140,2);
  delay(10000);
  tft.fillScreen(TFT_BLACK);

// CAN Config
  Serial.println("Initializing SPI...");
  hspi.begin();
  Serial.println("Initializing CAN...");
  if(CAN0.begin(CAN_125KBPS) == CAN_OK) Serial.println("can init ok!!\r\n");
  else Serial.println("Can init fail!!\r\n");
  pinMode(4, INPUT);                            // Setting pin 4 for /INT input
  Serial.println("MCP2515 Library Receive Example...");
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    UpdateDisplay();
    startMillis = currentMillis; 
  }
 
 CANRead();
 
  
}

void CANRead(){
  // CAN Read
  
    if(data_available())
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
      Serial.print("  Data: ");
      for(int i = 0; i<len; i++)                // Print each byte of the data
      {
        if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
    
      if(rxId == 1017){
        tft.drawCentreString("CLT",110,80,4);
        tft.drawCentreString(String(rxBuf[4]) + "",210,80,4);
      }
      }
      Serial.println();

      digitalWrite(LED_BUILTIN, led_on);
      led_on = !led_on;
    }

}

void UpdateDisplay(){
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  int wmipsi  = random(10,99);
  int clttemp = random(80,99);
  int oilpsi = random(90,99);
  int boostpsi = random(10,22);
  int iattemp = random(10,99);
  tft.drawCentreString("BFGarage CAN Display",160,10,2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("WMI",110,40,4);
  tft.drawCentreString(String(wmipsi) + "PSI",210,40,4);
  tft.drawCentreString("Oil",110,120,4);
  tft.drawCentreString(String(oilpsi) + "PSI",210,120,4);
  tft.drawCentreString("BOOST",110,160,4);
  tft.drawCentreString(String(boostpsi) + "PSI",210,160,4);
  tft.drawCentreString("IAT",110,200,4);
  tft.drawCentreString(String(iattemp) + "C",210,200,4);
  
}

