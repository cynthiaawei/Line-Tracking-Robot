#include "Adafruit_BluefruitLE_SPI.h"
#include <Adafruit_MotorShield.h>

#define VERBOSE_MODE                   false

#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4

#include <Adafruit_MotorShield.h>
// Define Sensor
#define IR_LEFT A4
#define IR_RIGHT A0
#define IR_MIDDLE A2
#define IR_MIDDLE_RIGHT A1
#define IR_MIDDLE_LEFT A3
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


//Define the bluetooth module object
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A function that prints the error
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// Time variable
int currTime = 0;
int timer = 0;
char lastMove = 'o';
bool start = false;
bool end = false;

void setup() {
  // put your setup code here, to run once:

    pinMode(IR_RIGHT, INPUT);
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_MIDDLE, INPUT);
    pinMode(IR_MIDDLE_LEFT, INPUT);
    pinMode(IR_MIDDLE_RIGHT, INPUT);

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

   // Initialise the bluetooth module
    if ( !ble.begin(VERBOSE_MODE) ) {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }


    // Perform a factory reset to make sure everything is in a known state


    if ( !ble.factoryReset() ){
        error(F("Couldn't factory reset"));
    }
    //Disable command echo from Bluefruit so commands aren't sent back for confirmation
    ble.echo(false);
    //ble.info() can be used to get info about the bluefruit module
    // prevents module from sending extensive debug info
    ble.verbose(false);
    //Wait for connection
    while (! ble.isConnected()) {
        delay(500);
    }
    // Set module to DATA mode
    ble.setMode(BLUEFRUIT_MODE_DATA);

   
}
void loop() {
  // read all five sensors once
  bool sr = digitalRead(IR_RIGHT);
  bool sm = digitalRead(IR_MIDDLE);
  bool sl = digitalRead(IR_LEFT);
  bool smr = digitalRead(IR_MIDDLE_RIGHT);
  bool sml = digitalRead(IR_MIDDLE_LEFT);

  // Case A: all black OR all white → drive straight
  if ((sr&&sm&&sl&&smr&&sml) || (!sr&&!sm&&!sl&&!smr&&!sml)) {
    goForwards(255);
    if (currTime > 2) goForwards(255);
    conTurn('f');
    delay(1);
    return;    // skip everything else
  }

  // Case B: normal line‑following
  if (sr) {
    currTime = 0;
    slightTurn(255, conTurn('r'));
  }
  else if (sl) {
    currTime = 0;
    slightTurn(conTurn('l'), 255);
  }
  else {
    goForwards(255);
    if (currTime > 2) goForwards(255);
    conTurn('f');
  }

  if (sl && !sm && !sr && !smr && !sml) {
  // Only far left is triggered -> SHARP LEFT TURN
  sharpTurnLeft(255, 255);
  delay(150); // Tune this value to adjust turn amount
  return;
}
if (sr && !sm && !sl && !smr && !sml) {
  // Only far right is triggered -> SHARP RIGHT TURN
  sharpTurnRight(255, 255);
  delay(150); // Tune this value to adjust turn amount
  return;
}
  delay(1);

  // DEBUG or other Bluetooth code…

  // put your main code here, to run repeatedly:
    char button;


  // get controller input as a single char
  if ( ble.available() ){
    button = getInput();
    //pressed: (Up, down, left, right) => (A, B, C, D)
    //let go: (Up, down, left, right) => (H, I, J, K)

  //rotate servo according to input
  if(button == 'A'){
    //going forward
    MOTOR_FR->setSpeed(255);
    MOTOR_FR->run(FORWARD);
    MOTOR_FL->setSpeed(255);
    MOTOR_FL->run(FORWARD);
    MOTOR_BL->setSpeed(255);
    MOTOR_BL->run(FORWARD);
    MOTOR_BR->setSpeed(255);
    MOTOR_BR->run(FORWARD);
  } else if (button == 'B'){
    //going backwards
    MOTOR_FR->setSpeed(255);
    MOTOR_FR->run(BACKWARD);
    MOTOR_FL->setSpeed(255);
    MOTOR_FL->run(BACKWARD);
    MOTOR_BL->setSpeed(255);
    MOTOR_BL->run(BACKWARD);
    MOTOR_BR->setSpeed(255);
    MOTOR_BR->run(BACKWARD);
  } else if (button == 'C'){
    //going left
    MOTOR_FR->setSpeed(255);
    MOTOR_FR->run(FORWARD);
    MOTOR_FL->setSpeed(255);
    MOTOR_FL->run(BACKWARD);
    MOTOR_BL->setSpeed(255);
    MOTOR_BL->run(BACKWARD);
    MOTOR_BR->setSpeed(255);
    MOTOR_BR->run(FORWARD);
  } else if (button == 'D'){
    //going right
    MOTOR_FR->setSpeed(255);
    MOTOR_FR->run(BACKWARD);
    MOTOR_FL->setSpeed(255);
    MOTOR_FL->run(FORWARD);
    MOTOR_BL->setSpeed(255);
    MOTOR_BL->run(FORWARD);
    MOTOR_BR->setSpeed(255);
    MOTOR_BR->run(BACKWARD);
  } else if (button == 'H' || button == 'I' || button == 'J' || button == 'K'){
    MOTOR_FR->setSpeed(0);
    MOTOR_FR->run(FORWARD);
    MOTOR_FL->setSpeed(0);
    MOTOR_FL->run(FORWARD);
    MOTOR_BL->setSpeed(0);
    MOTOR_BL->run(FORWARD);
    MOTOR_BR->setSpeed(0);
    MOTOR_BR->run(FORWARD);
  }
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

int conTurn( char currentMove){
  int temp = 255 - 200 - timer;
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
char getInput(){
  //reads next 5 chars and returns the last button pressed
  char output;
  char btnNum;
  char onOrOff;
  //first two chars are always !B
  output = ble.read();
  output = ble.read();
  //third char says which button, 4th char says where it was pressed or let go
  btnNum = ble.read();
  onOrOff = ble.read();
  //fifth char is useless, go over it
  output = ble.read();

  Serial.print(btnNum);
  //logic
  if(onOrOff == '1'){
    //all cases where button was pressed/not let go
    if (btnNum == '5'){
      output = 'A';
    } else if (btnNum == '6'){
      output = 'B';
    } else if (btnNum == '7'){
      output = 'C';
    } else if (btnNum == '8'){
      output = 'D';
    }
  } else { //onOrOff == '0', cases were button was let go
    if (btnNum == '5'){
      output = 'H';
    } else if (btnNum == '6'){
      output = 'I';
    } else if (btnNum == '7'){
      output = 'J';
    } else if (btnNum == '8'){
      output = 'K';  
    }
  }
  return output;
}
