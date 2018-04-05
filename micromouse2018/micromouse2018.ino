#include <Arduino.h>
#include "BasicStepperDriver.h"

//define pin outs sensors
#define LEFT A4
#define RIGHT A2
#define FRONT A0

//define pin outs button and LEDS
#define LEDR 13
#define LEDB 11
#define LEDG 10
#define button 12

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

// All the wires needed for full functionality
#define DIR1 3
#define DIR2 7
#define STEP1 2
#define STEP2 6

//Define the boundary for the side sensors. The robot should reangle itself if its too close
//or too far from walls.
#define sideClose 300
#define sideFar 240

// 2-wire basic config, microstepping is hardwired on the driver
//Stepper1 is left stepper, Stepper2 is right stepper.
BasicStepperDriver stepper1(MOTOR_STEPS, DIR1,STEP1);
BasicStepperDriver stepper2(MOTOR_STEPS, DIR2, STEP2);

////Mode Acceleration
BasicStepperDriver::Mode current_mode = BasicStepperDriver::Mode::LINEAR_SPEED;
short accel = 100;
short decel = 100;

void setup() {

    //setup two stepper motors in full step mode
    stepper1.begin(RPM, MICROSTEPS);
    stepper1.setSpeedProfile(current_mode, accel, decel);
    stepper2.begin(RPM, MICROSTEPS);
    stepper2.setSpeedProfile(current_mode, accel, decel);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);

    pinMode(LEDR, OUTPUT);
    pinMode(LEDB, OUTPUT);
    pinMode(LEDG, OUTPUT);

    Serial.begin(9600);

}

void loop() {
//  if (checkFront()){
//    resetLEDs();
//    digitalWrite(LEDR,HIGH);
//    moveOneBlock();
//  } else if (checkLeft()){
//    resetLEDs();
//    digitalWrite(LEDB,HIGH);
//    rotateLeft90();
//    moveOneBlock();
//  } else if (checkRight()){
//    resetLEDs();
//    digitalWrite(LEDG,HIGH);
//    rotateRight90();
//    moveOneBlock();
//  } else{
//    resetLEDs();
//    digitalWrite(LEDR,HIGH);
//    digitalWrite(LEDG,HIGH);
//    digitalWrite(LEDB,HIGH);
//    rotate180();
//    moveOneBlock();
//  }
//  delay(100);

//    followLeft();
//    backUpLeft();
    navieLeft();
    delay(5000);


}

void resetLEDs(){
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDR, LOW);
}

void moveOneBlock(){
  for (int i = 0; i < 191; i++){
    stepper1.move(1);
    stepper2.move(1);
    delay(20);
  }
  //stop();
}

void rotateLeft90(){
  for (int i = 0; i < 95; i++){
    stepper1.move(-1);
    stepper2.move(1);
    delay(20);
  }
}

void rotateRight90(){
  for (int i = 0; i < 95; i++){
    stepper1.move(1);
    stepper2.move(-1);
    delay(20);
  }
}

void rotate180(){
  for (int i = 0; i < 95*2; i++){
    stepper1.move(1);
    stepper2.move(-1);
    delay(20);
  }
}

void stop(){
  stepper1.startBrake();
  stepper2.startBrake();
}

//Follow the left wall for one block
//If too close or too far from left wall, will adjust servo on one side.
void followLeft(){
    int i = 0;
    while(i < 191){
      if(analogRead(LEFT) > 250){
        stepper1.move(1);
        stepper2.move(1);
        stepper1.move(1);
        i += 2;}
      else if(analogRead(LEFT) < 250){
        stepper2.move(1);
        stepper1.move(1);
        stepper2.move(1);
        i++;
      }
      else{
        stepper1.move(1);
        stepper2.move(1);
        i++;
      }
      delay(20);
  }
}

//Trying to overcome the delay with a limited adjustment of direction once a while
//Can't do sharp turns. Currently the only working sensor auxilerated function.
void navieLeft(){
    int i = 0;
    int fullDistence = 190;
    while(i < fullDistence){
      if(i == 0 || i == fullDistence/2 ){
        if(analogRead(LEFT) > 260){
          stepper1.move(1);
          stepper2.move(-1);
          delay(10);
      }
      else if(analogRead(LEFT) < 220){
          stepper2.move(1);
          stepper1.move(-1);
          delay(10);
      }
      }

      stepper1.move(1);
      stepper2.move(1);
      delay(10);
      i += 1;
    }
}

void navieRight(){
    int i = 0;
    int fullDistence = 190;
    while(i < fullDistence){
      if(i == 0 || i == fullDistence/2 ){
        if(analogRead(RIGHT) < 260){
          stepper1.move(1);
          stepper2.move(-1);
          delay(10);
      }
      else if(analogRead(RIGHT) > 220){
          stepper2.move(1);
          stepper1.move(-1);
          delay(10);
      }
      }

      stepper1.move(1);
      stepper2.move(1);
      delay(10);
      i += 1;
    }
}
//Backup one grid following left wall.
void backUpLeft(){
    int i = 0;
    while(i < 191){
      if(analogRead(LEFT) > 250){
        stepper1.move(-1);
        stepper2.move(-1);
        stepper1.move(-1);
        i += 2;}
      else if(analogRead(LEFT) < 250){
        stepper2.move(-1);
        stepper1.move(-1);
        stepper2.move(-1);
        i++;
      }
      else{
        stepper1.move(-1);
        stepper2.move(-1);
        i++;
      }
      delay(20);
  }
}

bool checkFront(){
  return analogRead(FRONT) <= 50;
}

bool checkLeft(){
  return analogRead(LEFT) <= 150;
}

bool checkRight(){
  return analogRead(RIGHT) <= 150;
}
