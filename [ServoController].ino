#include <BluetoothSerial.h>
#include <ESP32Servo.h>
#include <math.h>

BluetoothSerial SerialBT;   // Bluetoothシリアル
Servo myServo;              // サーボモーターのオブジェクト
int servoPin = 27;          // サーボモーターを接続するピン
int led = 25;
int mosfetGatePin = 14;
const int sensorPin = 26;   // センサの入力ピン

// センサのパラメータ（KXR94-2050の仕様に合わせて調整してください）
const float V_supply = 3.3;               // センサへ供給する電圧
const float V_offset = V_supply / 2;        // 0g時のバイアス電圧（例：1.65V）
const float sensitivity = 0.66;             // センサの感度 (V/g)

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);         // シリアルモニタ開始
  SerialBT.begin("key_mortor"); // Bluetoothデバイス名設定
  Serial.println("Bluetooth is ready to pair");

  pinMode(mosfetGatePin, OUTPUT); // MOSFET制御ピンを出力モードに設定
  myServo.attach(servoPin);       // サーボモーターを指定のピンに接続
  pinMode(sensorPin, INPUT);      // センサ読み取り用ピンを設定
}

void loop() {
  if (SerialBT.available()) {     // Bluetoothからデータを受信
    char command = SerialBT.read(); // 受信したコマンドを読み取る

    if (command == '0') {
      float angleValue = getAngleFromSensor();  // センサーを読み、角度計算
      SerialBT.println(String(angleValue));       // 計算結果を送信
    } 
    else if (command == '1') {
      moveServoToAngle(0);    // 0度に移動
    }
    else if (command == '2') {
      moveServoToAngle(90);   // 90度に移動
    }
    else if (command == '3') {
      moveServoToAngle(180);  // 180度に移動
    }
    else if (command == '4') {
      digitalWrite(led, HIGH);
      Serial.println("Blink!");
    }
    else if (command == '5') {
      digitalWrite(led, LOW);
      Serial.println("Led Low.");
    }
  }
  delay(100); // 少し待機
}

// センサ値から角度を計算する関数
float getAngleFromSensor() {
  // アナログ読み取り（12bit分解能：0～4095）
  int sensorRaw = analogRead(sensorPin);
  // センサ出力電圧に変換
  float voltage = sensorRaw * (V_supply / 4095.0);
  // 0g時のバイアス電圧との差から加速度（g単位）を計算
  float acceleration = (voltage - V_offset) / sensitivity;
  
  // asinに渡す値は-1～1でなければならないのでクランプする
  if (acceleration > 1.0) acceleration = 1.0;
  else if (acceleration < -1.0) acceleration = -1.0;
  
  // 加速度から傾斜角（ラジアン）を計算し、度に変換
  float angleRad = asin(acceleration);
  float angleDeg = angleRad * (180.0 / PI);
  
  // デバッグ用シリアル出力
  Serial.print("Sensor Raw: ");
  Serial.print(sensorRaw);
  Serial.print(", Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V, Acceleration: ");
  Serial.print(acceleration, 3);
  Serial.print(" g, Angle: ");
  Serial.print(angleDeg, 3);
  Serial.println(" deg");

  return angleDeg;
}

// 指定角度にサーボを動作させる関数
void moveServoToAngle(int angle) {
  digitalWrite(mosfetGatePin, HIGH); // MOSFETをオン
  delay(50); // MOSFETの安定化待機

  myServo.write(angle);  // サーボを指定角度に回転
  Serial.print("Servo moving to ");
  Serial.print(angle);
  Serial.println(" degrees");

  delay(500); // サーボの動作待ち

  digitalWrite(mosfetGatePin, LOW); // MOSFETをオフ
  Serial.println("MOSFET turned off");
}
