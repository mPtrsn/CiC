#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


// defines pins numbers
const int stepPin = 5; 
const int dirPin = 2; 
const int enPin = 8;
const int reedPin = 10;
const int stepDuration = 500;
const float stepLength = 0.1125f;

#define RIGHT 1
#define LEFT 0

int stepQueue = 0;
int i = 0;
float position = 0.0;
String str = "";
int direction = 0;
int message;

void setDirection(int dir){
  if(dir == 0){
    digitalWrite(dirPin,LOW);
  } else {
    digitalWrite(dirPin,HIGH);
  }
  direction = dir;
}

void oneStep(){
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(stepDuration); 
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(stepDuration);
  position += 360;
  if(direction == 0){
    position -= stepLength;
  } else {
    position += stepLength;
  }
  while (position > 360){
    position -= 360;
  }
}

void refreshParams(int target){
float diff = target - position;
float dist = 0;
if (abs(diff) > 180){
  if (position > target){
    dist = (target + 360) - position;
    setDirection(RIGHT);
  } else {
    dist = (position + 360) - target;
    setDirection(LEFT);
  }
} else {
  dist = abs(diff);
  setDirection(diff > 0 ? RIGHT : LEFT);
}
  stepQueue = (int)(dist / stepLength);
}

void home(){
  setDirection(RIGHT);
  while(!digitalRead(reedPin)){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(200);
  }
  position = 0;
}

int convertMessage(){
  str = "";
  bool terminate = false;
  char c;
//  while(Serial.available() > 0 && !terminate){
//    c = Serial.read();
//    str.concat(c);
//    Serial.println(str);
//     if(c == '\0'||c=='\r'||c=='\n'){
//      Serial.println("terminate");
//      terminate = true;
//    }
//  }

  str = Serial.readString();
  int degree;
  if(str != ""){
    degree = str.toInt();
    Serial.println(degree);
  } else {
    degree = position;
  }
  return degree;
}


void setup() {
  Serial.begin(250000);
  
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  pinMode(reedPin,INPUT);
  
  digitalWrite(enPin,LOW);
  Serial.setTimeout(1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
}
void loop() {
  if(Serial.available() > 0){
    message = Serial.parseInt();
    display.setCursor(10,0);
    display.clearDisplay();
    display.println(message);
    display.display();
    
  }
  if(message == 500){
    home();
    message = 0;
  } else {
    
    refreshParams(message);
    if(stepQueue > 0){
      //Serial.println("else");
      oneStep();
      stepQueue--;
    }
  }
}
