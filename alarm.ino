//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <RTClib.h>


#include <IRremote.h>

RTC_DS3231 rtc;
DateTime now;

static const uint8_t mp3   = 25;
static const uint8_t ptr   = 26;
static const uint8_t rkm   = 27;


String data;
int al_jam, al_menit, al_jam1, al_menit1,index1, index2, index3, index4, index5, index6, index7, index8, fr, mn,mn1,mn2,mn3,jam1,i,j,hr,RECV_PIN = 15;

unsigned long previousMillis = 0;  // millis instaed of delay
const long interval = 60000;  // blink after ecery 500ms

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Pillow_Reminder"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  rtc.begin();
  irrecv.enableIRIn();
  pinMode(mp3, OUTPUT);
  pinMode(ptr, OUTPUT);
  pinMode(rkm, OUTPUT);
}

void loop() {
  if (SerialBT.available()) {
    data = SerialBT.readString();
  }
  delay(20);
  if (data.length() > 0 && data.indexOf('#')!=0) {
    index1 = data.indexOf('|');
    index2 = data.indexOf('|', index1+1);
    index3 = data.indexOf('|', index2+1);
    
    al_jam = data.substring(index1+1, index2).toInt();
    al_menit = data.substring(index2+1, index3).toInt();
    data = ""; 
    rtc.adjust(DateTime(2014, 1, 21,al_jam,al_menit));
  }
  if(data.length() > 0 && data.indexOf('#')==0){
    index5 = data.indexOf('#');
    index6 = data.indexOf('#', index5+1);
    index7 = data.indexOf('#', index6+1);

    al_jam1 = data.substring(index5+1, index6).toInt();
    al_menit1 = data.substring(index6+1, index7).toInt();
  }
  if (irrecv.decode(&results)){
    irrecv.resume();
    digitalWrite(mp3, LOW); 
    digitalWrite(ptr, LOW); 
    digitalWrite(rkm, LOW); 
    switch(results.value){
      case  0xFFA25D: //power
        fr=now.second();
        while(now.second()<=fr+10){
          Serial.println("mp3 on");
          digitalWrite(mp3, HIGH); 
          }       
//        Serial.println(now.second(), DEC);
//        delay(10000);
//        Serial.println(now.second(), DEC);
      case  0xFF30CF: 
        fr=now.second();
        while(now.second()<=fr+10){
          Serial.println("ptr on");
          digitalWrite(ptr, HIGH);
          }
//        digitalWrite(ptr, HIGH);
//        Serial.println("ptr on");
//        Serial.println(now.second(), DEC);
//        delay(10000);
//        Serial.println(now.second(), DEC);
      case 0xFF7A85: 
        fr=now.second();
        while(now.second()<=fr+10){
          digitalWrite(rkm, HIGH);
          Serial.println("rkm on");
          }
//        digitalWrite(rkm, HIGH);
//        Serial.println("rkm on");
//        Serial.println(now.second(), DEC);
//        delay(10000);
//        Serial.println(now.second(), DEC);
    }
  }
  
  now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  mn1=al_menit1+5;
  mn2=al_menit1+10;
  jam1=al_jam1;
  mn3=al_menit1;
  if(mn1>=59 || mn2>=59 && mn==0)
  {
    mn1=mn1-60;
    mn2=mn2-60;
    mn3=59-al_menit1;
    jam1+=1;
  }
  i==LOW;j=LOW;
  hr = now.hour();
  mn = now.minute();
  if(now.minute()<mn1 && now.minute() >=mn3 && hr==jam1){ 
          Serial.println("ptr on");
          digitalWrite(ptr, HIGH);
          delay(100);
          digitalWrite(ptr, LOW);
    }
    
   if(now.minute()<mn2 && now.minute() >=mn1 && hr==jam1)
      {
        if(i==LOW){
          i=HIGH;
         }
         digitalWrite(mp3, i);
      }
   else  {digitalWrite(ptr, LOW);digitalWrite(mp3, LOW);i=LOW;}
}
