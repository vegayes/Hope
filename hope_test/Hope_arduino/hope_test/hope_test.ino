#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)

#define BAUD_RATE 230400 // 보드레이트 

// 28 (4output motor) test
#define mtrPin1  8     // IN1 on the ULN2003 driver 1
#define mtrPin2  9     // IN2 on the ULN2003 driver 1
#define mtrPin3  10     // IN3 on the ULN2003 driver 1
#define mtrPin4  11     // IN4 on the ULN2003 driver 1

AccelStepper Xaxis(1, 2, 5); // pin 2 = step, pin 5 = direction
//AccelStepper Yaxis(1, 3, 6); // pin 3 = step, pin 6 = direction
//AccelStepper Zaxis(1, 4, 7); // pin 4 = step, pin 7 = direction
//AccelStepper Aaxis(1, 9, 10); //이거 맞음? pin 더 찾기 

AccelStepper stepper1(HALFSTEP, mtrPin1, mtrPin3, mtrPin2, mtrPin4);

void setup() {
  Serial.begin(BAUD_RATE);
  stepper1.setMaxSpeed(10); // 0.9도 기준 9도 
  Xaxis.setMaxSpeed(400); // 한바퀴
//  Yaxis.setMaxSpeed(400);
//  Zaxis.setMaxSpeed(400);
  stepper1.setSpeed(45);
  Xaxis.setSpeed(45);
//  Yaxis.setSpeed(25);
//  Zaxis.setSpeed(80);
  stepper1.setAcceleration(5000); // Accel : X steps per second per second

  
}

void loop() {  
   stepper1.runSpeed();
//   Xaxis.runSpeed();
//   Yaxis.runSpeed();
//   Zaxis.runSpeed();

  getDataFromPC();


}


void getDataFromPC() {
  if (Serial.available() > 0) {



}

void jog_x() {


  
}

//AccelStepper stepper1(HALFSTEP, mtrPin1, mtrPin3, mtrPin2, mtrPin4);
//
//
//int speedLeft = 500;
//int speedRight = 500;
//
//int leftWheelForeward = 1;
//int leftWheelBackward = -1;
//int rightWheelForeward = -1;
//int rightWheelBackward = 1;
//
//char controlKey = 1;
//
//void setup() {
//  Serial.begin(9600);
//  stepper1.setMaxSpeed(2000.0);
//  stepper2.setMaxSpeed(2000.0);
//}
//
//
//
//void loop() {
//
//  switch (controlKey) {
//    case 'w': //전진
//      _Go();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case 's': //후진
//      _Back();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case 'a': //좌회전 전진
//      _Left();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case 'd': //우회전 전진
//      _Right();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case 'q': //왼쪽 제자리 회전
//      _Left360();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case 'e': //오른쪽 제자리 회전
//      _Right360();
//      Serial.println(controlKey);
//      //controlKey = '0';
//      break;
//    case '1': //정지
//      stepper1.stop(); //motor stop
//      stepper2.stop();
//      stepper1.disableOutputs(); //motor power disconnect, so motor led will turn off
//      stepper2.disableOutputs();
//      controlKey = '0';
//      Serial.println(controlKey);
//      break;
//    default :
//      controlKey = Serial.read();
//      break;
//  }
//}
//
//void _Go() {  //foreward
//  for (;;) {
//    stepper1.move(leftWheelForeward);
//    stepper2.move(rightWheelForeward);
//    stepper1.setSpeed(500);
//    stepper2.setSpeed(500);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 's' || controlKey == 'a' || controlKey == 'd' || controlKey == '1' || controlKey == 'q' || controlKey == 'e')
//    {
//      return;
//    }
//  }
//}
//
//void _Back() { //Backward
//  for (;;) {
//    stepper1.move(leftWheelBackward);
//    stepper2.move(rightWheelBackward);
//    stepper1.setSpeed(500);
//    stepper2.setSpeed(500);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 'w' || controlKey == 'a' || controlKey == 'd' || controlKey == '1' || controlKey == 'q' || controlKey == 'e')
//    {
//      return;
//    }
//  }
//}
//
//void _Left() { //Left ForeWard
//  for (;;) {
//    stepper1.move(leftWheelForeward);
//    stepper2.move(rightWheelForeward);
//    stepper1.setSpeed(200);
//    stepper2.setSpeed(600);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 'w' || controlKey == 's' || controlKey == 'd' || controlKey == '1' || controlKey == 'q' || controlKey == 'e')
//    {
//      return;
//    }
//  }
//}
//
//void _Right() { //Right ForeWard
//  for (;;) {
//    stepper1.move(leftWheelForeward);
//    stepper2.move(rightWheelForeward);
//    stepper1.setSpeed(600);
//    stepper2.setSpeed(200);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 'w' || controlKey == 's' || controlKey == 'a' || controlKey == '1' || controlKey == 'q' || controlKey == 'e')
//    {
//      return;
//    }
//  }
//}
//
//void _Left360() { //Left Quick Trun
//  for (;;) {
//    stepper1.move(leftWheelBackward);
//    stepper2.move(rightWheelForeward);
//    stepper1.setSpeed(500);
//    stepper2.setSpeed(500);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 'w' || controlKey == 's' || controlKey == 'd' || controlKey == '1' || controlKey == 'a' || controlKey == 'e')
//    {
//      return;
//    }
//  }
//}
//
//void _Right360() { //Right Quick Trun
//  for (;;) {
//    stepper1.move(leftWheelForeward);
//    stepper2.move(rightWheelBackward);
//    stepper1.setSpeed(500);
//    stepper2.setSpeed(500);
//    stepper1.runSpeedToPosition();
//    stepper2.runSpeedToPosition();
//    //delay(10);
//    controlKey = Serial.read();
//    if (controlKey == 'w' || controlKey == 's' || controlKey == 'd' || controlKey == '1' || controlKey == 'q' || controlKey == 'a')
//    {
//      return;
//    }
//  }
//}
