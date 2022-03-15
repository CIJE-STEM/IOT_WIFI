/*
WiFi communications class for IoT commm
*/

#ifndef __IOT_WIFI_H__
#define __IOT_WIFI_H__

#include <time.h>

#include "Arduino.h"


class IOT_WiFi {

public: 
  
  //rule of 5
  //there should only be one instance of hte IOT_WiFi class so copy and move disabled
IOT_WiFi()=default; //will throw at compile if do not include in cpp file without default keyword
~IOT_WiFi()=default;
IOT_WiFi(const IOT_WiFi& other) = delete;
IOT_WiFi& operator=(const IOT_WiFi& other) = delete;
IOT_WiFi(IOT_WiFi&& other) = delete;
IOT_WiFi& operator=(IOT_WiFi&& other) = delete;


  //connection and initialization
  void initWiFi();
  void initWiFi(const char* WIFI_SSID, const char* WIFI_PASSWORD);
  bool isConnected();

  //time and time server
  struct Td {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t sec;
  };
  struct Td td{}; //Init td structure. Doing it this way to maintain dot(.) access for student familiarity.  
  struct Td* td_ptr=&td;
  
  struct tm* getTimeTm(const char*);
  const char* getTime();
  const char* getLocalTime();
  void getTimeDetails(const char*);
  
  bool setNTPCheckPeriod(long time_period_sec);
  int getNTPCheckPeriod();
  bool setTimeFormat(const char* new_time_fmt);
  bool setTzOffsets(const long gmtOffset_sec, const long daylightOffset_sec);

  const char* getMACAddress();
  
private:

  String _mac_addr;
  // String _local_ip;

  /**************time server related **********/
  const char* ntpServer = "pool.ntp.org";
  long  gmtOffset_sec = 0; //timezone is set to UTC by default. IF want local time, use setTzOffsets(). 
  int   daylightOffset_sec = 0;
  //string format in MySQL format
  const char* time_fmt = "%Y-%m-%d %H:%M:%S.000000";
  const int timestamp_sz = 27; //size of the output !!! this is not the same size as the format string of %Y, etc. dont forget the /0 on the end
  char* timestamp= (char*) malloc(timestamp_sz*sizeof(char));
  
  //variables for rechecking the NTP server: honestly a timer interrupt is probably best here...
  const long ntp_check_delta_sec = 600; //number of seconds before realigning clock with NTP server
  time_t ntp_check_zero = -2*ntp_check_delta_sec; //last time the server was checked, initialized to guarantee
  time_t* ptr_ntp_check_zero = &ntp_check_zero; 


  void updateCredentials();

};






















#endif
