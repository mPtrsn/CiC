#include <SoftwareSerial.h>

#include <Arduino.h>

// This simple code allow you to send data from Arduino to Unity3D.

// uncomment "NATIVE_USB" if you're using ARM CPU (Arduino DUE, Arduino M0, ..)
//#define NATIVE_USB

// uncomment "SERIAL_USB" if you're using non ARM CPU (Arduino Uno, Arduino Mega, ..)
#define SERIAL_USB

SoftwareSerial mySerial(2,3); // RX, TX

String message;
int leftButton = 100;
int rightButton = 100;
int speedVal = 0;
String readData;

unsigned long lastMillis;

int secondsRemaining = 5;
void setup() {
  #ifdef NATIVE_USB
    SerialUSB.begin(1); //Baudrate is irevelant for Native USB
  #endif

  #ifdef SERIAL_USB
    Serial.begin(250000); // You can choose any baudrate, just need to also change it in Unity.
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
  #endif

  mySerial.begin(4800);
  
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  lastMillis = millis();
}

void sendMessage(){
  message = "Alive";
  
  message = handleLRMovement(message);
  message = handleFBMovement(message);
  message = coinInput(message);
  sendData(message);
}
String handleLRMovement(String msg){
  return msg +","+ analogRead(A1);
}

String handleFBMovement(String msg){
  
  return msg +","+ analogRead(A2);
}

String readSerial(){
  String toReturn ="Rot:  ";
  toReturn += Serial.readString();
  //sendData(toReturn);
  return toReturn;
}
// Run forever
void loop() {
  sendMessage();
  
  delay(10); // Choose your delay having in mind your ReadTimeout in Unity3D
}
int i;

String coinInput(String msg){
  secondsRemaining = 0;
        // any input coming from coin acceptor?
        if (mySerial.available()) {
            // read input, which is a 1 byte integer
            i=mySerial.read();
            // ignore any 255 amounts
            //Serial.println(i);
            if (i != 255) {
                // increment time based on coin amount
                secondsRemaining = 30;       
                }
            }
        
        return msg + ","+secondsRemaining;
}

void sendData(String data){
   #ifdef NATIVE_USB
    SerialUSB.println(data); 
  #endif

  #ifdef SERIAL_USB
    Serial.println(data); // need a end-line because wrmlh.csharp use readLine method to receive data
  #endif
}
