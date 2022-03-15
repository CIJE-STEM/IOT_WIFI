/* POST data to server. Here specifically the measurememtn server set up at Glitch for ease of implementation
 *  
 *  NOTE: This is a good way to prototype, check, and run low throughput data. Eventually, for longer times and larger data sets, a move to a dedicated server is suggested. 
 */

#include "IoT_WiFi.h"
#include "IoT_HTTP.h" //some helper functions for doing HTTP calls

#define ANALOG_PIN 34

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

const char* data_server = "https://cije-basic-iot-server.glitch.me/measurements/";

IOT_WiFi IOT_WiFi; //initialize the IOT instance

Measurement measurement_now; //this is a struct (defined in IoT_HTTP.h) that contains the fields to be sent to the DB

void setup() {
 Serial.begin(115200);

 //You can use initWiFi() that assumes that SSID and PASSWORD have been stored on the board already
 //Use Utilities/preferences_mem_save/preferences_mem_save.ino in the library to do that.
// If you have that stored on the board you can uncomment the line below, and comment out the IOT_WiFi.initWiFi(WIFI_SSID, WIFI_PASSWORD)
// IOT_WiFi.initWiFi();
  IOT_WiFi.initWiFi(WIFI_SSID, WIFI_PASSWORD);
  delay(1000);
  
  Serial.print("TIMESTAMP: ");Serial.println(IOT_WiFi.getTime()); //returns UTC time

  //set measurement_now parameters that wont be changing much, if at all. We can set directly with dot notation. These fucntions are just for checking and clarity
  //Thsi example in measuring the voltage going in to the ADC, for example a battery voltage. 
  setMeasurement_MeasurementUnits(measurement_now,"V"); //for volts. if I was measuring say milivolts would use "mV", etc
  setMeasurement_DeviceID(measurement_now,"CIJEJAY0001"); //set this to somethign unique that you will be able to identify later.
  setMeasurement_MeasurementType(measurement_now,"LDR");//again, this is for the user. If measure an LDR and battery, then need ot be able to differentiate
  setMeasurement_MacAddress(measurement_now,IOT_WiFi.getMACAddress());//MAC Address should be unique harware identifier for the board. Again helpful in identification 

  //optional data
  setMeasurement_Latitude(measurement_now, 40.70662827476518); //CIJE main office, from Google Maps
  setMeasurement_Longitude(measurement_now,-74.01307855310544); //CIJE main office, from Google Maps
  setMeasurement_MeasurementNote(measurement_now,"Testing for IOT Server"); //optional: include if want more info for later
  
}

void loop() {

  //measure a sensor. Here an analog read.
  int val_now = analogRead(ANALOG_PIN);
  //convert to a voltage assuming 3.3V is my peak, not necessarily a good assumption, but 
  float volts_now = (float)val_now*(3.3/4095.); //important to use the decimal points to indicate decimal division
  
  Serial.print("Analog, Volts: ");Serial.print(val_now);Serial.print(" ");Serial.println(volts_now);

  /*
   * PREPARE MEASUREMENTS FOR POSTING TO DB. 
   * These change often, we can set directly using dot notatiaon or use helper functions
   */
  //measurement_now.timestamp=IOT_WiFi.getTime(); !! Doesn't work with const char* type, 
  setMeasurement_Timestamp(measurement_now,IOT_WiFi.getTime());
  measurement_now.value=volts_now;
  
  //post the data
  postData2Server(data_server,measurement_now);
  delay(3000);
}
