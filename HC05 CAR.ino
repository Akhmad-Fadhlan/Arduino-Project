#include <SoftwareSerial.h>
SoftwareSerial module_bluetooth(0, 1); // pin RX | TX
//#define enA 9
#define in1 12
#define in2 11
//#define enB 10
#define in3 10
#define in4 9
int motorSpeedA = 0;
int motorSpeedB = 0;
char data=0;


void setup() {
//  pinMode(enA, OUTPUT);
//  pinMode(enB, OUTPUT);
  Serial.begin(9600);
  Serial.print("halo");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
}


void loop() {
//  digitalWrite(6,HIGH);
//  delay(500);
//  digitalWrite(6,LOW);
//  delay(500);
//  digitalWrite(7,HIGH);
//  delay(500);
//  digitalWrite(7,LOW);
//  delay(500);
  if(Serial.available() > 0)  
  {
    data = Serial.read();
    Serial.print(data);
    if(data == '5'){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    }
    if(data == '0'){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    }
    if(data == '6'){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    }
    if(data == '7'){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    }
    if(data == '8'){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    }
////    motorSpeedA = 200; //bisa diambil dari joystick, tapi sekarang kita input manual aja yak; 0-255
////    motorSpeedB = 200; //bisa diambil dari joystick, tapi sekarang kita input manual aja yak; 0-255
////    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
////    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
//    // Set Motor A backward
//    digitalWrite(in1, HIGH);
//    digitalWrite(in2, LOW);
//    // Set Motor B backward
//    digitalWrite(in3, HIGH);
//    digitalWrite(in4, LOW);
//    delay(5000);
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, HIGH);
//    // Set Motor B forward
//    digitalWrite(in3, LOW);
//    digitalWrite(in4, HIGH);
//    delay(2000);
  }
}
