/* demo ICCC(ATTiny85) テスト検知のコードデモ */

// ソフトウェアシリアル
#include <SoftwareSerial.h>
#define RX_PIN 0   // USB-シリアル変換モジュールのTXへつなぐ
#define TX_PIN 1   // USB-シリアル変換モジュールのRXへつなぐ
SoftwareSerial mySerial(RX_PIN, TX_PIN);    // RX,TXの割り当て

// ピンアサイン
#define BZ_PIN  0   // ブザー出力
#define EN_PIN  2   // 電源許可
#define TEST_PIN  A2  // テストリード電圧（PB4はA2）

unsigned long V = 0L;
unsigned int v[100];

void setup() {
  // ソフトウェアシリアルの起動
  mySerial.begin(9600); 
  mySerial.println("ICCC(ATtiny85) start");

  // ピンアサイン
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
  
  // ADC設定
  analogReference(INTERNAL);  // 基準電圧を内部1.1Vに設定

  delay(1000);

}

void loop() {

  double avr = 0;
  for (int i = 0 ; i < 100 ; i++) {
    v[i] = analogRead(TEST_PIN);
    avr += v[i];
  }
  avr = avr / 100.0;

  double var = 0;
  for (int i = 0 ; i < 100 ; i++) {
    var += (v[i] - avr) * (v[i] - avr);
  }
  var /= 100;

  

  mySerial.print(avr); mySerial.print(", "); mySerial.println(var);  

  delay(500);
}

/*
 * テストリード（PB4/A2）の電圧を返す
*/
unsigned long readVTest() {
  unsigned long sum = 0L;
  for (int i = 0 ; i < 100 ; i++) sum += analogRead(TEST_PIN); // 安定のため10回測定して平均を取る
  //return (1.1 * sum / 10240.0); // 電圧に変換して返す
  return (sum); 
}