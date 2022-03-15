/*
 * 
 * 
 */

// Wifi and IoT includes and global variables. 

#include "IoT_WiFi.h"
#include "IoT_HTTP.h" //some helper functions for doing HTTP calls

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

IOT_WiFi IOT_WiFi; //initialize the IOT instance

Measurement measurement_now; //this is a struct (defined in IoT_HTTP.h) that contains the fields to be sent to the DB

long measurement_delay_ms = 60000; //1 minute between measurements. adjust as necessary. 

// libraries and global variables for the sensors
#include <AHT10.h>
AHT10 AHT10_1(AHT10_ADDRESS_0X38);

#define ldr_pin 36

// indicator lights, etc
#define good_status_pin 4 
#define bad_status_pin 2

void setup() {
  pinMode(good_status_pin,OUTPUT);
  pinMode(bad_status_pin,OUTPUT);
  
  Serial.begin(115200);

  //indicator lights ot indicate sensor initialize correct
  // a green LED will blink several times once the sensors initialize correctly, red LED on continuously if not
  digitalWrite(good_status_pin,LOW);
  digitalWrite(bad_status_pin,HIGH);
  while (AHT10_1.begin() != true);
  digitalWrite(bad_status_pin,LOW);
  for (int i=0;i<4;i++){
    digitalWrite(good_status_pin,HIGH);
    delay(150);
    digitalWrite(good_status_pin,LOW);
    delay(150);
  }

  /*** INIT WIFI AND SHOW STATUS **/
  digitalWrite(bad_status_pin,HIGH);
  //connect to WIFI and 
  IOT_WiFi.initWiFi(WIFI_SSID,WIFI_PASSWORD);
  digitalWrite(bad_status_pin,LOW);
  digitalWrite(good_status_pin,HIGH);

  /**SET UP CONSTANT PARTS OF MEASUREMENT SENDS. Compare this to other examples to see different applications that have more or fewer constants  **/ 
  setMeasurement_DeviceID(measurement_now,"CIJEJAY0002"); //set this to somethign unique that you will be able to identify later.
  setMeasurement_MacAddress(measurement_now,IOT_WiFi.getMACAddress());//MAC Address should be unique harware identifier for the board. Again helpful in identification 
  setMeasurement_Latitude(measurement_now, 40.65860385788833); //A location in Brooklyn, from Google Maps
  setMeasurement_Longitude(measurement_now, -73.99407541369197); //A location in Brooklyn, from Google Maps
}

void loop() {

  long start_time = millis(); //time at beginning of loop
  
if (IOT_WiFi.isConnected()){

  setMeasurement_Timestamp(measurement_now,IOT_WiFi.getTime());

  float temp=AHT10_1.readTemperature();
  float rel_humid=AHT10_1.readHumidity();
  float ldr_read = (float)analogRead(ldr_pin)*3.3/4095.;

  //Serial printing for debugging only. Comment out after making sure sensors are sending correct data before connecting to internet
  //Serial.print(F("Temperature: ")); Serial.print(AHT10_1.readTemperature()); Serial.println(F(" +-0.3C")); //by default "AHT10_FORCE_READ_DATA"
  //  Serial.print(F("Humidity...: ")); Serial.print(AHT10_1.readHumidity());    Serial.println(F(" +-2%"));   //by default "AHT10_FORCE_READ_DATA"
  //
  //Serial.print("Analog ");Serial.println(analogRead(ldr_pin));

  //send temperature data
  setMeasurement_MeasurementUnits(measurement_now,"C"); 
  setMeasurement_MeasurementType(measurement_now,"Temperature");//again, this is for the user. If measure Temperature and battery, then need ot be able to differentiate
  setMeasurement_MeasurementNote(measurement_now,"Room 1"); //optional: include if want more info for later
  measurement_now.value=temp;

  postData2Server("https://cije-basic-iot-server.glitch.me/measurements/",measurement_now);

  Serial.print("value: ");Serial.print(measurement_now.value);Serial.print(" latitude: "); Serial.print(measurement_now.latitude); Serial.print(" longitude: "); Serial.print(measurement_now.longitude); 
  //send humidity data
  setMeasurement_MeasurementUnits(measurement_now,"%"); 
  setMeasurement_MeasurementType(measurement_now,"Relative Humidity");//again, this is for the user. If measure Temperature and battery, then need ot be able to differentiate
  setMeasurement_MeasurementNote(measurement_now,"Room 1"); //optional: include if want more info for later
  measurement_now.value=rel_humid;

  postData2Server("https://cije-basic-iot-server.glitch.me/measurements/",measurement_now);

  //send ldr value
  setMeasurement_MeasurementUnits(measurement_now,"V"); 
  setMeasurement_MeasurementType(measurement_now,"Light");//again, this is for the user. If measure Temperature and battery, then need ot be able to differentiate
  setMeasurement_MeasurementNote(measurement_now,"Room 1, uncalibrated measure"); //optional: include if want more info for later
  measurement_now.value=ldr_read;


  postData2Server("https://cije-basic-iot-server.glitch.me/measurements/",measurement_now);

  while((millis()-start_time)<measurement_delay_ms); //delay until next measurement to be made.  
  
} else {
  digitalWrite(bad_status_pin,HIGH);
  digitalWrite(good_status_pin,LOW);

//TODO: this needs to be better. this could hang the whole damn thing
  IOT_WiFi.initWiFi(WIFI_SSID,WIFI_PASSWORD);

  digitalWrite(bad_status_pin,LOW);
  digitalWrite(good_status_pin,HIGH);
}

} 
