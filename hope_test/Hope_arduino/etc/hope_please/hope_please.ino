#include <ezButton.h>
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

#define limitX      9   //Limit pins: X->9 , Y->10 , Z->11
#define limitY      10
#define limitZ      11

char data;
float steps = 10; // JOG이동 시, 고정값 (step * 5) 1step per 1mm distance 그러면 G코드 라고 생각하면 ??


// define motor
AccelStepper stepper_1(HALFSTEP, 8, 10, 9, 11); // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper stepper1(AccelStepper::DRIVER, X_STP, X_DIR); // X motor
AccelStepper stepper2(AccelStepper::DRIVER, Y_STP, Y_DIR); // Y motor
AccelStepper stepper3(AccelStepper::DRIVER, Z_STP, Z_DIR); // Z motor
AccelStepper stepper4(AccelStepper::DRIVER, A_STP, A_DIR); // A motor

//AccelStepper steppers[4] = {stepper1, stepper2, stepper3, stepper4};

// define limite switch
ezButton limitSwitch_1(limitX);
ezButton limitSwitch_2(limitY);
ezButton limitSwitch_3(limitZ);

bool isStopped = false;  // stop 인식 

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  stepper_1.setMaxSpeed(500); // 28 test motor
  stepper_1.setAcceleration(800);


  stepper1.setMaxSpeed(5000); // x motor
  stepper1.setAcceleration(6000);

  stepper2.setMaxSpeed(2000); // y motor
  stepper2.setAcceleration(5000);

  stepper3.setMaxSpeed(7000); // z motor
  stepper3.setAcceleration(500);

  stepper4.setMaxSpeed(8000); // a motor
  stepper4.setAcceleration(8000);


  limitSwitch_1.setDebounceTime(50);
  limitSwitch_2.setDebounceTime(50);
//  limitSwitch_3.setDebounceTime(50);
}

void loop() {
  limitSwitch_1.loop();
  limitSwitch_2.loop();
  limitSwitch_3.loop();

//LimitSwitch_check
  if (limitSwitch_1.isPressed()) {
    Serial.println(F("The limit switch1: TOUCHED"));
    stepper1.stop();
    isStopped = true;
  }
  if (limitSwitch_2.isPressed()) {
    Serial.println(F("The limit switch2: TOUCHED"));
    stepper2.stop();
    isStopped = true;
  }
  if(limitSwitch_3.isPressed()) {
    Serial.println(F("The limit switch3: TOUCHED"));
    stepper3.stop();
    isStopped = true;
  }

  if (limitSwitch_1.isReleased()) {
    Serial.println("The button1 is released");
    isStopped = false;
  }
  if (limitSwitch_2.isReleased()) {
    Serial.println("The button2 is released");
    isStopped = false;
  }
  if (limitSwitch_3.isReleased()) {
    Serial.println("The button2 is released");
    isStopped = false;
  }

  if (isStopped == false) {
    while (Serial.available()) {
      data = Serial.read();
      if (data == 'x') { // JOG X_ Up Button
        if (stepper1.distanceToGo() == 0 ) {
          stepper1.move(2500);
          stepper_1.move(2500);
          Serial.println((String)"Current X position:" + (stepper1.currentPosition() + 2500)); // 한번 누를 때마다 2500이라는 위치 가짐
          data = 0;
        }
      }
      else if (data == 'b') { // JOG X_Down Button
        if (stepper1.distanceToGo() == 0 ) {
          stepper_1.move(-2500);
          stepper1.move(-2500);
          Serial.println((String)"Current X position:" + (stepper1.currentPosition() - 2500));
          data = 0;
        }
      }
      else if (data == 'y') { // JOG Y_Right Button
        if (stepper2.distanceToGo() == 0 ) {
          stepper2.move(-steps * 5);
          Serial.println((String)"Current Y position:" + (stepper2.currentPosition() - 250));
          data = 0;
        }
      }
      else if (data == 'c') { // JOG Y_Left Button
        if (stepper2.distanceToGo() == 0 ) {
          stepper2.move(steps * 5);
          Serial.println((String)"Current Y position:" + (stepper2.currentPosition() + 250));
          data = 0;
        }
      }
      else if (data == 'z') { // JOG Z_ Up Button
        if (stepper3.distanceToGo() == 0 ) {
          stepper3.move(-steps * 5);
          Serial.println((String)"Current Z position:" + (stepper3.currentPosition() + 250));
          data = 0;
        }
      }
      else if (data == 'd') { // JOG Z_ Down Button
        if (stepper3.distanceToGo() == 0 ) {
          stepper3.move(steps * 5);
          Serial.println((String)"Current Z position:" + (stepper3.currentPosition() - 250));
          data = 0;
        }
      }
      else if (data == 'a') { // JOG A_ Up Button
        if (stepper4.distanceToGo() == 0 ) { //4
          stepper4.move(1);
          Serial.println((String)"Current A position:" + (stepper4.currentPosition() * 0.9));
          data = 0;
        }
      }
      else if (data == 'e') { // JOG A_ Down Button
        if (stepper4.distanceToGo() == 0 ) {
          stepper4.move(-50);
          Serial.println((String)"Current A position:" + (stepper4.currentPosition() * 0.9));
          data = 0;
        }
      }
      else if ( data == 'f') { // JOG AZRN Button  뭔가 for문 써서 줄이고 싶음. 그리고 serial.println 4개 묶어서 사용했으면,,
        stepper1.moveTo(0);
        stepper1.runToPosition();
        stepper2.moveTo(0);
        stepper2.runToPosition();
        stepper3.moveTo(0);
        stepper3.runToPosition();
        stepper4.moveTo(0);
        stepper4.runToPosition();
        // ★ 위치 값 표시하기
        data = 0;
      }
      else if (data == 'p') {
        //          for(int i = 0; i <4; i +=1){
        //            steppers[i].stop();
        //          }
        stepper1.stop();
        stepper2.stop();
        stepper3.stop();
        stepper4.stop();
        //      Serial.println("Stop");
      }
      else { // G 코드 파일이랑 연동시키기 (class연결?!??!!?!?!?!식으로!?!!!!!!!!!!)
        data = 0;
      }
    }
    stepper_1.run();
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();
  }

}