/* demo ATTiny85 ADC for V-Batt */
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


// ソフトウェアシリアル
#include <SoftwareSerial.h>
#define RX_PIN 1   // USB-シリアル変換モジュールのTXに
#define TX_PIN 0   // USB-シリアル変換モジュールのRXに
SoftwareSerial mySerial(RX_PIN, TX_PIN);    // RX,TXの割り当て

// ピンアサイン
#define A_VBATT A3

void setup() {
  mySerial.begin(9600); 
  mySerial.println("ATtiny85 start");

  //analogReference(INTERNAL);
}

void loop() {
  //mySerial.println(1.1*analogRead(3)/1024.0);
  //mySerial.println(analogRead(readPB3()));

  ADMUX = 0x83; // Vref = 1.1V, ADC=PB3
  sbi(ADCSRA, ADSC); // start ADC
  while (bit_is_set(ADCSRA, ADSC)); // wait until ADC stop
  mySerial.println(ADC);

  delay(500);
}