#include "IoT_HTTP.h"



bool setMeasurement_Timestamp(Measurement& measure,const char* timestamp){
  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }
   strcpy(measure.timestamp,timestamp);
   return true;
}

bool setMeasurement_MeasurementUnits(Measurement& measure, char* measure_units){

  //TODO: some checks here
  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }
  
  measure.units = measure_units;
  return true;
}

bool setMeasurement_DeviceID(Measurement& measure, char* device_id){
  //TODO: some checks here
  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }

  measure.device_id = device_id;
  return true; 
}

bool setMeasurement_MeasurementType(Measurement& measure,char* measure_type){

  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }

  measure.measurement_type = measure_type;
  return true; 
}

bool setMeasurement_MeasurementNote(Measurement& measure,char* note){
  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }

  measure.note = note;
  return true; 
}

bool setMeasurement_MacAddress(Measurement& measure,const char* mac_address){

  if (false) { //placeholder for checking, does nothing right now.
    return false;
  }
  strcpy(measure.mac_address,mac_address);
//  measure.mac_address = mac_address;
  return true; 
}

bool setMeasurement_Latitude(Measurement& measure, double latitude){

  if (false) {//placeholder for data checking
    return false;
    }

  measure.latitude = latitude;
  return true;
}

bool setMeasurement_Longitude(Measurement& measure, double longitude){
  if (false) {//placeholder for data checking
    return false;
    }

  measure.longitude = longitude;
  return true;
}


bool postData2Server(const char* route, Measurement& measure){

  size_t json_size=1000;
  char body_json[json_size];
  snprintf(body_json,json_size,"{ \"new_measure\": {\"timestamp\": \"%s\", \"measurement_value\":\"%f\", \"measurement_units\":\"%s\",\"measurement_type\":\"%s\",\
  \"mac_addr\":\"%s\",\"device_id\":\"%s\",\"note\":\"%s\" }}",measure.timestamp, measure.value, measure.units, measure.measurement_type,\
  measure.mac_address,measure.device_id,measure.note);
  
  
  Serial.print("JSON OUT ");Serial.println(body_json);

    //HTTP POST Headers etc.  using HTTP Client
  HTTPClient http;

  http.begin(route);
  http.addHeader("Content-Type","application/json");
//  http.addHeader("Content-Length",

  
  int httpResponseCode=http.POST(body_json);

  if (httpResponseCode>0){
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
    
 
  return true;
}


//
//  //Build the JSON for sending
//  //TODO: do a more accurate size count
  
//

//

//
//  return true; //if successful return true
//}
//
