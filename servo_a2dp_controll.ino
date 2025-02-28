#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include <ESP32Servo.h>

const int SERVO_PIN = 18;
Servo myServo;
BluetoothA2DPSink a2dp_sink;

// グローバル変数に最新の volume を入れる
volatile int currentVolume = 0;

// 音量コールバック
void volume_callback(int volume) {
  currentVolume = volume;
}

void setup() {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);

  // BluetoothA2DPSink の設定
  a2dp_sink.set_on_volumechange(volume_callback);
  a2dp_sink.start("F00_Bluetooth");

  Serial.println("A2DP Sink started. Waiting for connection...");
}

void loop() {
  // スマホからのボリューム（0〜127）を 角度（0〜180）にマッピング
  int angle = map(currentVolume, 0, 127, 0, 180);
  angle = constrain(angle, 0, 180);

  // サーボに即時書き込み
  myServo.write(angle);

  // ここで 1秒待つと動きが遅く感じるので、できるだけ短い delay にする
  delay(20);  // 例: 20msごとに更新
}
