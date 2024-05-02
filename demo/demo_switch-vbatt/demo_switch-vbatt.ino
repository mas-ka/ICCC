/* demo ICCC(ATTiny85) 電源とスイッチ周りのコードデモ */

// ソフトウェアシリアル
//#include <SoftwareSerial.h>
//#define RX_PIN 1   // USB-シリアル変換モジュールのTXへつなぐ
//#define TX_PIN 0   // USB-シリアル変換モジュールのRXへつなぐ
//SoftwareSerial mySerial(RX_PIN, TX_PIN);    // RX,TXの割り当て

// ピンアサイン
//#define BZ_PIN  0   // ブザー出力
#define SW_PIN  1   // 電源スイッチ
#define EN_PIN  2   // パワーON許可
#define VB_PIN  A3  // バッテリー電圧

// 電源制御
unsigned int count_sw;
unsigned long ms_APO;
boolean is_LED_ON = false;

// ブザー
#define BZ_FREQ = 2700; // ブザー鳴動周波数を2700Hzにする
boolean is_buzzer_ON;

void setup() {
  // ソフトウェアシリアルの起動
  //mySerial.begin(9600); 
  //mySerial.println("ICCC(ATtiny85) start");

  // ピン設定
  //pinMode(BZ_PIN, OUTPUT);
  pinMode(SW_PIN, INPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(4, OUTPUT);

  // ADC設定
  analogReference(INTERNAL);  // 基準電圧を内部1.1Vに設定

  // 起動時チェック
  digitalWrite(EN_PIN, LOW);
  if (!is_battery_OK()) { // バッテリー残量が少なかったら
    //mySerial.println("Low Battery!"); // 警告（ブザー長音１発とか）
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
    digitalWrite(EN_PIN, LOW);        // 明示的にパワーON許可を削除
    while(1) {;}                       // 死ぬまで待つ
  } else { // バッテリー残量が充分だった
    //mySerial.println("Enough Battery!"); // 起動音（ブザー短音１発とか）
    digitalWrite(EN_PIN, HIGH);        // パワーON許可を出す
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
    digitalWrite(4, HIGH); delay(100);
    digitalWrite(4, LOW); delay(100);
  }
  

  // 電源制御設定
  count_sw = 0;       // 電源スイッチの押下イベント回数をリセット
  ms_APO = millis();  // 自動パワーOFF設定タイマーの時刻をセット

  // ブザー状態
  //  noTone(BZ_PIN); // ブザーを明示的に停止する
  is_buzzer_ON = false; // ブザー状態を明示的に非鳴動にする

  delay(1000);

}

void loop() {
  if (digitalRead(SW_PIN) == HIGH) { // 電源スイッチが押下された
    count_sw++;
    //mySerial.println("ON");
  } else {
    count_sw = 0;
  }
  if ((count_sw > 3) ||                 // 電源スイッチが押下され続けた
      (!is_battery_OK()) ||             // バッテリー残量が少なかった
      (millis() - ms_APO > 15000)) {   // 自動パワーOFF時刻となった
    if (is_buzzer_ON) { // ブザーが鳴っているなら
      //noTone(BZ_PIN);               // ブザーを明示的に停止する
      delay(1000);                  // 1秒ほど間を開ける
    }
    //tone(BZ_PIN, BZ_FREQ, 1000);  // ブザーを1秒間鳴らす
    //mySerial.println("Auto Power OFF!");
    digitalWrite(4, HIGH);
    delay(1500);                     // 1.5秒ほど待つ
    digitalWrite(4, LOW);
    digitalWrite(EN_PIN, LOW);       // 明示的にパワーON許可を削除
    while(1) {;}                      // 死ぬまで待つ
  } else { // タイマーやバッテリー残量によるパワーOFFはしなくて良い
    // テスター検知
  }

  if (is_LED_ON) {
    digitalWrite(4, LOW); is_LED_ON = false;
  } else {
    digitalWrite(4, HIGH); is_LED_ON = true;
  }
  delay(500);
}

/*
 * バッテリー残量のチェック
 * バッテリー電圧が0.9V以下ならfalseを返す
*/
boolean is_battery_OK() {
  unsigned long sum = 0;
  for (int i = 0 ; i < 10 ; i++) sum += analogRead(VB_PIN); // 安定のため10回測定して平均を取る
  float v_batt = 1.1 * sum / 10240.0 ; // VB_PINのADC値を電圧に変換
  //float v_batt = 3.2;
  return (v_batt > 0.9); // バッテリー電圧が0.9V超ならTRUEを返す
}