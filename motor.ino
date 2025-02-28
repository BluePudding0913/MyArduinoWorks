#include <ESP32Servo.h>

Servo ex_mt;

void setup() {
  //ライブラリ割り当て
  ex_mt.attach(5);
}

void loop() {

  //９０度
  ex_mt.write(90);

  //1秒プログラム停止
  delay(1000);

  //０度
  ex_mt.write(0);

  //1秒プログラム停止
  delay(1000);
}
