/*
 * ICCC (In-Circit Conductivity Cheker) for ATTiny85
 * programmed by KAWAMOTO
*/

// ソフトウェアシリアル
//#include <SoftwareSerial.h>
//#define RX_PIN 0   // USB-シリアル変換モジュールのTXへつなぐ
//#define TX_PIN 1   // USB-シリアル変換モジュールのRXへつなぐ
//SoftwareSerial mySerial(RX_PIN, TX_PIN);    // RX,TXの割り当て

// ピンアサイン
#define BZ_PIN    0   // ブザー出力（PB0）
#define SW_PIN    1   // スイッチ入力（PB1）
#define EN_PIN    2   // 電源許可出力（PB2）
#define VBATT_PIN A3  // バッテリー電圧（PB3はA3）
#define TEST_PIN  A2  // テストリード電圧（PB4はA2）

// 変数
unsigned int count_sw; // スイッチ押下カウンタ
unsigned long ms_last_event; // 最後にテスター検知をした時刻
double adc_test; // テスターのADC値

void setup() {
  // ソフトウェアシリアルの起動
  //mySerial.begin(9600); 
  //mySerial.println("ICCC(ATtiny85) start");
  
  // ピンアサイン
  //pinMode(SW_PIN, INPUT);
  pinMode(EN_PIN, OUTPUT);
  
  // ADC設定
  analogReference(INTERNAL);  // 基準電圧を内部1.1Vに設定

  // 変数のセット
  count_sw = 0;
  ms_last_event = millis();

  // 準備完了  
  digitalWrite(EN_PIN, HIGH); // とりあえず電源許可を出す
  delay(500);
}

void loop() {
  // バッテリー残量のチェック
  if (readVBatt() < 0.9) {// バッテリー残量が0.9V未満となった
    noTone(BZ_PIN); delay(200); // 一旦ブザーを止める
    tone(BZ_PIN, 554, 240); delay(250); tone(BZ_PIN, 932, 240); delay(250); 
    tone(BZ_PIN, 554, 240); delay(250); tone(BZ_PIN, 932, 240); delay(250); 
    noTone(BZ_PIN); // 一旦ブザーを止める
    digitalWrite(EN_PIN, LOW); // 電源許可を停止
    while(1){;} // 停止まで無期限待機
  }

  // スイッチのチェック
  if (digitalRead(SW_PIN) == LOW) { // スイッチが押されていない
    count_sw = 0; // スイッチ押下カウントをゼロにリセット
  } else if (count_sw < 5) { // スイッチが押されているがまだ5回に満たない
    count_sw++; // 押下回数をインクリメント
  } else { // 5回以上押し続けられてるので強制電源オフ
    noTone(BZ_PIN); delay(200); // 一旦ブザーを止める
    tone(BZ_PIN, 2688, 1000); delay(1250); // 高音で1秒発音 
    noTone(BZ_PIN); // 一旦ブザーを止める
    digitalWrite(EN_PIN, LOW); // 電源許可を停止
    while(1){;} // 停止まで無期限待機
  }

  // 自動電源オフのチェック
  if (millis() - ms_last_event > 30000) { // 最後のテスター検知から30秒以上経過した
    noTone(BZ_PIN); delay(200); // 一旦ブザーを止める
    tone(BZ_PIN, 2688, 1000); delay(1250); // 高音で1秒発音 
    noTone(BZ_PIN); // 一旦ブザーを止める
    digitalWrite(EN_PIN, LOW); // 電源許可を停止
    while(1){;} // 停止まで無期限待機
  }

  // テスター検知のチェック
  adc_test = readADCTest();
  if (adc_test < 2.0) { // ADC値が2（大体10Ω）未満
    tone(BZ_PIN, 2688); // 高音連続音
    ms_last_event = millis(); // テスター検知時刻を更新
  } else if (adc_test < 10.0) { // ADC値が2（10Ω）以上 10（50Ω）未満
    noTone(); // ブザーを止める
    tone(BZ_PIN, 928, 200); delay(250); // 中音断続音
    ms_last_event = millis(); // テスター検知時刻を更新
  } else { // 絶縁状態
    noTone(); // ブザーを止める
  }

  delay(100);
}

/*
 * バッテリー電圧の取得（内部基準電圧1.1V以上は全て1.1を返す）
*/
double readVBatt() {
  unsigned long sum = 0L;
  for (int i= 0 ; i < 10 ; i++) sum += analogRead(VBATT_PIN);
  return (1.1 * sum / 10240);  
}

/*
 * テスターADC値の取得
*/
double readADCTest() {
  unsigned long sum = 0L;
  for (int i= 0 ; i < 10 ; i++) sum += analogRead(TEST_PIN);
  return (sum / 10);  
}
