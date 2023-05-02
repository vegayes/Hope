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
#include <WString.h>

#include "AccelStepperControl.h"
#include "GCodeTranslator.h"
#include "Drawing.h"

#define THALFSTEP 8       //Half-step mode (8 step control signal sequence)
#define BAUD_RATE 115200  // 보드레이트 230400 or 115200

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

long StartTime = 0;

double getG;
double getX;
double getY;
double getZ;
double getA;
double getF = 1000;
double getM;
double getP;

double maxMove;                         // x가 500이므로
double xr, yr, zr, ar;                  // 처음 실행전 x y z 의 위치
double xp, yp, zp, ap;                  // 비율
double speedX, speedY, speedZ, speedA;  //우리 모터는 1초에 400 이 한바퀴 이므로 2초 간다 생각하고 maxMove / 2 = 250;
double maxSpeed;
double portion;
double i = 0;
bool xstop = false;
bool ystop = false;
bool zstop = false;
bool astop = false;
int xdir, ydir, zdir, adir;
double PreAcalibration = 0;  // 이전 보정 값
double Acalibration = 0;     // 현재 보정 값
double preA = 0;             // 이전 A값

// define motor
AccelStepper Test_Stepper(THALFSTEP, 8, 10, 9, 11);             // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper X_Stepper(AccelStepper::FULL2WIRE, X_STP, X_DIR);  // X motor
AccelStepper Y_Stepper(AccelStepper::FULL2WIRE, Y_STP, Y_DIR);  // Y motor
AccelStepper Z_Stepper(AccelStepper::FULL2WIRE, Z_STP, Z_DIR);  // Z motor
AccelStepper A_Stepper(AccelStepper::FULL2WIRE, A_STP, A_DIR);  // A motor

AccelStepperControl Steppers(X_Stepper, Y_Stepper, Z_Stepper, A_Stepper);

// define limite switch
ezButton LSwitchX(limitX);
ezButton LSwitchY(limitY);
ezButton LSwitchZ(limitZ);

