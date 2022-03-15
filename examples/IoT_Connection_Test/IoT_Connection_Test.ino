/* Connection Test Sketch with IoT_MCU library 

NOTE: 
//Two ways to connect in the library. Here going to use direct compile of SSID and PASSWORD.
//This is not the most opitimal since if you move to a different network, you must recompile and reload the entire code.
//Instead you can save the SSID and Password to ROM adn just change this. Use Utilities/preferences_mem_save/preferences_mem_save.ino in the library to do that.

*/

#include "IoT_WiFi.h"

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

IOT_WiFi IOT_WiFi; //initialize the IOT instance

void setup() {
  Serial.begin(115200);

  IOT_WiFi.initWiFi(WIFI_SSID, WIFI_PASSWORD);
  delay(1000);
}

void loop() {
  //use library getTime function to get UTC. 
  Serial.print("TIMESTAMP: ");Serial.println(IOT_WiFi.getTime());
  delay(1000);
}
