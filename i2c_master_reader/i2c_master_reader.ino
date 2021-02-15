#include <Wire.h>
#define RXD2 17
#define TXD2 16
#define PKEY 5
#define RST 4

#define SerialAT Serial2
#define SerialMon Serial

unsigned long cur_time, old_time;
String dt_at;
String dt_mega;
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("LoRa_Tani"); 
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
  Serial.println("test at mulai");
  pinMode(RST, OUTPUT);
  pinMode(PKEY, OUTPUT);
  digitalWrite(PKEY, LOW);
  digitalWrite(RST, LOW); delay(5000);

  digitalWrite(RST, HIGH); delay(1000);
  digitalWrite(RST, LOW); delay(1000);
  digitalWrite(RST, HIGH); delay(1000);
  delay(1000);
  wRespon(15000);
}

void loop() {
  Serial.println("test AT");
  send_at("AT");
  send_at("AT+IPR?");
  send_at("AT+CPIN?");
  send_at("AT+CSQ");
  send_at("AT+CREG?");
  send_at("AT+CPSI?");

  Serial.println("request data");
  Wire.requestFrom(1, 20);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    dt_mega = Wire.readString(); // receive a byte as character
    
    //Serial.print(c);         // print the character
    //SerialBT.print(c);
  }
  Serial.println(dt_mega);
  SerialBT.print(dt_mega);
  dt_mega = "";
  Serial.println();
  delay(1000);
}


void send_at(char *_command) {
  SerialAT.println(_command);
  wRespon(1000);
}

void wRespon(long waktu) {
  cur_time = millis();
  old_time = cur_time;
  while (cur_time - old_time < waktu ) {
    cur_time = millis();
    while (SerialAT.available() > 0) {
      dt_at = SerialAT.readString();
      Serial.println(dt_at);
      SerialBT.println(dt_at);
      dt_at = "";
    }

  }
}