GCodeTranslator GCode = GCodeTranslator();
Drawing draw = Drawing();

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("시리얼을 보냄....");
  set_StepperSetting(X_Stepper, 2500, 5000);  // 1200, 1250
  set_StepperSetting(Y_Stepper, 2500, 5000);  // 2200 ,1800  // 2500, 1250(1000) // 2500, 5000
  set_StepperSetting(Z_Stepper, 2500, 1000);  // 8000, 8000
  set_StepperSetting(A_Stepper, 2500, 1000);  // 500, 500

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

  // maxMove = 500;                  // x가 500이므로
  // xr = -1000, yr = -5000, zr = 300;  // 처음 실행전 x y z 의 위치
  // xp = 1/5, yp = 1, zp = 3/5;         // 비율
  // i = 0;

  if (Steppers.doGCode == false) {
    Steppers.run();
  } else {
    if (xdir == 1) {
      if ((long)((xp * i) * xdir + xr) > Steppers.currentPos('x')) {
        Steppers.runSpeed('x');
      } else {
        xstop = true;
      }
    } else if (xdir == -1) {
      if ((long)((xp * i) * xdir + xr) < Steppers.currentPos('x')) {
        Steppers.runSpeed('x');
      } else {
        xstop = true;
      }
    }

    if (ydir == 1) {
      if ((long)((yp * i) * ydir + yr) > Steppers.currentPos('y')) {
        Steppers.runSpeed('y');
      } else {
        ystop = true;
      }
    } else if (ydir == -1) {
      if ((long)((yp * i) * ydir + yr) < Steppers.currentPos('y')) {
        Steppers.runSpeed('y');
      } else {
        ystop = true;
      }
    }

    if (zdir == 1) {
      if ((long)((zp * i) * zdir + zr) > Steppers.currentPos('z')) {
        Steppers.runSpeed('z');
      } else {
        zstop = true;
      }
    } else if (zdir == -1) {
      if ((long)((zp * i) * zdir + zr) < Steppers.currentPos('z')) {
        Steppers.runSpeed('z');
      } else {
        zstop = true;
      }
    }

    if (adir == 1) {
      if ((long)(ap * i + ar) > Steppers.currentPos('a')) {
        Steppers.runSpeed('a');
        if (!isnan(round((PreAcalibration - Acalibration) / maxMove * i))) {
          Steppers.NowAcalibration = round((PreAcalibration - Acalibration) / maxMove * i);
        }
      } else {
        astop = true;
      }
    } else if (adir == -1) {
      if ((long)((ap * i) * adir + ar) < Steppers.currentPos('a')) {
        Steppers.runSpeed('a');
        if (!isnan(round((((PreAcalibration - Acalibration) / maxMove) * i) - PreAcalibration))) {
          Steppers.NowAcalibration = round((((PreAcalibration - Acalibration) / maxMove) * i) - PreAcalibration);
        }
      } else {
        astop = true;
      }
    }
    // Serial.println((String) "<c> i : " + i);
    // Serial.println((String) "<c> xp : " + xp + " xPos : " + Steppers.currentPos('x') + " xSpeed" + Steppers.getSpeed('x'));
    // Serial.println((String) "<c> yp : " + yp + " yPos : " + Steppers.currentPos('y') + " ySpeed" + Steppers.getSpeed('y'));
    // Serial.println((String) "<c> zp : " + zp + " zPos : " + Steppers.currentPos('z') + " zSpeed" + Steppers.getSpeed('z'));
    if (xstop && ystop && zstop && astop) {
      if (i < maxMove) {
        i++;
        xstop = false;
        ystop = false;
        zstop = false;
        astop = false;
        // Serial.println((String) " Steppers.NowAcalibration : " + Steppers.NowAcalibration);
        // Serial.println((String) "<c> ap : " + ap);
        // Serial.println((String) "<c> i : " + i);
        // Serial.println((String) " posA : " + (Steppers.currentPos('a') + Steppers.NowAcalibration));
        // Serial.println((String) " NowAc : " + Steppers.NowAcalibration);
        // Serial.println((String) " posX : " + Steppers.currentPos('x') + " xp * i : " + xp * i);

      } else {
        i = 0;
        PreAcalibration = Acalibration;
        Steppers.doGCode = false;
        Steppers.setSpeed('x', 0);
        Steppers.setSpeed('y', 0);
        Steppers.setSpeed('z', 0);
        Steppers.setSpeed('a', 0);
        Serial.println((String) "<g>");
        // Serial.println((String) " Steppers.NowAcalibration : " + Steppers.NowAcalibration + " maxMove : " + maxMove + " i : " + i);
      }
    }
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
    Steppers.show_AcurrentPos();
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
      GCode.RemoveCommentSeparators();
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

  if (Steppers.isNewFileOpen) {
    reset_variable();
  }
}

void reset_variable() {
  maxMove = 0;                                     // x가 500이므로
  xr = 0, yr = 0, zr = 0, ar = 0;                  // 처음 실행전 x y z 의 위치
  xp = 0, yp = 0, zp = 0, ap = 0;                  // 비율
  speedX = 0, speedY = 0, speedZ = 0, speedA = 0;  //우리 모터는 1초에 400 이 한바퀴 이므로 2초 간다 생각하고 maxMove / 2 = 250;
  maxSpeed = 0;
  portion = 0;
  i = 0;
  xstop = false;
  ystop = false;
  zstop = false;
  astop = false;
  xdir = 0, ydir = 0, zdir = 0, adir = 0;
  PreAcalibration = 0;  // 이전 보정 값
  Acalibration = 0;     // 현재 보정 값
  preA = 0;             // 이전 A값

  Steppers.isNewFileOpen = false;
}


