/* demo Tone for Arduino Leonardo */

#define PIN_SP 10

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

}

void loop() {
  long t;
  //for (long t = 240 ; t < 4200 ; t += 16) {
  //  Serial.println(t); delay(500);
  //  tone(PIN_SP, t, 1000);
  //  delay(1500);
  //}
  delay(1000);

  // uxcell の 90dB 電磁ブザー
  t = 2688; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  t = 932; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  t = 554; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  //t = 688; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);

  // MuRaTa の圧電ブザー（PKM22EPPH4007-B0）
  t = 4096; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  t = 1520; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  //t = 3616; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  //t = 3200; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  //t = 1888; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);
  //t = 2400; Serial.println(t); delay(500); tone(PIN_SP, t, 1000); delay(1500);

  while(1){;}
}
