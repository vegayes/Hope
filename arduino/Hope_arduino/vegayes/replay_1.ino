#include <ezButton.h>
#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8        //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 115200  // 보드레이트 230400 or 115200

#define X_DIR 5   // X axis, direction pin
#define Y_DIR 6   // Y
#define Z_DIR 7   // Z
#define A_DIR 13  // A

#define X_STP 2   // X axis, step pin
#define Y_STP 3   // Y
#define Z_STP 4   // Z
#define A_STP 12  // A

#define limitX 9  //Limit pins: X->9 , Y->10 , Z->11
#define limitY 10
#define limitZ 11

float steps = 10;  // JOG이동 시, 고정값 (step * 5) 1step per 1mm distance 그러면 G코드 라고 생각하면 ??

// define motor
AccelStepper Test_Stepper(HALFSTEP, 8, 10, 9, 11);           // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper X_Stepper(AccelStepper::DRIVER, X_STP, X_DIR);  // X motor
AccelStepper Y_Stepper(AccelStepper::DRIVER, Y_STP, Y_DIR);  // Y motor
AccelStepper Z_Stepper(AccelStepper::DRIVER, Z_STP, Z_DIR);  // Z motor
AccelStepper A_Stepper(AccelStepper::DRIVER, A_STP, A_DIR);  // A motor

AccelStepper *steppers[5] = { &Test_Stepper, &X_Stepper, &Y_Stepper, &Z_Stepper, &A_Stepper };

// define limite switch
ezButton LSwitchX(limitX);
ezButton LSwitchY(limitY);
ezButton LSwitchZ(limitZ);

bool isStopped = false;  // stop 인식


void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  set_StepperSetting(Test_Stepper, 5000, 8000);
  set_StepperSetting(X_Stepper, 5000, 8000);
  set_StepperSetting(Y_Stepper, 2000, 5000);
  set_StepperSetting(Z_Stepper, 7000, 500);
  set_StepperSetting(A_Stepper, 8000, 8000);

  LSwitchX.setDebounceTime(50);
  LSwitchY.setDebounceTime(50);
  // LSwitchZ.setDebounceTime(50);
}

void loop() {
  /*
    스위치가 눌렸는지 안눌렸는지 확인하기 위해 LSwitch.loop();를 호출해야함.
  */
  LSwitchX.loop();
  LSwitchY.loop();
  LSwitchZ.loop();


  getDataFromPython();
  if (LSwitchX.isPressed() || LSwitchY.isPressed() || LSwitchZ.isPressed()) {
    X_Stepper.stop();
    Test_Stepper.stop();
    Y_Stepper.stop();
    Z_Stepper.stop();
    A_Stepper.stop();
    Serial.println(F("The limit switchButton: TOUCHED"));
  }

  //  for (int i = 0; i < 5; i++) {
  //    steppers[i]->run();
  //  }
  X_Stepper.run();
  Y_Stepper.run();
}


/*
  모터 초기 세팅 함수
  @param stepper : 설정할 모터
         maxSpeed : 최대 속도
         accel : 가속도
*/
void set_StepperSetting(AccelStepper &stepper, int maxSpeed, int accel) {
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(accel);
}


/*
  파이썬으로부터 값을 받아오는 함수
*/
void getDataFromPython() {
  if (Serial.available() > 0) {
    char data = Serial.read();
    mainFunction(data);
  }
}

/*
  Python으로 부터 값을 받아온후 실행하게 될 함수.
*/
void mainFunction(char data) {
  switch (data) {
    case 'x':
      move_stepper(X_Stepper, 2500);
      move_stepper(Test_Stepper, 2500);
      print_position(X_Stepper, "X", X_Stepper.currentPosition() + 2500);
      //      print_position(Test_Stepper, "Test", Test_Stepper.currentPosition() + 2500);
      break;
    case 'b':
      move_stepper(X_Stepper, -2500);
      move_stepper(Test_Stepper, -2500);
      print_position(X_Stepper, "X", X_Stepper.currentPosition() - 2500);
      //      print_position(Test_Stepper, "Test", Test_Stepper.currentPosition() + 2500);
      break;
    case 'y':
      move_stepper(Y_Stepper, -2000);
      print_position(Y_Stepper, "Y", Y_Stepper.currentPosition() - 2000);
      break;
    case 'c':
      move_stepper(Y_Stepper, 2000);
      print_position(Y_Stepper, "Y", Y_Stepper.currentPosition() + 2000);
      break;
    case 'z':
      move_stepper(Z_Stepper, -200);
      print_position(Z_Stepper, "Z", Z_Stepper.currentPosition() + 200);
      break;
    case 'd':
      move_stepper(Z_Stepper, 200);
      print_position(Z_Stepper, "Z", Z_Stepper.currentPosition() - 200);
      break;
    case 'a':
      move_stepper(A_Stepper, 50);
      print_position(A_Stepper, "A", A_Stepper.currentPosition() + 50);// (stepper4.currentPosition() * 0.9)
      break; 
    case 'e':
      move_stepper(A_Stepper, -50);
      print_position(A_Stepper, "A", A_Stepper.currentPosition() - 50);
      break;
//    case 'f':
//        X_Stepper.moveTo(0);
//        X_Stepper.runToPosition();
//        print_position(X_Stepper, "X", X_Stepper.currentPosition());
//        Y_Stepper.moveTo(0);
//        Y_Stepper.runToPosition();
//        print_position(Y_Stepper, "Y", Y_Stepper.currentPosition());
//        Z_Stepper.moveTo(0);
//        Z_Stepper.runToPosition();
//        print_position(Z_Stepper, "Z", Z_Stepper.currentPosition());
//        A_Stepper.moveTo(0);
//        A_Stepper.runToPosition();
//        print_position(A_Stepper, "A", A_Stepper.currentPosition());
//        break;
     case 'o':
        X_Stepper.setCurrentPosition(0);
        print_position(X_Stepper, "X", X_Stepper.currentPosition());
        Y_Stepper.setCurrentPosition(0);
        print_position(Y_Stepper, "Y", Y_Stepper.currentPosition());
        Z_Stepper.setCurrentPosition(0);
        print_position(Z_Stepper, "Z", Z_Stepper.currentPosition());
        A_Stepper.setCurrentPosition(0);
        print_position(A_Stepper, "A", A_Stepper.currentPosition());
        
        break;

  }
}

void move_stepper(AccelStepper &stepper, long value) {
  if (stepper.distanceToGo() == 0) {
    stepper.move(value);
  }
}



// void print_position(AccelStepper &stepper, String axis) {
//   Serial.println((String) "현재 " + axis + "축 위치 :" + stepper.currentPosition());
// }

void print_position(AccelStepper &stepper, String axis, int value) {
  Serial.println((String) "현재 " + axis + "축 위치 :" + value);
}
