#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 230400 // 보드레이트 230400

#define X_DIR     5       // X axis, direction pin
#define Y_DIR     6       // Y
#define Z_DIR     7       // Z
#define A_DIR     12       // A

#define X_STP     2       // X axis, step pin
#define Y_STP     3       // Y
#define Z_STP     4       // Z
#define A_STP     13       // A

char data;

// define motor
AccelStepper stepper_1(HALFSTEP, 8, 10, 9, 11); // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper stepper1(AccelStepper::DRIVER, X_STP, X_DIR); // X motor
AccelStepper stepper2(AccelStepper::DRIVER, Y_STP, Y_DIR); // Y motor 
AccelStepper stepper3(AccelStepper::DRIVER, Z_STP, Z_DIR); // Z motor
AccelStepper stepper4(AccelStepper::DRIVER, A_STP, A_DIR); // A motor


void setup(){
  Serial.begin(BAUD_RATE);
  Serial.println("보낸다 시리얼을");
  stepper_1.setMaxSpeed(1000); // 28 test motor
  stepper_1.setAcceleration(200);
  
  stepper_1.setMaxSpeed(1000); // x motor
  stepper_1.setAcceleration(5000);
  stepper2.setMaxSpeed(1000); // y motor
  stepper2.setAcceleration(5000);
  stepper3.setMaxSpeed(1000); // z motor
  stepper3.setAcceleration(5000);
  stepper4.setMaxSpeed(1000); // a motor
  stepper4.setAcceleration(5000);
  
}

void loop(){

  while(Serial.available()){
    data = Serial.read();
  }
  
  if(data == 'x'){ // JOG X_ Up Button 왜 다시 안 돌아오는 거야..?
   stepper_1.moveTo(200);
   stepper_1.runToPosition();
   data = 0; }
  else if ( data == 'b'){ // JOG X_Down Button
   stepper_1.moveTo(-100);
   stepper_1.runToPosition();
   data = 0; }
  else if ( data == 'y'){ // JOG Y_ Up Button
   stepper2.moveTo(100);
   stepper2.runToPosition();
   data = 0; }
  else if ( data == 'c'){ // JOG Y_Down Button
   stepper2.moveTo(-100);
   stepper2.runToPosition();
   data = 0; }
  else if ( data == 'z'){ // JOG Z_ Up Button
   stepper3.moveTo(100);
   stepper3.runToPosition();
   data = 0; }
  else if ( data == 'd'){ // JOG Z_ Down Button
   stepper3.moveTo(-100);
   stepper3.runToPosition();
   data = 0; }
  else if ( data == 'a'){ // JOG A_ Up Button
   stepper4.moveTo(100);
   stepper4.runToPosition();
   data = 0; }
  else if ( data == 'e'){ // JOG A_ Down Button
   stepper4.moveTo(-100);
   stepper4.runToPosition();
   data = 0; }
    
  

}
