/* Example of the time functionality of the IoT_WiFi wrapper class.
 *  Originally designed for the CIJE-Tech Advanced Engineering Course. 
 *  
 */


#include "IoT_WiFi.h"


/* TIME TESTING */

 IOT_WiFi IOT_WiFi;
 
 const char* WIFI_SSID = "YOUR_SSID";
 const char* WIFI_PASSWORD = "YOUR_PASSWORD";
 
 void setup(){

 Serial.begin(115200);
  
  IOT_WiFi.initWiFi(WIFI_SSID,WIFI_PASSWORD);

 // set the TZ offsets in seconds. E.G. East Coast USA Standard is -5:00 hours. In seconds htis is 5*3600 = -18000 (there are 3600 seconds in an hour)
 // Function prototype: setTzOffsets(const long gmtOffset_sec, const long daylightOffset_sec);
  IOT_WiFi.setTzOffsets(-18000, 3600);
 }
 
void loop(){

  Serial.print("UTC TIMESTAMP: "); Serial.println(IOT_WiFi.getTime());
  Serial.print("LOCAL TIMESTAMP: "); Serial.println(IOT_WiFi.getLocalTime());
  
  IOT_WiFi.getTimeDetails("local");//"local" for local set time or "UTC" for UTC time
  Serial.print("YEAR: ");Serial.println(IOT_WiFi.td.year);//current year
  Serial.print("MONTH: ");Serial.println(IOT_WiFi.td.month);//1-12
  Serial.print("DAY: ");Serial.println(IOT_WiFi.td.day);//1-31
  Serial.print("HOUR: ");Serial.println(IOT_WiFi.td.hour);//0-23
  Serial.print("MINUTE: ");Serial.println(IOT_WiFi.td.minute);//0-59
  Serial.print("SECOND: ");Serial.println(IOT_WiFi.td.sec);//0-59 inclusive of leap seconds. 
  Serial.println();
  delay(3000);
 
}
