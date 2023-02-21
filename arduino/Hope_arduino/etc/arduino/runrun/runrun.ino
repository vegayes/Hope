#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 115200 // 보드레이트 230400 or 115200

#define X_DIR     5       // X axis, direction pin
#define Y_DIR     6       // Y      
#define Z_DIR     7       // Z
#define A_DIR     13      // A

#define X_STP     2       // X axis, step pin
#define Y_STP     3       // Y
#define Z_STP     4       // Z
#define A_STP     12      // A

char data;

// define motor
AccelStepper stepper_1(HALFSTEP, 8, 10, 9, 11); // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper stepper1(AccelStepper::DRIVER, X_STP, X_DIR); // X motor
AccelStepper stepper2(AccelStepper::DRIVER, Y_STP, Y_DIR); // Y motor 
AccelStepper stepper3(AccelStepper::DRIVER, Z_STP, Z_DIR); // Z motor
AccelStepper stepper4(AccelStepper::DRIVER, A_STP, A_DIR); // A motor

AccelStepper steppers[4] = {stepper_1, stepper2, stepper3, stepper4};

signed long distances_to_go[4];
signed long targets_position[4];
signed long distances_left[4];



void setup(){
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  stepper_1.setMaxSpeed(1000); // 28 test motor
  stepper_1.setAcceleration(200);


  stepper1.setMaxSpeed(1000); // x motor
  stepper1.setAcceleration(2000);
  
  stepper2.setMaxSpeed(1000); // y motor
  stepper2.setAcceleration(2000);
  
  stepper3.setMaxSpeed(1000); // z motor
  stepper3.setAcceleration(2000);
  
  stepper4.setMaxSpeed(1000); // a motor
  stepper4.setAcceleration(2000);
}

void loop(){

  while(Serial.available()){
    data = Serial.read();
  
   if(data == 'x'){ // JOG X_ Up Button 
    stepper_1.move(2000);
    stepper_1.runToPosition();
    Serial.println((String)"Current X position:"+stepper_1.currentPosition());
    data = 0; }
   else if ( data == 'b'){ // JOG X_Down Button
    stepper_1.move(-2000);
    stepper_1.runToPosition();
    Serial.println((String)"Current X position:"+stepper_1.currentPosition());
    data = 0; }
   else if ( data == 'y'){ // JOG Y_Right Button
    stepper2.move(-2000);
    stepper2.runToPosition();
    Serial.println((String)"Current Y position:"+stepper2.currentPosition());
    data = 0; }
   else if ( data == 'c'){ // JOG Y_Left Button
    stepper2.move(2000);
    stepper2.runToPosition();
    Serial.println((String)"Current Y position:"+stepper2.currentPosition());
    data = 0; }
   else if ( data == 'z'){ // JOG Z_ Up Button
    stepper3.move(-2000);
    stepper3.runToPosition();
    Serial.println((String)"Current Z position:"+stepper3.currentPosition());
    data = 0; }
   else if ( data == 'd'){ // JOG Z_ Down Button
    stepper3.move(2000);
    stepper3.runToPosition();
    Serial.println((String)"Current Z position:"+stepper3.currentPosition());
    data = 0; }
   else if ( data == 'a'){ // JOG A_ Up Button
    stepper4.move(2000);
    stepper4.runToPosition();
    Serial.println((String)"Current A position:"+stepper4.currentPosition());
    data = 0; }
   else if ( data == 'e'){ // JOG A_ Down Button
    stepper4.move(-2000);
    stepper4.runToPosition();
    Serial.println((String)"Current A position:"+stepper4.currentPosition());
    data = 0; }
   else if ( data == 'f'){ // JOG AZRN Button  뭔가 for문 써서 줄이고 싶음. 그리고 serial.println 4개 묶어서 사용했으면,, 
    stepper_1.moveTo(0);
    stepper_1.runToPosition();
    stepper2.moveTo(0);
    stepper2.runToPosition();
    stepper3.moveTo(0);
    stepper3.runToPosition();
    stepper4.moveTo(0);  
    stepper4.runToPosition();
    Serial.println((String)"Current X position:"+stepper_1.currentPosition());
    Serial.println((String)"Current Y position:"+stepper2.currentPosition());
    Serial.println((String)"Current Z position:"+stepper3.currentPosition());
    Serial.println((String)"Current A position:"+stepper4.currentPosition());
    data = 0; 
   }
   else if ( data == 'p'){
    return stopAll();
//    stepper_1.stop();
//    stepper_1.disableOutputs();
//    stepper2.stop();
//    stepper3.stop();
//    stepper4.stop();
//    data = 0; 
   }
} }


 void stopAll() {
  for (int i = 0; i < 3; i += 1) {
    steppers[i].stop();
  } }

//void data(){
//  char * strtokIndx; // start ok index
//
//  str 
// }
// }
