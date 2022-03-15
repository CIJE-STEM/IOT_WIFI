/*
Soem stuff for WiFi
*/

#include "IoT_WiFi.h"

#include <Preferences.h>
#include <WiFi.h>
#include <time.h>

void IOT_WiFi::initWiFi(const char* WIFI_SSID, const char* WIFI_PASSWORD){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

    //Attempt to connect: possible communicate with home screen
    //TODO: need to do this where it cuts out if it cannot 
    while (!isConnected()) {
    Serial.print('.');
    //TODO: Pritn to SCREEN
    delay(1000);
    }
    Serial.println(WiFi.localIP());
    //TODO: HERE IF THE UPLOAD DOESN"T WORK ETC THEN GO INTO THE UPDATE CREDENTIALS STUFF
    // updateCredentials();
    this->_mac_addr = WiFi.macAddress();
    // this->_local_ip = WiFi.localIP();
    
    Serial.println("GETTTING TIME INFO");
    while(this->getTimeTm("UTC")->tm_year==1970);
    Serial.println(getTime()); //initialize with ntp server
}

//ESP32 (possibly ESP8266) Only currently
void IOT_WiFi::initWiFi(){

  //attempt to get WiFi credentials stored in pref_mem (Preferences.h nvt system)
  Preferences pref_mem;
  pref_mem.begin("credentials",true); //true=read_only
  String ssid = pref_mem.getString("ssid",""); //return "" if nothing
  String password = pref_mem.getString("password","");
  pref_mem.end(); 

  if (ssid=="" || password==""){
    //DEBUG ONLY
    Serial.println("SSID/PASS NULL");
    updateCredentials();

  } else {

    //TRY TO CONNECT
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),password.c_str());

    //Attempt to connect: possible communicate with home screen
    //TODO: need to do this where it cuts out if it cannot 
    while (!isConnected()) {
    Serial.print('.');
    //TODO: Pritn to SCREEN
    delay(1000);
    }
    Serial.println(WiFi.localIP());
    //TODO: HERE IF THE UPLOAD DOESN"T WORK ETC THEN GO INTO THE UPDATE CREDENTIALS STUFF
    // updateCredentials();
    this->_mac_addr = WiFi.macAddress();
    // this->_local_ip = WiFi.localIP();

    Serial.println("GETTTING TIME INFO");
    while(this->getTimeTm("UTC")->tm_year==1970);
    Serial.println(getTime()); //initialize with ntp server
  }
  
}

void IOT_WiFi::updateCredentials(){

  //HERe will change the mode of the EPS32 board, create a POST to change the credentials, make a list of the SSIDs available, etc
  // But for now just a Serial

  //currenlty just using a preferences script to add these. will include more later

  String ssid="DIFFERENT SSID";
  String password="DIFFERNET PASSWORD";
}

bool IOT_WiFi::isConnected(){

  return WiFi.status() == WL_CONNECTED;
}

const char* IOT_WiFi::getMACAddress(){
    return _mac_addr.c_str();
  }



bool IOT_WiFi::setTzOffsets(const long gmtOffset_sec, const long daylightOffset_sec){
    //TODO: add set checks
    
    this->gmtOffset_sec = gmtOffset_sec;
    this->daylightOffset_sec = daylightOffset_sec;

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    //reset the NTP check counter
    time_t now;
    time(&now);
    *ptr_ntp_check_zero=now;

    return true;   
}

struct tm* IOT_WiFi::getTimeTm(const char* tz){

  //get current system time from epoch
     time_t now;
     time(&now);
  //TODO: check internet connnection and skip if not
     if (now-*ptr_ntp_check_zero >= ntp_check_delta_sec) {
       configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // This is an ESP32 arduino function that does the SNTP stuff.
      *ptr_ntp_check_zero=now;
    }

     struct tm *time_tm;
      //TODO: can do fuller comparison later
     if (!strcmp(tz,"local")){
       time_tm = localtime(&now);
     } else {
       time_tm = gmtime(&now);
     }

     return time_tm;
}

void IOT_WiFi::getTimeDetails(const char* tz){

  struct tm *time_tm = getTimeTm(tz);
  
  td_ptr->year = time_tm->tm_year + 1900; 
  td_ptr->month = time_tm->tm_mon+1;
  td_ptr->day = time_tm->tm_mday;
  td_ptr->hour = time_tm->tm_hour;
  td_ptr->minute = time_tm->tm_min;
  td_ptr->sec = time_tm->tm_sec;
}


const char* IOT_WiFi::getTime(){

   strftime(timestamp,timestamp_sz,time_fmt,this->getTimeTm("UTC"));

   return timestamp;

 }

const char* IOT_WiFi::getLocalTime(){

   strftime(timestamp,timestamp_sz,time_fmt,this->getTimeTm("local"));

   return timestamp;
    
}

bool IOT_WiFi::setNTPCheckPeriod(long time_period_sec){//de-const,set,re-const
}
bool IOT_WiFi::setTimeFormat(const char* new_time_fmt){
  //don't forget to un-constt the format, set it, and  re-const it
}
