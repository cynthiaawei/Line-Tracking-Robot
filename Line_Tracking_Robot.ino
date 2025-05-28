#include <Adafruit_MotorShield.h>
// Define Sensor
#define IR_FRONT_LEFT A4
#define IR_FRONT_RIGHT A0
#define IR_FRONT_MIDDLE A2
#define IR_BACK_RIGHT A1
#define IR_BACK_LEFT A3
// Defining Motor Pins
#define MOTOR_FR_TERMINAL 1
#define MOTOR_FL_TERMINAL 2
#define MOTOR_BR_TERMINAL 3
#define MOTOR_BL_TERMINAL 4
// Instantiating the Motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // default board constructor
// Accessing the Motors from the Motorshield
Adafruit_DCMotor *MOTOR_FL = AFMS.getMotor(MOTOR_FL_TERMINAL);
Adafruit_DCMotor *MOTOR_FR = AFMS.getMotor(MOTOR_FR_TERMINAL);
Adafruit_DCMotor *MOTOR_BL = AFMS.getMotor(MOTOR_BL_TERMINAL);
Adafruit_DCMotor *MOTOR_BR = AFMS.getMotor(MOTOR_BR_TERMINAL);
// Time variable
int time = 0;
int timer = 0;
char lastMove = 'o';
bool start = false;
bool end = false;

void setup() {
  // put your setup code here, to run once:

    pinMode(IR_FRONT_RIGHT, INPUT);
    pinMode(IR_FRONT_LEFT, INPUT);
    pinMode(IR_FRONT_MIDDLE, INPUT);
    pinMode(IR_BACK_LEFT, INPUT);
    pinMode(IR_BACK_RIGHT, INPUT);

  // Turn on the motorshield and PWM driver
  AFMS.begin();

  // Reset Motor A
  MOTOR_FL->setSpeed(0);
  MOTOR_FL->run(RELEASE);

  // Reset Motor B
  MOTOR_FR->setSpeed(0);
  MOTOR_FR->run(RELEASE);

  // Reset Motor C
  MOTOR_BL->setSpeed(0);
  MOTOR_BL->run(RELEASE);

  // Reset Motor B
  MOTOR_BR->setSpeed(0);
  MOTOR_BR->run(RELEASE);

}
void loop() {
   // 1 is black, 0 is white
  if (digitalRead(IR_FRONT_RIGHT)==1){
      time = 0;
      slightTurn(255, conTurn('r'));
  } else if (digitalRead(IR_FRONT_LEFT)==1){
      time = 0;
      slightTurn( conTurn('l'), 255);
      
  } else {
    goForwards(190);
    if(time > 2){
      goForwards(255);
    } 
    conTurn('f');
  }
  delay(1);

  // if((digitalRead(IR_FRONT_RIGHT) == 0 && digitalRead(IR_FRONT_MIDDLE) == 0 && digitalRead(IR_BACK_LEFT) == 1) ||(digitalRead(IR_FRONT_LEFT) == 1 && digitalRead(IR_FRONT_MIDDLE) == 1 && digitalRead(IR_BACK_RIGHT) == 1)){
  //   stop();
  //   delay(300);
  // }
  if(digitalRead(IR_FRONT_RIGHT) == 1 && digitalRead(IR_FRONT_LEFT) == 1 && digitalRead(IR_FRONT_MIDDLE) == 1){
    stop();
    delay(300);
    lastMove = 's';

  }

  if (timer % 10 == 0){
    Serial.println(lastMove);
    Serial.println(timer);

  }

}

void goForwards(int speed){   // speed = 0, minimum speed, speed = 255, full speed
  MOTOR_FL->setSpeed(speed);
  MOTOR_FR->setSpeed(speed);
  MOTOR_BL->setSpeed(speed);
  MOTOR_BR->setSpeed(speed);

  MOTOR_FL->run(FORWARD);
  MOTOR_FR->run(FORWARD);
  MOTOR_BL->run(FORWARD);
  MOTOR_BR->run(FORWARD);
}
void goBackwards(int speed){
  MOTOR_FL->setSpeed(speed);
  MOTOR_FR->setSpeed(speed);
  MOTOR_BL->setSpeed(speed);
  MOTOR_BR->setSpeed(speed);

  MOTOR_FL->run(BACKWARD);
  MOTOR_FR->run(BACKWARD);
  MOTOR_BL->run(BACKWARD);
  MOTOR_BR->run(BACKWARD);  
}
void stop(){
  MOTOR_FL->setSpeed(0);
  MOTOR_FR->setSpeed(0);
  MOTOR_BL->setSpeed(0);
  MOTOR_BR->setSpeed(0);

  MOTOR_FL->run(RELEASE);
  MOTOR_FR->run(RELEASE);
  MOTOR_BL->run(RELEASE);
  MOTOR_BR->run(RELEASE);
}
void slightTurn(int left, int right){   // turn right (right < left), turn left (left < right)
  MOTOR_FL->setSpeed(left);
  MOTOR_FL->run(FORWARD);

  // Motor B
  MOTOR_FR->setSpeed(right);
  MOTOR_FR->run(FORWARD);

  // Motor C
  MOTOR_BL->setSpeed(left);
  MOTOR_BL->run(FORWARD);

  // Motor D
  MOTOR_BR->setSpeed(right);
  MOTOR_BR->run(FORWARD);
}
void sharpTurnLeft(int left, int right){
  MOTOR_FL->setSpeed(left);
  MOTOR_FL->run(BACKWARD);

  // Motor B
  MOTOR_FR->setSpeed(right);
  MOTOR_FR->run(FORWARD);

  // Motor C
  MOTOR_BL->setSpeed(left);
  MOTOR_BL->run(BACKWARD);

  // Motor D
  MOTOR_BR->setSpeed(right);
  MOTOR_BR->run(FORWARD);
}
void sharpTurnRight(int left, int right){
  MOTOR_FL->setSpeed(left);
  MOTOR_FL->run(FORWARD);

  // Motor B
  MOTOR_FR->setSpeed(right);
  MOTOR_FR->run(BACKWARD);

  // Motor C
  MOTOR_BL->setSpeed(left);
  MOTOR_BL->run(FORWARD);

  // Motor D
  MOTOR_BR->setSpeed(right);
  MOTOR_BR->run(BACKWARD);
}
bool ifStop(){
  if (digitalRead(IR_FRONT_RIGHT) == 1 && digitalRead(IR_FRONT_MIDDLE) == 1 && digitalRead(IR_BACK_LEFT) == 1){
    stop();
    return true;
  } else if (digitalRead(IR_FRONT_LEFT) == 1 && digitalRead(IR_FRONT_MIDDLE) == 1 && digitalRead(IR_BACK_RIGHT) == 1){
    stop();
    return true;
  } 
  return false;
}
int conTurn( char currentMove){
  int temp = 255 -200 - timer;
  if (lastMove == currentMove){
        timer++;
      }
      else {
        timer =0;
      }
    lastMove = currentMove;
  if (temp <= 0){
    return 0;
  }
  
  return temp;
}

