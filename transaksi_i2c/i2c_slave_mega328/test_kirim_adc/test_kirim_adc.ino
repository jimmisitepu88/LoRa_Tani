#include <ArduinoJson.h>
StaticJsonDocument<50> doc1;
StaticJsonDocument<50> doc2;
StaticJsonDocument<50> doc3;

#include <Wire.h>
unsigned long cur_time,old_time;

int dt_adc0;
int dt_adc1;
int dt_adc2;
int dt_adc3;
int dt_adc6;
int dt_adc7;

char char_payload1[50];
char char_payload2[50];
char char_payload3[50];

String str_payload;

byte count_request = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  cur_time = millis();
  if(cur_time -old_time >= 1000){
    baca_adc();
    //Serial.println("bismillaah");
    old_time = cur_time;
  }
  //Serial.println("bismillaah");
}

void requestEvent() {
    switch(count_request){
      case 0:
        count_request = 1;
        Wire.write(char_payload1);
        //Serial.println(char_payload1);
      break;
      case 1:
        count_request = 0;
        Wire.write(char_payload2);
        //Serial.println(char_payload2);
      break; 
    }
}


void baca_adc(){
  dt_adc0 = analogRead(A0);
  dt_adc1 = analogRead(A1);
  dt_adc2 = analogRead(A2);
  dt_adc3 = analogRead(A3);
  dt_adc6 = analogRead(A6);
  dt_adc7 = analogRead(A7);
  build_json();
}

void build_json(){
  String payload;
  doc1["s"] = 0;
  doc1["a"] = dt_adc0;
  doc1["b"] = dt_adc1;
  doc1["c"] = dt_adc2;
  serializeJson(doc1, payload);
  str_payload = payload;
  str_payload.toCharArray(char_payload1, str_payload.length()+1);
  str_payload = "";payload = "";

  
  doc2["s"] = 1;
  doc2["d"] = dt_adc3;
  doc2["e"] = dt_adc6;
  doc2["f"] = dt_adc7;

  serializeJson(doc2, payload);
  str_payload = payload;
  str_payload.toCharArray(char_payload2, str_payload.length()+1);
  str_payload = "";payload = "";
  
  //Serial.println(char_payload1);
  //Serial.println(char_payload2);
  doc1.clear();
  doc2.clear();
}
