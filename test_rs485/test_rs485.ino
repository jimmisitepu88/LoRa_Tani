#include <ModbusMaster.h>
ModbusMaster node;
static uint8_t pzemSlaveAddr = 1;

#define RXD1 33
#define TXD1 25

unsigned cur_time, old_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  node.begin(pzemSlaveAddr, Serial1);
}

void loop() {
  // put your main code here, to run repeatedly:
  cur_time = millis();
  if (cur_time - old_time >= 1000) {
    baca_rs485(15);
    old_time = cur_time;
  }
}

void baca_rs485(uint8_t wkt) {
  uint8_t result;
  uint32_t tempdouble;
  result = node.readHoldingRegisters(171, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.println("succes1");
    
  }
  else {
    Serial.println("failed1");
  }
  Serial.println();
  Serial.println("-----------------------------------------");
}
