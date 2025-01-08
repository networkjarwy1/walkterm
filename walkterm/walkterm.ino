#include <TFT_eSPI.h>
#include <Arduino.h>

char charTable[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '/', '*', '\'', '!', '@', '#', '$', '%', '^', '&', '(', ')', '-', 
    '_', '=', '+', '[', ']', '{', '}', ';', ':', '"', '<', '>', ',', 
    '.', '?', '\\', '|', '`', '~', '0', '1', '2', '3', '4', '5', '6', 
    '7', '8', '9', '\n', '\r'
};

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

char getKbInput(){
  unit8_t currentByte = 0;
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

  return decode(currentByte);
}

char decode(unit8_t hex){
  return charTable[hex];
}

void execCommand(char command[]) {
  char* token = strtok(command, " ");

  if (token == NULL) {
    return;
  }

  if (strcmp(token, "connect") == 0) {
    token = strtok(NULL, " ");
    
    if (token == NULL) {
      tft.println("you forgot to select device \n valid format: connect <device> wifi <SSID> <password>");
      return;
    }
    char* device = token;

    token = strtok(NULL, " ");
    if (token == NULL || strcmp(token, "wifi") != 0) {
      tft.println("you forgot to select type of connection \n valid format: connect <device> wifi <SSID> <password>");
      return;
    }
    
    token = strtok(NULL, " ");
    if (token == NULL) {
      tft.println("you forgot to select SSID \n valid format: connect <device> wifi <SSID> <password>");
      return;
    }
    char* ssid = token;

    token = strtok(NULL, " ");
    if (token == NULL) {
      tft.println("you forgot to select password \n valid format: connect <device> wifi <SSID> <password>");
      return;
    }
    char* password = token;
  }
}