#include <GCodeParser.h>

#include <ezButton.h>
#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define HALFSTEP 8        //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 230400  // 보드레이트 230400 or 115200

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

float steps = 10;  // JOG이동 시, 고정값 (step * 5) 1step per 1mm distance 
long StartTime = 0;
bool isStopped = false;  // stop 인식

bool can_G = false;  //G코드 인식 값

// 초기 위치 설정
int current_x = 0;
int current_y = 0;
int current_z = 0;
int current_a = 0;


// define motor
AccelStepper Test_Stepper(HALFSTEP, 8, 10, 9, 11);           // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper X_Stepper(AccelStepper::DRIVER, X_STP, X_DIR);  // X motor
AccelStepper Y_Stepper(AccelStepper::DRIVER, Y_STP, Y_DIR);  // Y motor
AccelStepper Z_Stepper(AccelStepper::DRIVER, Z_STP, Z_DIR);  // Z motor
AccelStepper A_Stepper(AccelStepper::DRIVER, A_STP, A_DIR);  // A motor

//AccelStepper *steppers[5] = { &Test_Stepper, &X_Stepper, &Y_Stepper, &Z_Stepper, &A_Stepper };

// define limite switch
ezButton LSwitchX(limitX);
ezButton LSwitchY(limitY);
ezButton LSwitchZ(limitZ);

GCodeParser GCode = GCodeParser();


void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  //set_StepperSetting(AccelStepper &stepper, int Speed, int accel)
  set_StepperSetting(Test_Stepper, 500, 800);
  set_StepperSetting(X_Stepper, 500, 550); // 1200, 1250
  set_StepperSetting(Y_Stepper, 2200, 1800); // 2200 ,1800
  set_StepperSetting(Z_Stepper, 8000, 8000); // 8000, 8000
  set_StepperSetting(A_Stepper, 500, 500); // 500, 500

  LSwitchX.setDebounceTime(50);
  LSwitchY.setDebounceTime(50);
  LSwitchZ.setDebounceTime(50);

  StartTime = millis();
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

//  Test_Stepper.run();
  X_Stepper.run();
  Y_Stepper.run();
  Z_Stepper.run();
  A_Stepper.run();

  if ((millis() - StartTime) >= 25) {
    StartTime = millis();
    print_position("X", X_Stepper.currentPosition(), "<d>");
    print_position("Y", Y_Stepper.currentPosition(), "<d>");
    print_position("Z", Z_Stepper.currentPosition(), "<d>");
    print_position("A", A_Stepper.currentPosition(), "<d>");


    //예시,, current_x = update_x * 10;은 목표 값 vs X모터의 현재 위치 값을 비교
    if ((current_x == X_Stepper.currentPosition()) && (current_y == Y_Stepper.currentPosition()) && (current_z == Z_Stepper.currentPosition()) && (current_a == A_Stepper.currentPosition())) {
      if (can_G == true) {
        Serial.println((String) "<g>");
        can_G = false;
      }
    }
  }
}

/*
  모터 초기 세팅 함수
  @param stepper : 설정할 모터
         maxSpeed : 최대 속도
         accel : 가속도
*/

void set_StepperSetting(AccelStepper &stepper, int Speed, int accel) {
  stepper.setMaxSpeed(Speed);
  stepper.setAcceleration(accel);
}


/*
  파이썬으로부터 값을 받아오는 함수
*/

void getDataFromPython() {
  if (Serial.available() > 0) {
    if (GCode.AddCharToLine(Serial.read())) {
      mainFunction();
    }
  }
}

