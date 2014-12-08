// For Servo Motor
#include <Servo.h> 

// For Adafuruit Motor Shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// For Motor
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)

// Select which 'port' M1, M2
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);    // M1
Adafruit_DCMotor *leftMotor  = AFMS.getMotor(2);    // M2

// For Ranging (HC-SR04)
int rangeTrig = 2;
int rangeEcho = 4;
double duration, distance = 0;   // distance[cm]

// For Servo moter
Servo arm;
int position;
int servoPin = 10;  // Servo 1 in MotorShield 

void setup(){

    AFMS.begin();  // create with the default frequency 1.6KHz

    // Setup Ultrasonic Ranging Module
    pinMode(rangeTrig, OUTPUT);
    pinMode(rangeEcho, INPUT);
    
    // Setup Servo moter
    pinMode(servoPin, OUTPUT);
    arm.attach(servoPin);    // 9 pin to servo

}

// Moter Functions 
void forword(){

    rightMotor->setSpeed(50);
    leftMotor->setSpeed(50);

    rightMotor->run(FORWARD);
    leftMotor->run(FORWARD);

}

// turn right
void turn(){

    rightMotor->setSpeed(50);
    leftMotor->setSpeed(50);

    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);

}

void stop(){

    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);

}

// Ranging Functions
void ranging(){
    digitalWrite(rangeTrig, HIGH);
    // Need HIGH pulse of 2 microsec or more 
    delayMicroseconds(10);
    digitalWrite(rangeEcho, LOW);
    
    duration = pulseIn(rangeEcho, HIGH);
    
    // convert pluse to cm
    distance = (duration / 2) / 29.1;
}

// Main 
void loop(){
  
    ranging();

    if (distance >= 3.0 || distance <= 0){
        forword();
    }
    else {
        stop();
    }

}
