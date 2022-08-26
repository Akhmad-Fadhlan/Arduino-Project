#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

int trig_pin = 27;
int echo_pin = 26;
int relay_pin = 25;
long echotime; 
float distance;

// Replace with your network credentials
const char* ssid = "realme C12";
const char* password = "akhmad90";

// Initialize Telegram BOT
#define BOTtoken "1637026900:AAEnlhj5OpWb96XcAjTeJjFRQ2pPLGdCxXI"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1465123828"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


//const int motionSensor = 14; // PIR Motion Sensor
//bool motionDetected = false;

// Indicates when motion is detected
//void ICACHE_RAM_ATTR detectsMovement() {
//  //Serial.println("MOTION DETECTED!!!");
//  motionDetected = true;
//}

void setup() {
  Serial.begin(115200);
  pinMode(trig_pin, OUTPUT); 
  pinMode(echo_pin, INPUT);
  pinMode(relay_pin, OUTPUT); 
  digitalWrite(trig_pin, LOW);  

  // PIR Motion Sensor mode INPUT_PULLUP
  //pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  //attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  client.setInsecure();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
 // bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  echotime= pulseIn(echo_pin, HIGH);
  distance= 0.0001*((float)echotime*340.0)/2.0;
    if(distance > 6) 
      {bot.sendMessage(CHAT_ID, "Tingkat Kepenuhan 0 sampai 33.33%"); Serial.println("Tingkat Kepenuhan 0 sampai 33.33%");digitalWrite(relay_pin,HIGH);}
    else if(distance <=6 && distance >=4 )
      {bot.sendMessage(CHAT_ID, "Tingkat Kepenuhan 33.33% sampai 66.67%"); Serial.println("Tingkat Kepenuhan 33.33% sampai 66.67%");digitalWrite(relay_pin,HIGH);}
    else {bot.sendMessage(CHAT_ID, "Tingkat Kepenuhan 66.67% sampai 100%");Serial.println("Tingkat Kepenuhan 66.67% sampai 100%"); digitalWrite(relay_pin,LOW);}
    delay(1000);
}
