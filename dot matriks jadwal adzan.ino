#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* ssid = "Teacher Only";
const char* password = "b1sm1llah11562377";

#define USE_UI_CONTROL 0

#if USE_UI_CONTROL
#include <MD_UISwitch.h>
#endif

// Turn on debug statements to the serial output
#define DEBUG 0

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5
char Buf[250];
int a=0;

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Scrolling parameters
#if USE_UI_CONTROL
const uint8_t SPEED_IN = A5;
const uint8_t DIRECTION_SET = 8;  // change the effect
const uint8_t INVERT_SET = 9;     // change the invert

const uint8_t SPEED_DEADBAND = 15;
#endif // USE_UI_CONTROL

uint8_t scrollSpeed = 60;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 200; // in milliseconds

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "halo fadhlan" };
//char newMessage[BUF_SIZE] = { "Masukan Teks di Serial Monitor fadhlan ganteng" };
bool newMessageAvailable = true;

#if USE_UI_CONTROL

MD_UISwitch_Digital uiDirection(DIRECTION_SET);
MD_UISwitch_Digital uiInvert(INVERT_SET);

void doUI(void)
{
  // set the speed if it has changed
  {
    int16_t speed = map(analogRead(SPEED_IN), 0, 1023, 10, 100);

    if ((speed >= ((int16_t)P.getSpeed() + SPEED_DEADBAND)) ||
      (speed <= ((int16_t)P.getSpeed() - SPEED_DEADBAND)))
    {
      P.setSpeed(speed);
      scrollSpeed = speed;
      PRINT("\nChanged speed to ", P.getSpeed());
    }
  }

  if (uiDirection.read() == MD_UISwitch::KEY_PRESS) // SCROLL DIRECTION
  {
    PRINTS("\nChanging scroll direction");
    scrollEffect = (scrollEffect == PA_SCROLL_LEFT ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayClear();
    P.displayReset();
  }

  if (uiInvert.read() == MD_UISwitch::KEY_PRESS)  // INVERT MODE
  {
    PRINTS("\nChanging invert mode");
    P.setInvert(!P.getInvert());
  }
}
#endif // USE_UI_CONTROL


void setup() {
Serial.begin(115200);
WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
WiFi.begin(ssid, password);

Serial.println("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED){
delay(1000);
Serial.print(".");
}
#if USE_UI_CONTROL
  uiDirection.begin();
  uiInvert.begin();
  pinMode(SPEED_IN, INPUT);

  doUI();
#endif // USE_UI_CONTROL

  P.begin();
  //P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

}

void loop() {
  if (P.displayAnimate())
  {
    P.displayText(Buf, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);  
  }
HTTPClient http;

http.begin("https://api.banghasan.com/sholat/format/json/jadwal/kota/700/tanggal/2022-07-25");
//http.addHeader("Content-Type", "application/json"); //Isso é para mandar dados
int httpCode = http.GET();   //Isso é para get request

//if (httpCode > 0){
//  String payload = http.getString();          //ISSO É PARA GET REQUEST
//  Serial.println(httpCode);
//  Serial.println(payload);
//}
if(httpCode == 200 && a==0)
  {
    String payload = http.getString();          //ISSO É PARA GET REQUEST
    Serial.println(httpCode);
//    Serial.println(payload);
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(9) + 204;
    DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(payload);
    //Serial.println(jsonBuffer.parseObject(payload));
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    } 
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(root["jadwal"]["data"]["ashar"].as<char*>());
    String newMessage2 = root["jadwal"]["data"]["dzuhur"].as<char*>();
    String newMessage3 = root["jadwal"]["data"]["ashar"].as<char*>();
    String newMessage4 = root["jadwal"]["data"]["maghrib"].as<char*>();
    String newMessage5 = root["jadwal"]["data"]["isya"].as<char*>();
    String newMessage1 = root["jadwal"]["data"]["subuh"].as<char*>();
    String message = "Subuh: " + newMessage1 + "  Dzuhur: " + newMessage2 + "  Ashar: " + newMessage3 + "  Maghrib: " + newMessage4 + "  Isya: " + newMessage5;
    message.toCharArray(Buf, 250);
    a=1;
//    Serial.println(root["time"].as<char*>());
//    Serial.println(root["data"][0].as<char*>());
//    Serial.println(root["data"][1].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
}
