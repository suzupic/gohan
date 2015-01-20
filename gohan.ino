// For Servo Motor
#include <Servo.h> 

// For Adafuruit Motor Shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
// #include "utility/Adafruit_PWMServoDriver.h"

// For Motor
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)

// Select which 'port' M1, M2
Adafruit_DCMotor *leftMotor  = AFMS.getMotor(1);    // M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);    // M2

// For Ranging (HC-SR04)
const int trigPin = 6;
const int echoPin = 7;

// For Servo moter
Servo arm;
int position;
const int servoPin = 9;  // Servo 2 in MotorShield 

// For Start or Stop Buttun
// const int buttunPin = 11;
// int buttonState = 0;

// H8 to Arduino
const int ssmStatusPin = 2;    // "ssm" mean "susumu-kun" 
int susumuStatus;

// show status
const int LED = 13;

// other 
int wallCount = 0;
int noiseCount = 0;


void setup(){

    AFMS.begin();  // create with the default frequency 1.6KHz
    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);

    // Serial.begin (9600);

    // Setup H8 Program Finish
    pinMode(ssmStatusPin, INPUT);

    // Setup Ultrasonic Ranging Module
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Setup Servo moter
    pinMode(servoPin, OUTPUT);
    arm.attach(servoPin);   // 9 pin to servo

    // pinMode(buttunPin, INPUT);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    resetVelocity();
}

// Main 
void loop(){

    double distance = 0;

    OUT:
    
    // Serial.print(wallCount);
    // Serial.println(" wall count");

    distance = range();
    susumuStatus = digitalRead(ssmStatusPin);

    // if (distance >= 400 || distance <= 0){
    //     Serial.println("Out of range");
    // }
    // else {
    //     Serial.print(distance);
    //     Serial.println(" cm");
    // }

    if (distance < 60.0 && wallCount == 0){
        while(distance < 40.0){
            AllStop();
            distance = range();
            resetVelocity();
            delay(5);
        }
        wallCount++;
    }

    else if (distance < 22.0 && wallCount < 2){

        if (noiseCount < 30) {
            noiseCount++;
            goto OUT;
        }

        Turn();
        wallCount++;
    }

    else if (distance < 35.0 && wallCount == 2 ){

        if (noiseCount < 30) {
            noiseCount++;
            goto OUT;
        }

        else if (digitalRead(ssmStatusPin) == HIGH){
            servoArm();
            wallCount++;
        }

        digitalWrite(LED, HIGH);
        AllStop();
    }

    else if (distance < 35.0){
        AllStop();
        digitalWrite(LED, LOW);
        noiseCount = 0;
    }

    else {
        Forward();
        digitalWrite(LED, LOW);
        noiseCount = 0;
        resetVelocity();
    }


}

// Moter Functions 
void Forward(){

    rightMotor->setSpeed(80);
    leftMotor->setSpeed(84);

    rightMotor->run(FORWARD);
    leftMotor->run(FORWARD);

}

// turn right
void Turn(){

    rightMotor->setSpeed(43);
    leftMotor->setSpeed(43);

    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);

    delay(1050);

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

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;

    return distance;

}

// For Servo motor Arm
void servoArm() {

    arm.write(120);
    delay(5000);
    resetVelocity();

}

// reset servo arm
void resetVelocity(){

    arm.write(90);

}
