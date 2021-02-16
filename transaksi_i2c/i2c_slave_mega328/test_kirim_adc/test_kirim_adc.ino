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

float dt_sensor1;
float dt_sensor2;
float dt_sensor3;

char char_payload1[50];
char char_payload2[50];
char char_payload3[50];

String str_payload;

byte count_request = 0;

int in1, in2, in3, in4, in5, in6;

void setup() {
  Serial.begin(115200);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP); 
   
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  cur_time = millis();
  if(cur_time -old_time >= 250){
    baca_adc();
    baca_sensor();
    build_json();
    
    //Serial.println("bismillaah");
    old_time = cur_time;
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {

    switch(count_request){
      case 0:
        count_request = 1;
        Wire.write(char_payload1);
        //Serial.println("kirim 0");
      break;
      case 1:
        count_request = 2;
        Wire.write(char_payload2);
        //Serial.println("kirim 1");
      break; 
      case 2:
        count_request = 0;
        Wire.write(char_payload3);
        //Serial.println("kirim 2");
      break; 
      
    }
    
   /*if(count_request == 0){
    Wire.print(char_payload1); 
    count_request = 1;
  }
  if(count_request == 1){
    Wire.print(char_payload2); 
    count_request = 0;
  }
  
  */
  // respond with message of 6 bytes
  // as expected by master
}


void baca_adc(){
  dt_adc0 = analogRead(A0);
  dt_adc1 = analogRead(A1);
  dt_adc2 = analogRead(A2);
  dt_adc3 = analogRead(A3);
  dt_adc6 = analogRead(A6);
  dt_adc7 = analogRead(A7);
}

void baca_sensor(){
  in1 = digitalRead(2);
  in2 = digitalRead(3);
  in3 = digitalRead(4);
  in4 = digitalRead(5);
  in5 = digitalRead(6);
  in6 = digitalRead(7);
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

  doc3["s"] = 2;
  doc3["g"] = in1;
  doc3["h"] = in2;
  doc3["i"] = in3;
  doc3["j"] = in4;
  
  serializeJson(doc3, payload);
  str_payload = payload;
  str_payload.toCharArray(char_payload3, str_payload.length()+1);
  str_payload = "";payload = "";
  
  Serial.println(char_payload1);
  Serial.println(char_payload2);
  Serial.println(char_payload3);
  
  doc1.clear();
  doc2.clear();
  doc3.clear();
}
