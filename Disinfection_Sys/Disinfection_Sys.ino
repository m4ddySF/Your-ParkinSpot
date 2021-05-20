/*
 * Smart Disinfectant Dispersal System
 * Works using Arduino UNO/NANO, a Long Range IR Sensor, a Relay Module and a Motor Pump.
 * 
*/

int relayPin=12;
int inputPin=11;
int irState=LOW;
int val=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val=digitalRead(inputPin);
  if(val==HIGH){
    digitalWrite(relayPin, HIGH);
    if(irState==HIGH){
      Serial.println("Motion No More");
      delay(2000);
      irState=LOW;
    }
  }
  else if(val==LOW){
    digitalWrite(relayPin, LOW);
    if(irState==LOW){
      Serial.println("Motion Detected");
      delay(4000);
      irState=LOW;
    }
  }
}
