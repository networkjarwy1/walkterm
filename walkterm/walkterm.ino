#include <TFT_eSPI.h>

#define CS 0
#define DATA 1
#define CLK 2

TFT_eSPI tft = TFT_eSPI();

char input[];
int i = 0;

void setup() {
  pinMode(26, OUTPUT);
  pinMode(CS, INPUT_PULLDOWN);
  pinMode(DATA, INPUT_PULLDOWN);
  pinMode(CLK, INPUT_PULLDOWN);
  digitalWrite(26, HIGH);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.println("Waiting for input...");
}

void loop() {
  while(digitalRead(CS)){
    input[i] = getKbInput();
    if(input[i] == "\n"){
      i = 0;
      break;
    }
    i++;
  }
}

byte getKbInput(){
  byte currentByte = 0;
  int bitCount = 0;
  
  bool lastCLKstate = LOW;

  while (bitCount < 8) {
    bool CLKstate = digitalRead(CLK);

    if (CLKstate != lastCLKstate) {
      int currentBit = digitalRead(DATA);
      currentByte = (currentByte << 1) | currentBit;
      lastCLKstate = !lastCLKstate;
      bitCount++;
    }
  }

  return currentByte;
}