/*
  Python으로 부터 값을 받아온후 실행하게 될 함수.
*/
void mainFunction() {
  String temp(GCode.line);
  int index = temp.indexOf("<G>");
  if (index >= 0) {
    temp.replace("<G>", "");
    strcpy(GCode.line, temp.c_str());
    GCodeFunction();
    return;
  }
  String line = GCode.line;
  Serial.println(GCode.line);

  if (line == "x") {
    move_stepper(X_Stepper, 1000);
//    move_stepper(Test_Stepper, 500);
    print_position("X", X_Stepper.currentPosition() + 1000, "<c>");
//    print_position("Test", Test_Stepper.currentPosition() + 500, "<c>");
  } else if (line == "b") {
    move_stepper(X_Stepper, -1000);
//    move_stepper(Test_Stepper, -500);
    print_position("X", X_Stepper.currentPosition() - 1000, "<c>");
//    print_position("Test", Test_Stepper.currentPosition() - 500, "<c>");
  } else if (line == "y") {
    move_stepper(Y_Stepper, -1000);
    print_position("Y", Y_Stepper.currentPosition() - 1000, "<c>");
  } else if (line == "c") {
    move_stepper(Y_Stepper, 1000);
    print_position("Y", Y_Stepper.currentPosition() + 1000, "<c>");
  } else if (line == "z") {
    move_stepper(Z_Stepper, +50);
    print_position("Z", Z_Stepper.currentPosition() + 50, "<c>");
  } else if (line == "d") {
    move_stepper(Z_Stepper, -50);
    print_position("Z", Z_Stepper.currentPosition() - 50, "<c>");
  } else if (line == "line") {
    move_stepper(A_Stepper, 10);
    print_position("A", A_Stepper.currentPosition() + 10, "<c>");  // (stepper4.currentPosition() * 0.9)
  } else if (line == "e") {
    move_stepper(A_Stepper, -10);
    print_position("A", A_Stepper.currentPosition() - 10, "<c>");
  } else if (line == "f") {
    X_Stepper.moveTo(0);
    X_Stepper.runToPosition();
    print_position("X", X_Stepper.currentPosition(), "<c>");
    Y_Stepper.moveTo(0);
    Y_Stepper.runToPosition();
    print_position("Y", Y_Stepper.currentPosition(), "<c>");
    Z_Stepper.moveTo(0);
    Z_Stepper.runToPosition();
    print_position("Z", Z_Stepper.currentPosition(), "<c>");
    A_Stepper.moveTo(0);
    A_Stepper.runToPosition();
    print_position("A", A_Stepper.currentPosition(), "<c>");

    current_x = X_Stepper.currentPosition();
    current_y = Y_Stepper.currentPosition();
    current_z = Z_Stepper.currentPosition();
    current_a = A_Stepper.currentPosition();
  } else if (line == "o") {
    X_Stepper.setCurrentPosition(0);
    print_position("X", X_Stepper.currentPosition(), "<c>");
    Y_Stepper.setCurrentPosition(0);
    print_position("Y", Y_Stepper.currentPosition(), "<c>");
    Z_Stepper.setCurrentPosition(0);
    print_position("Z", Z_Stepper.currentPosition(), "<c>");
    A_Stepper.setCurrentPosition(0);
    print_position("A", A_Stepper.currentPosition(), "<c>");
  } else if (line == "p") {
    X_Stepper.stop();
    Y_Stepper.stop();
    Z_Stepper.stop();
    A_Stepper.stop();

    current_x = X_Stepper.currentPosition();
    current_y = Y_Stepper.currentPosition();
    current_z = Z_Stepper.currentPosition();
    current_a = A_Stepper.currentPosition();
  }
}


/*
   G코드 인식
*/
void GCodeFunction() {
  String line = GCode.line;
  Serial.println(GCode.line);
  if (GCode.HasWord('G')) {
    if (GCode.GetWordValue('G') == 0 || GCode.GetWordValue('G') == 1) {
      if (GCode.HasWord('X')) {
        float x_update = GCode.GetWordValue('X');
        X_Stepper.moveTo(x_update * 10);
        current_x = x_update * 10;
        Serial.println(x_update);
      }
      if (GCode.HasWord('Y')) {
        float y_update = GCode.GetWordValue('Y');
        Y_Stepper.moveTo(y_update * 10);
        current_y = y_update * 10;
        Serial.println(y_update);
      }
      if (GCode.HasWord('Z')) {
        float z_update = GCode.GetWordValue('Z');
        Z_Stepper.moveTo(z_update * 10);
        current_z = z_update * 10;
        Serial.println(z_update);
      }
      if (GCode.HasWord('A')) {
        float a_update = GCode.GetWordValue('A');
        A_Stepper.moveTo(a_update * 10);
        current_a = a_update * 10;
        Serial.println(a_update);
      }

      can_G = true;
    }
  }

  if (GCode.HasWord('M')) {
    if (GCode.GetWordValue('M') == 30 ) { // 끝냄.
      X_Stepper.stop();
      Y_Stepper.stop();
      Z_Stepper.stop();
      A_Stepper.stop();

      can_G = true;

      // speed를 원래 값으로 다시 재설정
      X_Stepper.setSpeed(1000);
      Y_Stepper.setSpeed(1200);
      Z_Stepper.setSpeed(8000);
      A_Stepper.setSpeed(2000);

      Serial.println("G_code END Line");
    }
  }
}

float getValue(String data, char separator) {
  String value = "";
  int separatorCount = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == separator) {
      separatorCount++;
    } else if (separatorCount == 1) {
      value += data[i];
    }
  }
  return value.toFloat();
}

void move_stepper(AccelStepper &stepper, long value) {
  if (stepper.distanceToGo() == 0) {
    stepper.move(value);
  }
}


// void print_position(AccelStepper &stepper, String axis) {
//   Serial.println((String) "현재 " + axis + "축 위치 :" + stepper.currentPosition());
// }

void print_position(String axis, int value, String type) {
  Serial.println((String)type + "현재 " + axis + "축 위치 :" + value);
}
