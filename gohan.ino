// For Servo Motor
#include <Servo.h> 

// For Adafuruit Motor Shield
//// ライブラリはここからダウンロード
//// https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include <Wire.h>   // Adafruit_MotorShield.h を使うために必要
#include <Adafruit_MotorShield.h>
// #include "utility/Adafruit_PWMServoDriver.h"

// For Motor
// Create the motor shield object with the default I2C address
//// Adafruit の MotorShield は I2C 通信で制御している
//// I2C を使うためには Wire.h が要る
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)

// Select which 'port' M1, M2
Adafruit_DCMotor *leftMotor  = AFMS.getMotor(1);    // M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);    // M2

// For Ranging (HC-SR04)
//// const int == #define 
const int trigPin = 6;  // 周波を出す
const int echoPin = 7;  // 周波を受け取る

// For Servo moter
//// 使用しているサーボモータに
////  "arm" って名前をつけたという認識で
Servo arm;
const int servoPin = 9;  // "Servo 2" in MotorShield 

// H8 to Arduino
const int ssmStatusPin = 3;    // "ssm" mean "susumu-kun" 

// show status
//// 今回はボタン押し待機時に
//// LEDが光るようにプログラムしてある
const int LED = 13;

// other 
//// 壁の回数をカウント
//// wallCount == 0 -> susumu のスタートを待機
//// wallCount == 1 -> 1回目の外周の壁 
//// wallCount == 2 -> ボタンの前
int wallCount = 0;

//// 距離センサの誤作動対策
int noiseCount = 0;


void setup(){

    AFMS.begin();  // create with the default frequency 1.6KHz
    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);

    // Setup H8 Program Finish
    pinMode(ssmStatusPin, INPUT);

    // Setup Ultrasonic Ranging Module
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Setup Servo moter
    pinMode(servoPin, OUTPUT);
    arm.attach(servoPin);   // 9 pin to servo

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    //// サーボモータの位置を初期化
    resetVelocity();
}

// Main 
void loop(){

    double distance = 0;

    //// ノイズ対策に使用しているgoto文
    //// breakが使えんからこうした
    OUT:
    
    //// 距離計測
    //// distance の値で以下の if 文を分岐させる
    distance = range();

    //// susumu のスタートを待機
    if (distance < 60.0 && wallCount == 0){
        while(distance < 40.0){
            AllStop();
            distance = range();
            resetVelocity();
            delay(5);
        }

        //// wallCount == 1 になるので, 
        //// 次の loop 時にこの if 文は無視される
        wallCount++;
    }

    //// 1回目の壁
    else if (distance < 22.0 && wallCount < 2){

        if (noiseCount < 30) {
            noiseCount++;
            goto OUT;
        }

        Turn();

        //// wallCount == 2 になるので, 
        //// 次の loop 時にこの if 文は無視される
        wallCount++;
    }

    //// ボタンの前 
    else if (distance < 41.0 && wallCount == 2 ){

        if (noiseCount < 30) {
            noiseCount++;
            goto OUT;
        }

        else if (digitalRead(ssmStatusPin) == HIGH){
            //// サーボモータを作動させる
            //// つまりボタンが押される
            servoArm();

            //// wallCount == 3 になるので, 
            //// 次の loop 時にこの if 文は無視される
            wallCount++;
        }
        
        else {
            digitalWrite(LED, HIGH);
            AllStop();
        }
    }

    //// 3回目以上の壁で止まる
    else if (distance < 35.0){
        AllStop();
        digitalWrite(LED, LOW);
        noiseCount = 0;
    }

    //// 基本的に直進
    else {
        Forward();
        digitalWrite(LED, LOW);
        noiseCount = 0;
        resetVelocity();
    }

    //// loop() 終了
}

// Moter Functions 
void Forward(){

    //// 少しだけ右に曲がるように直進する
    //// 逆にまっすぐ走らせるのは難しい
    rightMotor->setSpeed(84);
    leftMotor->setSpeed(86);

    rightMotor->run(FORWARD);
    leftMotor->run(FORWARD);

}

// turn right
void Turn(){

    //// 回転時間と速度は "経験則"
    //// 悪く言うと "勘"
    rightMotor->setSpeed(51);
    leftMotor->setSpeed(51);

    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);

    delay(1300);

}

void AllStop(){

    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);

    rightMotor->run(RELEASE);
    leftMotor->run(RELEASE);

}

// Ranging Functions
double range() {

    double duration, distance;

    //// 10 ms の間超音波を出して, 
    //// pulseIn で超音波を受け取る
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    //// 音波が行って帰ってくる時間を 2 で割る
    //// さらに音速から距離に直すために 29.1 を掛けて cm に直している
    distance = (duration/2) / 29.1;

    return distance;

}

// For Servo motor Arm
void servoArm() {

    //// resetVelocity で arm.write(90) なので
    //// (120 - 90) * 2 = 60 [deg] 回転する
    //// ( 使用したサーボモータは 360 [deg] 回転する )
    arm.write(120);
    delay(5000);
    resetVelocity();

}

// reset servo arm
void resetVelocity(){

    //// サーボモータの通常時の角度
    arm.write(90);

}
