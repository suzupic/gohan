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
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);     // M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);    // M2

// For Ranging (HC-SR04)
const int trigPin = 6;
const int echoPin = 7;

// For Servo moter
Servo arm;
int position;
const int servoPin = 10;  // Servo 1 in MotorShield 

// For Start or Stop Buttun
const int buttunPin = 11;
int buttonState = 0;

int LED = 13;
int wallCount = 0;


void setup(){

    AFMS.begin();  // create with the default frequency 1.6KHz
    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);

    Serial.begin (9600);

    // Setup Ultrasonic Ranging Module
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    // Setup Servo moter
    pinMode(servoPin, OUTPUT);
    arm.attach(servoPin);    // 9 pin to servo

    pinMode(buttunPin, INPUT);

    pinMode(LED, OUTPUT);

}

// Main 
void loop(){
  
    double distance = 0;
    Serial.print(wallCount);
    Serial.println(" wall count");

    distance = range();

    if (distance >= 400 || distance <= 0){
        Serial.println("Out of range");
    }
    else {
        Serial.print(distance);
        Serial.println(" cm");
    }
    
    if (distance < 25.0 && wallCount == 0){
        while(distance < 15.0){
        AllStop();
        distance = range();
        }
        wallCount++;
    }

    else if (distance < 15.0 && wallCount < 2){
        Turn();
        digitalWrite(LED, LOW);
        wallCount++;
    }

    else if (distance < 15.0){
        AllStop();
    }

    else {
        Forward();
        digitalWrite(LED, HIGH);
    }


}

// Moter Functions 
void Forward(){

    rightMotor->setSpeed(60);
    leftMotor->setSpeed(60);

    rightMotor->run(FORWARD);
    leftMotor->run(FORWARD);

}

// turn right
void Turn(){

    rightMotor->setSpeed(35);
    leftMotor->setSpeed(35);

    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);

    delay(1415);

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
