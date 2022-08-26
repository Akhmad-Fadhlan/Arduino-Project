//sensor kelembaban
int sensorPin = analogRead(A0);
const byte powerPin = 8;
#define LIMIT 400
//relay
const int relay1 = 2;
int relayON = LOW;
int relayOFF = HIGH;

void setup() {
  //sensor kelembaban
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  Serial.begin(9600);
  //relay
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, relayOFF);

}

void loop() {
  //sensor kelembaban
  Serial.print("Nilai Kelembaban: ");
  Serial.println(bacaSensor());
  
  //relay
  if(bacaSensor>LIMIT){
    digitalWrite(relay1, relayOFF);
    Serial.println("POMPA HIDUP");
  }
  else(bacaSensor<LIMIT);{
    digitalWrite(relay1, relayON);
    Serial.println("POMPA MATI");
  }

}

int bacaSensor(){
  digitalWrite(powerPin, HIGH);
  delay(1000);
  int nilaiSensor = analogRead(sensorPin);
  digitalWrite(powerPin, LOW);
  return 1023 - nilaiSensor;
}
