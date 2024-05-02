/* demo ICCC(ATTiny85) テスト検知のコードデモ */

// ソフトウェアシリアル
//#include <SoftwareSerial.h>
//#define RX_PIN 1   // USB-シリアル変換モジュールのTXへつなぐ
//#define TX_PIN 0   // USB-シリアル変換モジュールのRXへつなぐ
//SoftwareSerial mySerial(RX_PIN, TX_PIN);    // RX,TXの割り当て

// ピンアサイン
#define BZ_PIN  2   // ブザー出力
#define TEST_PIN  A2  // テストリード電圧（PB4はA2）

unsigned long V = 0L;

void setup() {
  // ソフトウェアシリアルの起動
  //mySerial.begin(9600); 
  //mySerial.println("ICCC(ATtiny85) start");

  // ピンアサイン
  pinMode(BZ_PIN, OUTPUT);
  digitalWrite(BZ_PIN, LOW);
  
  // ADC設定
  analogReference(INTERNAL);  // 基準電圧を内部1.1Vに設定

  delay(1000);

}

void loop() {

  V = readVTest();
  
  //mySerial.println(V);

  digitalWrite(BZ_PIN, (V<100)?HIGH:LOW);


  delay(50);
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