// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
unsigned long cur_time,old_time;

void setup() {
  Serial.begin(115200);
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  cur_time = millis();
  if(cur_time -old_time >= 1000){

    Serial.println("bismillaah");
    old_time = cur_time;
  }
  //Serial.println("bismillaah");
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write("i am from atmega328 "); // respond with message of 6 bytes
  // as expected by master
}