/// =========================================================================================================================================================================
///                                                                     G Code 실행 관련 부분
/// =========================================================================================================================================================================
/*
   G코드 인식
*/
double time = 0.2;  // 도달하는 데 걸리는 시간
void GCodeFunction() {
  // CalculateAccelStepper calc = CalculateAccelStepper();
  getG = GCode.get('G').equals("") ? 0 : GCode.get('G').toDouble();
  getX = GCode.get('X').equals("") ? Steppers.currentPos('x') : GCode.get('X').toDouble();
  getY = GCode.get('Y').equals("") ? Steppers.currentPos('y') : GCode.get('Y').toDouble();
  getZ = GCode.get('Z').equals("") ? Steppers.currentPos('z') : GCode.get('Z').toDouble();
  getA = GCode.get('A').equals("") ? (Steppers.currentPos('a') - Acalibration) / 4: GCode.get('A').toDouble() / GCode.motion_calibration;
  getF = GCode.get('F').equals("") ? getF : GCode.get('F').toDouble();
  getM = GCode.get('M').equals("") ? 0 : GCode.get('M').toDouble();
  getP = GCode.get('P').equals("") ? 0 : GCode.get('P').toDouble();

  Serial.println(GCode.line);
  Serial.println((String) "<c> G : " + getG + " X : " + getX + " Y : " + getY + " Z : " + getZ + " A : " + getA + " F : " + getF + " M : " + getM);

  if (getG == 0 || getG == 1) {
    xr = Steppers.currentPos('x');
    yr = Steppers.currentPos('y');
    zr = Steppers.currentPos('z');
    ar = Steppers.currentPos('a');  // 보정값이 들어간 변수임.

    // da 의 기능은 받은 값 (getA) [보정 안받음] 와 현재 모터의 위치[보정()받음] 차이 만큼을 나타내는 변수임
    double APulseDir = getA - preA > 0 ? 1 : -1;  // 90 + 0 - 0 = 90, 45 - (100 - 10) = -45
    preA = getA;
    double APulse = (getA / 90) * 400;   // 90 -> 400, 45 -> 180, 0 -> 0
    Acalibration = APulse - (getA * 4);  // 360 - , 45 -> 5, 0 -> 0

    double dx = getX - xr;
    double dy = getY - yr;
    double dz = getZ - zr;
    double da = ((getA * 4) + Acalibration) - ar;  // 1. (90 + 10) - 0 = 100 || 2. (45 + 5) - 100 = -50 || 3. (0 + 0) -50 = 50 || 4. (90 + 10) - 0 = 100
    Serial.println((String) "<c> dx : " + dx + " dy : " + dy + " dz : " + dz + " da : " + da + " ar : " + ar + " Apulse : " + APulse + " Acalibration : " + Acalibration);

    maxMove = max(abs(dx), max(abs(dy), max(abs(dz), abs(da))));  // *2 하기. 3000은 2곱하기
    xdir = dx >= 0 ? 1 : -1;
    ydir = dy >= 0 ? 1 : -1;
    zdir = dz >= 0 ? 1 : -1;
    adir = da >= 0 ? 1 : -1;
    speedX = dx / time;
    speedY = dy / time;
    speedZ = dz / time;
    speedA = da / time;
    maxSpeed = max(abs(speedX), max(abs(speedY), max(abs(speedZ), abs(speedA))));
    portion = maxSpeed / max(Steppers.getMaxSpeed('x'), max(Steppers.getMaxSpeed('y'), max(Steppers.getMaxSpeed('z'), Steppers.getMaxSpeed('a'))));
    xp = abs(dx) / maxMove;
    yp = abs(dy) / maxMove;
    zp = abs(dz) / maxMove;
    ap = abs(da) / maxMove;  // 1. 90 + 10 = 100 || 2. -45 -5 = -50 || 3. -45 -5 = -50 ||

    Steppers.setSpeed('x', speedX / portion);
    Steppers.setSpeed('y', speedY / portion);
    Steppers.setSpeed('z', speedZ / portion);
    Steppers.setSpeed('a', speedA / portion);
    Serial.println((String) "<c> Xspeed : " + speedX / portion + " Yspeed : " + speedY / portion + " Zspeed : " + speedZ / portion + " Aspeed : " + speedA / portion + " NowAC : " + Steppers.NowAcalibration + " maxMove : " + maxMove + " PreAC : " + PreAcalibration);
  } else if (getG == 4) {
    delay(getP);
    Serial.println((String) "<g>");
    return;
  }
  if (getM == 30) {
    Steppers.allStop();

    set_StepperSetting(X_Stepper, 2500, 5000);
    set_StepperSetting(Y_Stepper, 2500, 5000);
    set_StepperSetting(Z_Stepper, 2500, 1000);
    set_StepperSetting(A_Stepper, 2500, 1000);
  }

  Steppers.doGCode = true;
}
