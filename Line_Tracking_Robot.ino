#include <Adafruit_MotorShield.h>
// Define Sensor - from right to left // (A4 A3 A2 A1 A0)
#define IR_R A0
#define IR_RM A1
#define IR_M A2
#define IR_LM A3
#define IR_L A4
// Defining Motor Pins
#define MOTOR_FR_TERMINAL 2
#define MOTOR_FL_TERMINAL 3
#define MOTOR_BR_TERMINAL 1
#define MOTOR_BL_TERMINAL 4
// Instantiating the Motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // default board constructor
// Accessing the Motors from the Motorshield
Adafruit_DCMotor *MOTOR_FL = AFMS.getMotor(MOTOR_FL_TERMINAL);
Adafruit_DCMotor *MOTOR_FR = AFMS.getMotor(MOTOR_FR_TERMINAL);
Adafruit_DCMotor *MOTOR_BL = AFMS.getMotor(MOTOR_BL_TERMINAL);
Adafruit_DCMotor *MOTOR_BR = AFMS.getMotor(MOTOR_BR_TERMINAL);

// Global variables
//int elapsedTime = 0;
bool direction; // false = right, true = left
bool stopReadingSensor = false;

int timer = 0;
char lastMove = 'o';
bool start = false;
bool end = false;

void setup() {
  // put your setup code here, to run once:

    pinMode(IR_R, INPUT);
    pinMode(IR_RM, INPUT);
    pinMode(IR_M, INPUT);
    pinMode(IR_LM, INPUT);
    pinMode(IR_L, INPUT);

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

  if(threeRightSensorsBlack()){
    direction = true;
  } else {
    direction = false;
  }

}
void loop() {
// 1 is black, 0 is white5
  
  // if(!stopReadingSensor) {
    if(readAllBlack()){
        stop();
        delay(10);
    } else if (threeRightSensorsBlack()) {
        // turn left
        turn(255, 110);
    } else if (threeLeftSensorsBlack()) {
        turn(110,255);
    } else if (twoRightSensorsBlack()){
        turn(255, 65);
    } else if(twoLeftSensorsBlack()){
        turn(65,255);
    } else if (oneRightSensorBlack()){
        turn (255, 15);
        //delay(5);
    } else if (oneLeftSensorBlack()){
        turn (15, 255);
        //delay(5);
    } else if (centered()){
        goForwards(255);
    } else if (readAllBlack() || readAllWhite()){
        stop();
        delay(10);
        // stopReadingSensor = true;
    } 
    // if(readAllBlack()){
        // stopReadingSensor = false;
        
  // }
  // }
  //digitalRead(IR_L) == 1 && digitalRead((R_R) == 1
/*
   if (direction){ // turn left
      if(!stopReadingSensor){
          if(threeRightSensorsBlack()){
            // turn left
            turn(150, 200);
          } else if (twoRightSensorsBlack()){
            // turn left but stronger turn
            turn(100, 200);
          } else if (oneRightSensorBlack()){
            turn(50, 200); // turn even sharper
          } else if (centered()) {
            goForwards(255); // goal is that going forward never activates when turning
          } else if (readAllBlack()){
            stop();
            stopReadingSensor = true;
          }
      }
   } else { // turn right
      if(!stopReadingSensor){
          if(threeLeftSensorsBlack()){
            // turn left
            turn(150, 200);
          } else if (twoLeftSensorsBlack()){
            // turn left but stronger turn
            turn(100, 200);
          } else if (oneLeftSensorBlack()){
            turn(50, 200); // turn even sharper
          } else if (centered()) {
            goForwards(255); // goal is that going forward never activates when turning
          } else if (readAllBlack()){
            stop();
            stopReadingSensor = true;
          }
      }
   }


*/
}


bool threeLeftSensorsBlack(){
  if(digitalRead(IR_L) == 1 && digitalRead(IR_LM) == 1 && digitalRead(IR_M) == 1 && (digitalRead(IR_R) == 0 || digitalRead(IR_RM) == 0)){
    return true;
  } else {
    return false;
  }
}
bool threeRightSensorsBlack(){
  if(digitalRead(IR_R) == 1 && digitalRead(IR_RM) == 1 && digitalRead(IR_M) == 1 && (digitalRead(IR_L) == 0 || digitalRead(IR_LM) == 0)){
    return true;
  } else {
    return false;
  }
}
bool twoLeftSensorsBlack(){
  if(digitalRead(IR_L) == 1 && digitalRead(IR_LM) == 1 && digitalRead(IR_M) == 0){
    return true;
  } else {
    return false;
  }
}
bool twoRightSensorsBlack(){
  if(digitalRead(IR_R) == 1 && digitalRead(IR_RM) == 1 && digitalRead(IR_M) == 0){
    return true;
  } else {
    return false;
  }
}
bool centered(){
  if(digitalRead(IR_R) == 0 && digitalRead(IR_RM) == 1 && digitalRead(IR_M) == 1 && digitalRead(IR_LM) == 1 && digitalRead(IR_L) == 0){
      return true;
  } else {
      return false;
  }
}
bool oneLeftSensorBlack(){
  if(digitalRead(IR_L) == 1 && digitalRead(IR_LM) == 0 && digitalRead(IR_M) == 0 && digitalRead(IR_RM) == 0 && digitalRead(IR_R) == 0){
    return true;
  } else {
    return false;
  }
}
bool oneRightSensorBlack(){
  if(digitalRead(IR_L) == 0 && digitalRead(IR_LM) == 0 && digitalRead(IR_M) == 0 && digitalRead(IR_RM) == 0 && digitalRead(IR_R) == 1){
    return true;
  } else {
    return false;
  }
}
bool readAllBlack(){
  if((digitalRead(IR_L) == 1) && (digitalRead(IR_LM) == 1) && (digitalRead(IR_M) == 1) && (digitalRead(IR_RM) == 1) && (digitalRead(IR_R) == 1)){
    return true;
  } else {
    return false;
  }
}
bool readAllWhite(){
  if(digitalRead(IR_L) == 0 && digitalRead(IR_LM) == 0 && digitalRead(IR_M) == 0 && digitalRead(IR_RM) == 0 && digitalRead(IR_R) == 0){
    return true;
  } else {
    return false;
  }
}
void goForwards(int speed){   // speed = 0, minimum speed, speed = 255, full speed
  MOTOR_FL->run(FORWARD);
  MOTOR_FR->run(FORWARD);
  MOTOR_BL->run(FORWARD);
  MOTOR_BR->run(FORWARD);

  MOTOR_FL->setSpeed(speed);
  MOTOR_FR->setSpeed(speed);
  MOTOR_BL->setSpeed(speed);
  MOTOR_BR->setSpeed(speed);

  
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
void resetStop(){
  goForwards(0);
}
void turn(int left, int right){  
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
// int cont( char currentMove){
//   int temp = 255 -timer;
//   if (lastMove == currentMove){
//         timer++;
//       }
//       else {
//         timer =0;
//       }
//     lastMove = currentMove;
//   if (temp <= 0){
//     return 0;
//   }
//   return 0;
// }

