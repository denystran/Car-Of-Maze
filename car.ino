#include "carMazeOfOz.h"

#define STEP_TIME_64 4e-6
#define TIMER1_STEP_CYCLE 65536

const byte trig = 8;

carMazeOfOz car;

volatile float speedValueLeft, speedValueRight;
volatile float speedValueLeft_SAMPLE[11], speedValueRight_SAMPLE[11];
volatile unsigned long timerPoint = 0, currentEncoderLeft = 0, currentEncoderRight = 0;

// Khai bao bien o duoi
//--------------------------------------------------//
float dist_h=car.getDistanceHead();
float dist_r=car.getDistanceRight();
float dist_l=car.getDistanceLeft();
float speed_l=car.getSpeedLeft();
float speed_r=car.getSpeedRight();

//--------------------------------------------------//

void setup() {
  Serial.begin(9600);
  car.setPin();
  car.setInterrupt();
  attachInterrupt(0, ENC_LEFT_ISR, RISING);
  attachInterrupt(1, ENC_RIGHT_ISR, RISING);
}

void ENC_LEFT_ISR() {
  static byte i = 0;
  speedValueLeft_SAMPLE[10] -= speedValueLeft_SAMPLE[i];
  speedValueLeft_SAMPLE[i] = (TCNT1 - currentEncoderLeft + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueLeft_SAMPLE[10] += speedValueLeft_SAMPLE[i];
  speedValueLeft = 255254.4 / (speedValueLeft_SAMPLE[10] / 10);
  car.setSpeedLeft(speedValueLeft);
  i = (i + 1) % 10;
  currentEncoderLeft = TCNT1;
}

void ENC_RIGHT_ISR() {
  static byte i = 0;
  speedValueRight_SAMPLE[10] -= speedValueRight_SAMPLE[i];
  speedValueRight_SAMPLE[i] = (TCNT1 - currentEncoderRight + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
  speedValueRight_SAMPLE[10] += speedValueRight_SAMPLE[i];
  speedValueRight = 255254.4 / (speedValueRight_SAMPLE[10] / 10);
  car.setSpeedRight(speedValueRight);
  i = (i + 1) % 10;
  currentEncoderRight = TCNT1;
}

//--------------------------------------------------//
// cac ham o day

void go_head(int speed){

  car.setMotorLeft(speed, 1);
  car.setMotorRight(speed,1);

}
void stop(){
  car.setMotorLeft(0, 1);
  car.setMotorRight(0, 1);
  delay(300);
}
void turn_right(){
  car.setMotorLeft(140, 1);
  car.setMotorRight(140, 0);
  delay(600);
}
void turn_left(){
  car.setMotorLeft(140, 0);
  car.setMotorRight(140, 1);
  delay(600);
}
void go_back(){
  car.setMotorLeft(135, 0);
  car.setMotorRight(135, 0);
}
int i=0;

//--------------------------------------------------//

void loop() {
  digitalWrite(trig, LOW);
  car.setSpeedLeft(speedValueLeft);
  car.setSpeedRight(speedValueRight);
  car.configureSpeed(speedValueLeft, speedValueRight);

  // go_head(150);
  // delay(1000);
  // turn_left();
  //--------------------------------------------------//
  // code in here
  go_head(150);
    if (car.getDistanceHead() < 50) {
      if(car.getDistanceHead()<=5){
        go_back();
        i=0;
      }
      else if (car.getDistanceHead() < 15 && car.getDistanceHead()>5) {
        
        if(car.getDistanceRight()> car.getDistanceLeft() && car.getDistanceRight()>20){
            
            turn_right();
            go_head(130);
            i=0;
            }
        else if(car.getDistanceLeft()>car.getDistanceRight() && car.getDistanceLeft()>20){
          turn_left();
          go_head(130);
          i=0;        
          }
        else {     
          // go_back();     
          turn_left();
          go_head(130);
          i=0;
          }
        }
      else {
        go_head(130);
        }
      }
    else {
      go_head(140);
      i=0;
    }
  if(car.getSpeedRight() <= 2 || car.getSpeedLeft() <= 2){
    if (i>1){
      car.setMotorLeft(130, 0);
      car.setMotorRight(160, 0);
      delay(200);
      if (car.getDistanceRight() > car.getDistanceLeft()){
        car.setMotorLeft(135, 0);
        car.setMotorRight(110, 0);
        delay(600);
        i=0;
      } else if (car.getDistanceRight() < car.getDistanceLeft()){
        car.setMotorLeft(130, 0);
        car.setMotorRight(160, 0);
        delay(600);
        i=0;
      }
      
    } 

  }

  //--------------------------------------------------//


  // Su dung serial monitor de hien thi cac giac tri len man hinh
  //--------------------------------------------------//
  // code in here
  // Serial.print(car.getSpeedLeft());
  // Serial.print(" ");
  // Serial.print(car.getSpeedRight());
  // Serial.print(" ");
  // Serial.print(car.getDistanceHead());
  // Serial.print(" ");
  // Serial.print(car.getDistanceRight());
  // Serial.print(" ");
  // Serial.println(car.getDistanceLeft());
  
  //--------------------------------------------------//
}