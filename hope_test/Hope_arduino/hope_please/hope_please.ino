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
float steps = 50; // JOG이동 시, 고정값 (step * 5) 1step per 1mm distance 그러면 G코드 라고 생각하면 ??

// define motor
AccelStepper stepper1(HALFSTEP, 8, 10, 9, 11); // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper stepper_1(AccelStepper::DRIVER, X_STP, X_DIR); // X motor
AccelStepper stepper2(AccelStepper::DRIVER, Y_STP, Y_DIR); // Y motor 
AccelStepper stepper3(AccelStepper::DRIVER, Z_STP, Z_DIR); // Z motor
AccelStepper stepper4(AccelStepper::DRIVER, A_STP, A_DIR); // A motor

AccelStepper steppers[4] = {stepper1, stepper2, stepper3, stepper4};

void setup(){
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  stepper_1.setMaxSpeed(1000); // 28 test motor
  stepper_1.setAcceleration(2000);


  stepper1.setMaxSpeed(2000); // x motor
  stepper1.setAcceleration(5000);
  
  stepper2.setMaxSpeed(2000); // y motor
  stepper2.setAcceleration(5000);
  
  stepper3.setMaxSpeed(2000); // z motor
  stepper3.setAcceleration(5000);
  
  stepper4.setMaxSpeed(2000); // a motor
  stepper4.setAcceleration(5000);
}

void loop(){
    while(Serial.available()){
      data = Serial.read(); 
      if(data == 'x'){ // JOG X_ Up Button 
         if(stepper1.distanceToGo() == 0 ){
          stepper1.move(2500);    // ★ 원래  steps*5
          data = 0;
         } 
      }
      else if(data == 'b'){ // JOG X_Down Button
        if(stepper1.distanceToGo() == 0 ){
          stepper1.move(-2500);  // ★ 원래  -steps*5
          data = 0;
        }
      }
      else if(data == 'y'){ // JOG Y_Right Button
        if(stepper2.distanceToGo() == 0 ){
          stepper2.move(-steps*5);
          data = 0;
        }
      }
      else if(data == 'c'){ // JOG Y_Left Button
        if(stepper2.distanceToGo() == 0 ){
          stepper2.move(steps*5);
          data = 0;
        }
      }
      else if(data == 'z'){ // JOG Z_ Up Button
        if(stepper3.distanceToGo() == 0 ){
          stepper3.move(-steps*5);
          data = 0;
        }
      }
      else if(data == 'd'){ // JOG Z_ Down Button
        if(stepper3.distanceToGo() == 0 ){
          stepper3.move(steps*5);
          data = 0;
        }
      }
      else if(data == 'a'){ // JOG A_ Up Button  
        if(stepper4.distanceToGo() == 0 ){
          stepper4.move(steps*5); // 각도 값 다시 계산
          data = 0;
        }
      }
      else if(data == 'e'){ // JOG A_ Down Button
        if(stepper4.distanceToGo() == 0 ){
          stepper4.move(-steps*5);
          data = 0;
        }
      }
      else if ( data == 'f'){ // JOG AZRN Button  뭔가 for문 써서 줄이고 싶음. 그리고 serial.println 4개 묶어서 사용했으면,, 
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
      else if (data == 'p'){
//          for(int i = 0; i <4; i +=1){
//            steppers[i].stop();
//          }
         stepper1.stop();
         stepper2.stop();
         stepper3.stop();
         stepper4.stop();         
      }
//      else{ // G 코드 파일이랑 연동시키기 (class연결?!??!!?!?!?!식으로!?!!!!!!!!!!)
//        
//        Serial.println(data);
//          data =0;
//      }
      
    }
      stepper1.run();
      stepper2.run();
      stepper3.run();
      stepper4.run();
}
