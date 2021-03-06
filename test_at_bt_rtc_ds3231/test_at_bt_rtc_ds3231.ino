#define RXD2 17
#define TXD2 16
#define PKEY 5
#define RST 4
unsigned long cur_time, old_time;
#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon
#define SerialAT Serial2


#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

bool hold = 0;

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
unsigned time_now, time_old;

void setup() {
  // put your setup code here, to run once:
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  //delay(1000);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  Serial.println("found RTC");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("test at mulai");
  pinMode(RST, OUTPUT);
  pinMode(PKEY, OUTPUT);
  digitalWrite(PKEY, LOW);
  digitalWrite(RST, HIGH); delay(1000);
  digitalWrite(RST, LOW); delay(1000);
  digitalWrite(RST, HIGH); delay(1000);
  //delay(1000);
  //wRespon(15000);
  /*SerialAT.println("AT");
    wRespon(1000);
    SerialAT.println("AT+IPREX?");
    wRespon(1000);
    SerialAT.println("AT+IPREX=115200");
    wRespon(1000);
    SerialAT.println("AT&W");
    wRespon(1000);
    SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  */

  SerialBT.begin("LoRa_Tani");
}

void loop() {
  // put your main code here, to run repeatedly:

  time_now = millis();
  if (time_now - time_old >= 1000) {
    DateTime now = rtc.now();
    SerialBT.print(now.year(), DEC);
    SerialBT.print('/');
    SerialBT.print(now.month(), DEC);
    SerialBT.print('/');
    SerialBT.print(now.day(), DEC);
    SerialBT.print(" (");
    SerialBT.print(daysOfTheWeek[now.dayOfTheWeek()]);
    SerialBT.print(") ");
    SerialBT.print(now.hour(), DEC);
    SerialBT.print(':');
    SerialBT.print(now.minute(), DEC);
    SerialBT.print(':');
    SerialBT.print(now.second(), DEC);
    SerialBT.println();
    time_old = time_now;
  }
  /*Serial.println("SEND AT");
    send_at("AT");
    send_at("AT+IPR?");
    send_at("AT+CPIN?");
    send_at("AT+CSQ");
    send_at("AT+CREG?");
    send_at("AT+CPSI?");
  */
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
      //Serial.print(SerialAT.read());
      SerialBT.write(SerialAT.read());
    }
  }
}
