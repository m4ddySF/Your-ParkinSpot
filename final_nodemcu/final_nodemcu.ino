/* 
 *  This is the Main Parking System. 
 *  This contains 1 IR Obstacle Sensor, 1 Ultrasonic Sensor, 1 NodeMCU, and 1 5V supply for Ultrasonic Sensor. 
 *  We are using Arduino UNO's 5v PIN Supply for Ultrasonic Sensor.
 *  Ultrasonic Sensor is connected to the 0-D3-Echo, 2-D4-Trig pins.
 *  IR sensor is connected to 15-D5-Out pin.
 *  Sends parking-lot status as 0 or 1 to firebase's realtime database.
*/
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define trigPin D1
#define echoPin D2
#define obstaclePin D0
#define RST_PIN D3     // Configurable, see typical pin layout above
#define SS_PIN D4     // Configurable, see typical pin layout above

#define WIFI_SSID "poco_f1"
#define WIFI_PASSWORD "zxcqwerty@7890"
#define FIREBASE_HOST "your-parkinspot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YXrqRHzpbEpwQLHaSPb25Y2vMSURviEuy1HcDnQh"


MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

long duration;
int distance;
int hasObstacle=HIGH;
int parkstatus=0;
bool stand;






void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(obstaclePin, INPUT);
Serial.begin(9600); //Serial Communication
SPI.begin(); // Init SPI bus
rfid.PCD_Init(); // Init MFRC522

// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to ");
Serial.print(WiFi.SSID());
while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
}
Serial.println();
Serial.print("connected to the WiFi SSID:");
Serial.println(WiFi.SSID());
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}




void loop() {
/*
* For Utlrasonic sensor distance measurement
*/

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
//Serial.println(hasObstacle);

if(hasObstacle==LOW){
  stand=true;
}
else{
  stand=false;
}

if(distance<=6){
  Serial.println("Parking Lot 1 Full");
  parkstatus=1;
}
else if(distance>6 && stand==true){
  Serial.println("Parking Lot 1 Full");
  parkstatus=1;
}
else if(distance>6 && stand==false){
  Serial.println("Parking Lot 1 Empty");
  parkstatus=0;
}

Firebase.setInt("Slot1/parkstatus",parkstatus); //Sends 0 or 1 to parkstatus under Slot1 directory in Realtime database.


//Serial.println("Show your card:");
if ( ! rfid.PICC_IsNewCardPresent()){
  return;
}
if (rfid.PICC_ReadCardSerial()) {
  for (byte i = 0; i < 4; i++) {
    tag += rfid.uid.uidByte[i];
  }
  Serial.println(tag);
  //long idnum=tag.toInt();
  Firebase.setString("Slot1/userID",tag);
  //idnum=0;
  tag = "";
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
delay(2000);
}
