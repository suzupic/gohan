// #include <Servo.h> // For Servo MOTOR

// For Motor
//// ch.A for Right Motor
#define RightMotor_Direction    12
#define RightMotor_PWM          3
#define RightMotor_Brake        9
//// ch.A for Left Motor
#define LeftMotor_Direction     13
#define LeftMotor_PWM           11
#define LeftMotor_Brake         8

// For Ranging (HC-SR04)
#define Range_Trig  2
#define Range_Echo  4
float duration, distance = 0;   // distance[cm]

/*
// For Servo moter
Servo servomoter;
int pos;
#define ServoPin  6
*/

void setup(){
    
    // Setup Channel A
    pinMode(RightMotor_Direction, OUTPUT);
    pinMode(RightMotor_Brake, OUTPUT);

    // Setup Channel B
    pinMode(LeftMotor_Direction, OUTPUT);
    pinMode(LeftMotor_Brake, OUTPUT);

    // Setup Ultrasonic Ranging Module
    pinMode(Range_Trig, OUTPUT);
    pinMode(Range_Echo, INPUT);
    
    // Setup Servo moter
    pinMode(ServoPin, OUTPUT);
    servo.attach(ServoPin);    // 9 pin to servo

// Moter Functions 
void forword(){
    // Motor A
    digitalWrite(RightMotor_Direction, HIGH);
    digitalWrite(RightMotor_Brake, LOW);
    analogWrite(RightMotor_PWM, 255);
    // Motor B
    digitalWrite(LeftMotor_Direction, HIGH);
    digitalWrite(LeftMotor_Brake, LOW);
    analogWrite(LeftMotor_PWM, 255);
}

// turn right
void turn(){
    // Motor A
    digitalWrite(RightMotor_Direction, HIGH);
    digitalWrite(RightMotor_Brake, LOW);
    analogWrite(RightMotor_PWM, 120);
    // Motor B
    digitalWrite(LeftMotor_Direction, LOW);
    digitalWrite(LeftMotor_Brake, LOW);
    analogWrite(LeftMotor_PWM, 120);
}

void stop(){
    // Motor A & B
    digitalWrite(RightMotor_Brake, HIGH);
    digitalWrite(LeftMotor_Brake, HIGH);
}

// Ranging Functions
void ranging(){
    digitalWrite(Range_Trig, HIGH);
    // Need HIGH pulse of 2 microsec or more 
    delayMicroseconds(2);
    digitalWrite(Range_Trig, LOW);
    
    duration = pulseIn(Range_Echo, HIGH);
    
    // convert pluse to cm
    distance = (duration / 2) / 29.1;
}

// Main 
void loop(){
  
    ranging();

    if (distance >= 5 || distance <= 0){
        forword();
    }
    else {
        stop();
    }

}
