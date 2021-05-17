/* 
 *  This is the Main Parking System. 
 *  This contains 1 IR Obstacle Sensor, 1 Ultrasonic Sensor, 1 NodeMCU, and 1 5V supply for Ultrasonic Sensor. 
 *  We are using Arduino UNO's 5v PIN Supply for Ultrasonic Sensor.
 *  Ultrasonic Sensor is connected to the 0-D3-Echo, 2-D4-Trig pins.
 *  IR sensor is connected to 15-D5-Out pin.
 *  Sends parking-lot status as 0 or 1 to firebase's realtime database.
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>


#define WIFI_SSID "poco_f1"
#define WIFI_PASSWORD "zxcqwerty@7890"
#define FIREBASE_HOST "your-parkinspot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YXrqRHzpbEpwQLHaSPb25Y2vMSURviEuy1HcDnQh"


const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
const int obstaclePin = 15; //D5
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

// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to the WiFi SSID");
while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
}
Serial.println();
Serial.print("connected to the WiFi SSID:");
Serial.println(WiFi.SSID());
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//Firebase.setInt("parkstatus", 0);
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

if(hasObstacle==LOW){
  stand=true;
}
else{
  stand=false;
}

if(distance<=12){
  Serial.println("Parking Lot 1 Full");
  parkstatus=1;
}
else if(distance>12 && stand==true){
  Serial.println("Parking Lot 1 Full");
  parkstatus=1;
}
else if(distance>12 && stand==false){
  Serial.println("Parking Lot 1 Empty");
  parkstatus=0;
}

/*This part was intended to add to a more complete understanding of the user. 
 * States whether parking lot is empty or not! Instead of 0s and 1s.
//String parkmsg="";

//if(parkstatus==0){
//  parkmsg="Parking Lot Empty!";
//}
//else{
//  parkmsg="Parking Lot Full!";
//}
//Firebase.pushString("Slot1/Status Message",parkmsg);
*/

Firebase.setInt("Slot1/parkstatus",parkstatus); //Sends 0 or 1 to parkstatus under Slot1 directory in Realtime database.
delay(2000);
}
