/*
 * WiFi_HTTP: A wrapper class for HTTP calls for sending measurements to the database. 
 * This is intended to further ease-of-use for HTTP calls while users become familiar with the system
 * It is intenionally limited, and once a user wants to 'break out', the direct use of HTTPClient and other libs is encouraged
 * 
 */
#include <HTTPClient.h>
#include <string.h>
#include <stdio.h>

struct Measurement {
    
    char timestamp[40];
    float value;
    char* units; // a measurement is meaningless without units. if it is miliVolts, then "mV", etc. 
    char* measurement_type; //e.g BATTERY, to help user catalog where this is from
    char mac_address[20]; 
    char* ip_address;
    char* device_id;  // currently, this is self-identified. Use somehting that will be unique. 
    char* note; //OPTIONAL: any additional info that can be helpful in later analysis. 
    double latitude; //OPTIONAL location data
    double longitude; // OPTIONAL location data
};

//set functions. you can set directly from the struct instance but included to help clarify/simplify

bool setMeasurement_Timestamp(Measurement& measure,const char* timestamp);
bool setMeasurement_MeasurementUnits(Measurement& measure,char* measure_units);
bool setMeasurement_DeviceID(Measurement& measure,char* device_id);
bool setMeasurement_MeasurementType(Measurement& measure,char* measure_type);
bool setMeasurement_MeasurementNote(Measurement& measure,char* note);
bool setMeasurement_MacAddress(Measurement& measure,const char* mac_address);
bool setMeasurement_Latitude(Measurement& measure, double latitude);
bool setMeasurement_Longitude(Measurement& measure, double longitude);

//post measurement JSON data to a server. The route must be configured to consume a POST statement
bool postData2Server(const char* route, Measurement& measure);
