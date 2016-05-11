/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or 
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include<Servo.h>

const int motor_en = 4;
const int motor_f = 5;
const int motor_t = 6;
const int buzzer = A5;
const int led = A3;
Servo servo1;

USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd,0x00,0x19,0x86,0x00,0x3C,0x65); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

boolean printTemperature;
boolean printAngle;

void setup() {
  Serial.begin(115200);
  
  servo1.attach(3);
  pinMode(motor_en, OUTPUT);
  pinMode(motor_f, OUTPUT);
  pinMode(motor_t, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(motor_en, HIGH);
  
  
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
void loop() {
  Usb.Task();

  if(PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if(PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: ")); 
      Serial.print(PS3.getAnalogHat(LeftHatX));
      servo1.write(map(PS3.getAnalogHat(LeftHatX), 0, 255, 120, 60));      
      Serial.print(F("\tLeftHatY: ")); 
      Serial.print(PS3.getAnalogHat(LeftHatY));
      if(!PS3.PS3NavigationConnected) {
        Serial.print(F("\tRightHatX: ")); 
        Serial.print(PS3.getAnalogHat(RightHatX));
        Serial.print(F("\tRightHatY: ")); 
        Serial.print(PS3.getAnalogHat(RightHatY));
//        if (PS3.getAnalogHat(RightHatY) < 107)
//          analogWrite(motor_f, map(PS3.getAnalogHat(RightHatY), 0, 107, 230, 0));
//        if (PS3.getAnalogHat(RightHatY) > 147) 
//          analogWrite(motor_t, map(PS3.getAnalogHat(RightHatY), 147, 255, 0, 230));      
      }      
    }

    //Analog button values can be read from almost all buttons
    if(PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      Serial.print(F("\r\nL2: ")); 
      Serial.print(PS3.getAnalogButton(L2));
      analogWrite(motor_t, PS3.getAnalogButton(L2));      
      if(!PS3.PS3NavigationConnected) {
        Serial.print(F("\tR2: ")); 
        Serial.print(PS3.getAnalogButton(R2));
        analogWrite(motor_f, map(PS3.getAnalogButton(R2), 0, 255, 0, 230));        
      }
    }
    if(PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    } 
    else {
      if(PS3.getAnalogButton(TRIANGLE)){
        Serial.print(F("\r\nTriangle"));
        digitalWrite(buzzer, HIGH);
      }else       
        digitalWrite(buzzer, LOW);  
      if(PS3.getButtonClick(CIRCLE)){
        Serial.print(F("\r\nCircle"));
        if (digitalRead(motor_en) == HIGH)
          digitalWrite(motor_en, LOW);
        else
          digitalWrite(motor_en, HIGH);
      }  
      if(PS3.getButtonClick(CROSS))
        Serial.print(F("\r\nCross"));
      if(PS3.getButtonClick(SQUARE)){
        Serial.print(F("\r\nSquare"));
        if (digitalRead(led) == HIGH)
          digitalWrite(led, LOW);
        else
          digitalWrite(led, HIGH);
      }
      if(PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));          
        if(PS3.PS3Connected) {
          PS3.setAllOff();
          PS3.setLedOn(LED4);
        }
      } 
      if(PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        if(PS3.PS3Connected) {
          PS3.setAllOff();
          PS3.setLedOn(LED1); 
        }         
      } 
      if(PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        if(PS3.PS3Connected) {
          PS3.setAllOff();
          PS3.setLedOn(LED2);          
        }
      } 
      if(PS3.getButtonClick(LEFT)) {          
        Serial.print(F("\r\nLeft"));          
        if(PS3.PS3Connected) {
          PS3.setAllOff();         
          PS3.setLedOn(LED3);            
        }         
      } 

      if(PS3.getButtonClick(L1))
        Serial.print(F("\r\nL1"));  
      if(PS3.getButtonClick(L3))
        Serial.print(F("\r\nL3"));
      if(PS3.getButtonClick(R1))
        Serial.print(F("\r\nR1"));             
      if(PS3.getButtonClick(R3))
        Serial.print(F("\r\nR3"));

      if(PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - ")); 
        Serial.print(PS3.getStatusString());        
      } 
      if(PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));              
        printAngle = !printAngle;
      }                           
    }
    if(printAngle) {
      Serial.print(F("\r\nPitch: "));               
      Serial.print(PS3.getAngle(Pitch));                  
      Serial.print(F("\tRoll: ")); 
      Serial.print(PS3.getAngle(Roll));
      servo1.write(map((int)PS3.getAngle(Roll), 90, 270, 120, 60));
    }
  }
  else if(PS3.PS3MoveConnected) {
    if(PS3.getAnalogButton(T)) {
      Serial.print(F("\r\nT: ")); 
      Serial.print(PS3.getAnalogButton(T)); 
    } 
    if(PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    } 
    else {
      if(PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect"));
        printTemperature = !printTemperature;
      } 
      if(PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;                          
      } 
      if(PS3.getButtonClick(TRIANGLE)) {            
        Serial.print(F("\r\nTriangle"));
        PS3.moveSetBulb(Red);
      } 
      if(PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.moveSetBulb(Green);
      } 
      if(PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        PS3.moveSetBulb(Blue);
      } 
      if(PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PS3.moveSetBulb(Yellow);
      } 
      if(PS3.getButtonClick(MOVE)) {     
        PS3.moveSetBulb(Off);                        
        Serial.print(F("\r\nMove"));
        Serial.print(F(" - ")); 
        Serial.print(PS3.getStatusString());
      }
    }
    if(printAngle) {
      Serial.print(F("\r\nPitch: "));               
      Serial.print(PS3.getAngle(Pitch));                  
      Serial.print(F("\tRoll: ")); 
      Serial.print((int)PS3.getAngle(Roll));
    }
    else if(printTemperature) {
      Serial.print(F("\r\nTemperature: "));
      Serial.print(PS3.getTemperature());
    }
  }
}
