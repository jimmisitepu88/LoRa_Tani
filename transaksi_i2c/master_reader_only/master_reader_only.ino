#include <ArduinoJson.h>
#include <Wire.h>

String str_dt;
char chr_i2c[33];
int jml_i2c;

int adc0, adc1, adc2, adc3, adc6, adc7;
int count_request = 0;
int sts = 0;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  request_i2c();
  delay(50);
}

void request_i2c() {
  //Serial.println("request data");
  Wire.requestFrom(1, 32);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    chr_i2c[jml_i2c] = Wire.read(); // receive a byte as character
    if (chr_i2c[jml_i2c] == '}')break;
    jml_i2c++;

  }

  for (int i = 0; i < 32; i++) {
    str_dt += String(chr_i2c[i]);
  }
  DynamicJsonDocument doc(100);
  deserializeJson(doc, str_dt);
  //Serial.println(str_dt);
  sts = doc["s"];
  //Serial.println(sts);
  switch (sts) {
    case 0:
      adc0 = doc["a"];
      adc1 = doc["b"];
      adc2 = doc["c"];
      Serial.print("adc0: ");Serial.print(adc0); Serial.print(",");
      Serial.print("adc1: ");Serial.print(adc1); Serial.print(",");
      Serial.print("adc2: ");Serial.print(adc2); Serial.println();
      break;
    case 1:
      adc3 = doc["d"];
      adc6 = doc["e"];
      adc7 = doc["f"];
      Serial.print("adc3: ");Serial.print(adc3); Serial.print(",");
      Serial.print("adc6: ");Serial.print(adc6); Serial.print(",");
      Serial.print("adc7: ");Serial.print(adc7); Serial.println();
      break;
  }

  str_dt = ""; jml_i2c = 0; doc.clear();

}
