#ifndef AccelStepperControl_h
#define AccelStepperControl_h

#include <AccelStepper.h>

class AccelStepperControl {
public:
  AccelStepperControl(AccelStepper& X_stepper, AccelStepper& Y_stepper, AccelStepper& Z_stepper, AccelStepper& A_stepper);

  bool doGCode;
  bool isNewFileOpen;
  double NowAcalibration;

  /// AccelStepper를 해당 축에서 pos의 위치만큼 move를 설정합니다.
  /// 만약 이미 설정된 move 값이 있다면 재설정 하지 않습니다.
  /// \param axis 움직여야하는 축 을 설정합니다.
  /// \param pos 해당 값 만큼 움직이게 됩니다.
  /// \return move 설정이 되었다면 true를 반환합니다.
  bool move(char axis, double pos);
  bool moveTo(char axis, double pos);

  /// X, Y, Z, A축의 모터를 run을 호출합니다.
  void run();
  void run(char axis);

  void runSpeed();
  void runSpeed(char axis);

  void setSpeed(char axis, double speed);

  void setAccel(char axis, double accel);

  /// X, Y, Z, A축 AccelStepper를 모두 멈춥니다.
  /// 바로 멈추지 않고, 가장 가까운 거리에서 멈출 수 있는 move값을 조정합니다.
  void allStop();

  /// X, Y, Z, A를 모두 초기 위치로 되돌립니다.
  /// X -> Y -> Z -> A 순으로 되돌아가며, 중간에 멈출 수 없습니다.
  void home();

  /// X, Y, Z, A를 모두 현재 위치를 초기 위치로 설정합니다.
  void setPosition(char axis, long value);

  /// 해당 축의 위치와 가야하는 만큼을 출력합니다.
  void print_ToGo(char axis);

  /// 해당 축의 현재 위치를 출력합니다.
  void print_currentPos(char axis);

  /// 해당 축의 현재 위치를 display 형식으로 전송하여 출력합니다. (Python에서 받아들이기 위함)
  void show_currentPos(char axis);
  // A축 출력 전용
  void show_AcurrentPos();

  ///  모든 AccelStepper들이 움직이고 있는 중이면 true, (움직임의 기준은 run이 아니라, distanceToGo 기준.)
  bool isSteppersMove();

  /// 모든 AccelStepper 들의 Speed가 0이면 true
  bool isStepperStop();

  double getSpeed(char axis);

  double getMaxSpeed(char axis);

  /// 해당 축의 현재 위치를 반환합니다.
  long currentPos(char axis);

  ///자동으로 mode 값에 따라서 모터의 동작상태를 조정합니다.
  void auto_mode(String mode);

private:
  AccelStepper& _X_stepper;
  AccelStepper& _Y_stepper;
  AccelStepper& _Z_stepper;
  AccelStepper& _A_stepper;
  double _X_speed;
  double _Y_speed;
  double _Z_speed;
  double _A_speed;
};

#endif
