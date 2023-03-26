/// Python과 데이터 송수신 처리
/// ---------------------------------------📱 Python -> Arduino 📲----------------------------------------------
/// Python에서 평상시 print("\n") 형식으로 값을 전달하게 됨.
/// G코드 실행시에는 print("<G>\n") 형식으로 전달.
/// Ex) Xup\n                       ->          Xup
/// Ex) <G> X10 Y20 F1000\n         ->          X10Y20F1000
/// 아두이노는 이를 getDataFromPython() 함수에서 \n 가 있을 때 까지 값을 받아옴. (\n 이전 까지는 계속 한 문자씩 저장)
/// 그 후 <G> 가 있다면 G코드 함수를 실행, 아니라면 auto_mode 를 통한 값 처리.
/// ---------------------------------------📱 Arduino -> Python 📲----------------------------------------------
/// Arduino에서는 Serial.println("<c>"); 형식으로 콘솔에 출력.
/// 형태들 [ <c> : 콘솔 출력, <d> : display에서 출력, <g> G코드가 실행중일때 G코드한줄의 완성을 알림. ]
/// <d>는 규정이 존재 :: "축" 이라는 단어가 들어가야 하며, "축" 앞에는 표시할 축을 대문자로 표시해야함, ":" 또한 존재해야하며, ":" 뒤에는 현재 위치(currentPostion) 이 존재해야함.
/// Ex) <c>"Hello"               ->          Hello
/// Ex) <d>X축 :80               ->          80
/// Ex) <g>                      ->
#include <ezButton.h>
#include <AccelStepper.h>
#include <SoftwareSerial.h>

#include "AccelStepperControl.h"
#include "GCodeTranslator.h"
#include "CalculateAccelStepper.h"

#define HALFSTEP 8        //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 230400  // 보드레이트 230400 or 115200

#define X_STP 2   // X axis, step pin
#define Y_STP 3   // Y
#define Z_STP 4   // Z
#define A_STP 12  // A

#define X_DIR 5   // X axis, direction pin
#define Y_DIR 6   // Y
#define Z_DIR 7   // Z
#define A_DIR 13  // A

#define limitX 9  //Limit pins: X->9 , Y->10 , Z->11
#define limitY 10
#define limitZ 11

float steps = 10;  // JOG이동 시, 고정값 (step * 5) 1step per 1mm distance **

long StartTime = 0;
long MovementTime = 0;

bool isStopped = false;  // stop 인식

bool can_G = false;  //G코드 인식 값

double getG = 0;
double getX = 0;
double getY = 0;
double getZ = 0;
double getA = 0;
double getF = 1000;
double getM = 0;

bool doGCode = false;


// define motor
AccelStepper Test_Stepper(HALFSTEP, 8, 10, 9, 11);           // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper X_Stepper(AccelStepper::DRIVER, X_STP, X_DIR);  // X motor
AccelStepper Y_Stepper(AccelStepper::DRIVER, Y_STP, Y_DIR);  // Y motor
AccelStepper Z_Stepper(AccelStepper::DRIVER, Z_STP, Z_DIR);  // Z motor
AccelStepper A_Stepper(AccelStepper::DRIVER, A_STP, A_DIR);  // A motor

AccelStepperControl Steppers(X_Stepper, Y_Stepper, Z_Stepper, A_Stepper);

// define limite switch
ezButton LSwitchX(limitX);
ezButton LSwitchY(limitY);
ezButton LSwitchZ(limitZ);

GCodeTranslator GCode = GCodeTranslator();


void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  set_StepperSetting(X_Stepper, 10000, 550);  // 1200, 1250
  set_StepperSetting(Y_Stepper, 10000, 550);  // 2200 ,1800
  set_StepperSetting(Z_Stepper, 500, 500);    // 8000, 8000
  set_StepperSetting(A_Stepper, 500, 500);    // 500, 500

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

  /// 스위치 체크는 getDataFromPython()을 통해 AccelStepper들이 move가 정해졌을때 stop을 함으로써 움직이는것을 막음.
  switchPressedCheck();

  /// Display 표시, 매개변수는 ms마다 Display 표시를 함.
  showDisplayPosition(50);

  // //예시,, current_x = update_x * 10;은 목표 값 vs X모터의 현재 위치 값을 비교
  // if (!Steppers.isSteppersMove()) {
  //   if (can_G == true) {
  //     Serial.println((String) "<g>");
  //     can_G = false;
  //   }
  // }

  if (doGCode) {
    Steppers.runSpeed();
    if (Steppers.currentPos('x') >= getX) {
      Steppers.setSpeed('x', 0);
    }
    if (Steppers.currentPos('y') >= getY) {
      Steppers.setSpeed('y', 0);
    }

    if (Steppers.isStepperStop()) {
      Serial.println((String) "<g>");
      doGCode = false;
      set_StepperSetting(X_Stepper, 10000, 550);  // 1200, 1250
      set_StepperSetting(Y_Stepper, 10000, 550);  // 2200 ,1800
      set_StepperSetting(Z_Stepper, 500, 500);    // 8000, 8000
      set_StepperSetting(A_Stepper, 500, 500);    // 500, 500
    }

  } else {
    Steppers.run();
  }
}


