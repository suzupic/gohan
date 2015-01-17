// For Servo Motor
#include <Servo.h> 

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

// show status
int LED = 13;

// other 
int wallCount = 0;
double distance = 0;


void setup(){

    // Serial.begin (9600);

    // Setup Ultrasonic Ranging Module
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    // Setup Servo moter
    pinMode(servoPin, OUTPUT);
    arm.attach(servoPin);    // 9 pin to servo

    // pinMode(buttunPin, INPUT);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    resetVelocity();
}

// Main 
void loop(){
  
    // Serial.print(wallCount);
    // Serial.println(" wall count");

    distance = range();

    if(distance = 10.0 && wallCount == 0){
        servoArm();
        wallCount++;
    }
    else {
        resetVelocity();
    }

    delay(50)
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
    int i;

    for(i=90; i>85; i--){
        arm.write(i);
        delay(100);
    }
}

// reset servo arm
void resetVelocity(){
    arm.write(90);
}