/// =========================================================================================================================================================================
///                                                                     loop 초기 설정 코드
/// =========================================================================================================================================================================


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

void switchPressedCheck() {
  if (LSwitchX.isPressed() || LSwitchY.isPressed() || LSwitchZ.isPressed()) {
    Steppers.allStop();
    Serial.println(F("The limit switchButton: TOUCHED"));
  }
}

void showDisplayPosition(int microsecond) {
  if ((millis() - StartTime) >= microsecond) {
    StartTime = millis();
    Steppers.show_currentPos('x');
    Steppers.show_currentPos('y');
    Steppers.show_currentPos('z');
    Steppers.show_currentPos('a');
  }
}


/// =========================================================================================================================================================================
///                                                                     Python 상호 작용 코드 ( main )
/// =========================================================================================================================================================================

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
  if (GCode.isGCode()) {
    GCodeFunction();
  }
  Steppers.auto_mode(GCode.line);
}


/// =========================================================================================================================================================================
///                                                                     G Code 실행 관련 부분
/// =========================================================================================================================================================================
/*
   G코드 인식
*/
double time = 2;  // 도달하는 데 걸리는 시간
void GCodeFunction() {
  // CalculateAccelStepper calc = CalculateAccelStepper();
  getG = GCode.get('G');
  getX = GCode.get('X');
  getY = GCode.get('Y');
  getZ = GCode.get('Z');
  getA = GCode.get('A');
  getF = GCode.get('F');
  getM = GCode.get('M');
  Serial.println((String) "<c> G : " + getG + " X : " + getX + " Y : " + getY + " Z : " + getZ + " A : " + getA + " F : " + getF + " M : " + getM);

  double distribution = 0;
  if (getX > getY) {                               // 100 > 50
    double speed = getY / time;                    // 50 /10 = 5
    distribution = getX / getY;                    // 100 / 50 = 2
    Steppers.setSpeed('x', speed * distribution);  // 5 * 2 = 10
    Steppers.setSpeed('y', speed);
  } else {
    float speed = getX / time;                     // 1000 / 20 = 50
    distribution = getY / getX;                    // 20000 / 1000 = 20
    Steppers.setSpeed('x', speed);                 // 100
    Steppers.setSpeed('y', speed * distribution);  // 100
  }

  doGCode = true;

  // 최대 속도 및 가속도
  // float max_Speed = getF;         // mm/s
  // float max_Acceleration = getF;  // mm/s^2

  // // 이동 시간 계산
  // double timeToReachMaxSpeed = max_Speed / max_Acceleration;                                            // 최대 속도에 도달하는 데 걸리는 시간
  // double distanceToReachMaxSpeed = 0.5 * max_Acceleration * timeToReachMaxSpeed * timeToReachMaxSpeed;  // 최대 속도에 도달하기 위한 거리
  // double totalTime = 2 * timeToReachMaxSpeed + (distance - 2 * distanceToReachMaxSpeed) / max_Speed;    // 이동에 걸리는 총 시간 원래

  // double speedX = dx / totalTime;
  // double speedY = dy / totalTime;
  // double speedZ = dz / totalTime;

  // // X, Y, Z 축의 이동 가속도 계산
  // double accelerationX = (speedX / timeToReachMaxSpeed) * max_Acceleration;
  // double accelerationY = (speedY / timeToReachMaxSpeed) * max_Acceleration;
  // double accelerationZ = (speedZ / timeToReachMaxSpeed) * max_Acceleration;

  // Steppers.setSpeed('x', speedX);
  // Steppers.setSpeed('y', speedY);
  // Steppers.setSpeed('z', speedZ);

  // Steppers.setAccel('x', accelerationX);
  // Steppers.setAccel('y', accelerationY);
  // Steppers.setAccel('z', accelerationZ);

  // Steppers.moveTo('x', getX);
  // Steppers.moveTo('y', getY);
  // Steppers.moveTo('z', getZ);


  // lines(getX, getY);  // Call the line() function with the new coordinates
  can_G = true;
}
