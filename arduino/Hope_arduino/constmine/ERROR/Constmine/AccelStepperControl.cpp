#include "AccelStepperControl.h"
#include "ctype.h"

AccelStepperControl::AccelStepperControl(AccelStepper& X_stepper, AccelStepper& Y_stepper, AccelStepper& Z_stepper, AccelStepper& A_stepper)
  : _X_stepper(X_stepper),
    _Y_stepper(Y_stepper),
    _Z_stepper(Z_stepper),
    _A_stepper(A_stepper) {
}


bool AccelStepperControl::move(char axis, long pos) {

  // 소문자로 바꿈.
  axis = tolower(axis);

  switch (axis) {
    case 'x':
      if (_X_stepper.distanceToGo() == 0) {
        _X_stepper.move(pos);
        return true;
      }
      return false;
    case 'y':
      if (_Y_stepper.distanceToGo() == 0) {
        _Y_stepper.move(pos);
        return true;
      }
      return false;
    case 'z':
      if (_Z_stepper.distanceToGo() == 0) {
        _Z_stepper.move(pos);
        return true;
      }
      return false;
    case 'a':
      if (_A_stepper.distanceToGo() == 0) {
        _A_stepper.move(pos);
        return true;
      }
      return false;
  }
}

void AccelStepperControl::allStop() {
  if (_X_stepper.distanceToGo() != 0) {
    _X_stepper.stop();
  }
  if (_Y_stepper.distanceToGo() != 0) {
    _Y_stepper.stop();
  }
  if (_Z_stepper.distanceToGo() != 0) {
    _Z_stepper.stop();
  }
  if (_A_stepper.distanceToGo() != 0) {
    _A_stepper.stop();
  }
}

void AccelStepperControl::home() {
  _X_stepper.moveTo(0);
  _Y_stepper.moveTo(0);
  _Z_stepper.moveTo(0);
  _A_stepper.moveTo(0);
  _X_stepper.runToPosition();
  print_currentPos('x');
  _Y_stepper.runToPosition();
  print_currentPos('y');
  _Z_stepper.runToPosition();
  print_currentPos('z');
  _A_stepper.runToPosition();
  print_currentPos('a');
}

void AccelStepperControl::setPosition() {
  _X_stepper.setCurrentPosition(0);
  _Y_stepper.setCurrentPosition(0);
  _Z_stepper.setCurrentPosition(0);
  _A_stepper.setCurrentPosition(0);
}

void AccelStepperControl::print_ToGo(char axis) {

  axis = tolower(axis);

  if (axis == 'y') {
    long Y_CurrentPos = _Y_stepper.currentPosition();
    long Y_ToGoPos = _Y_stepper.distanceToGo();
    Serial.println((String) "<c>" + axis + "축은 현재 " + Y_CurrentPos + "에 있고, " + Y_ToGoPos + "만큼 가야합니다. (도착 위치 : " + (long)(Y_CurrentPos + Y_ToGoPos) + ")");
  } else if (axis == 'x') {
    long X_CurrentPos = _X_stepper.currentPosition();
    long X_ToGoPos = _X_stepper.distanceToGo();
    Serial.println((String) "<c>" + axis + "축은 현재 " + X_CurrentPos + "에 있고, " + X_ToGoPos + "만큼 가야합니다. (도착 위치 : " + (long)(X_CurrentPos + X_ToGoPos) + ")");
  } else if (axis == 'z') {
    long Z_CurrentPos = _Z_stepper.currentPosition();
    long Z_ToGoPos = _Z_stepper.distanceToGo();
    Serial.println((String) "<c>" + axis + "축은 현재 " + Z_CurrentPos + "에 있고, " + Z_ToGoPos + "만큼 가야합니다. (도착 위치 : " + (long)(Z_CurrentPos + Z_ToGoPos) + ")");
  } else if (axis == 'a') {
    long A_CurrentPos = _A_stepper.currentPosition();
    long A_ToGoPos = _A_stepper.distanceToGo();
    Serial.println((String) "<c>" + axis + "축은 현재 " + A_CurrentPos + "에 있고, " + A_ToGoPos + "만큼 가야합니다. (도착 위치 : " + (long)(A_CurrentPos + A_ToGoPos) + ")");
  }
}


void AccelStepperControl::print_currentPos(char axis) {

  axis = tolower(axis);

  if (axis == 'x') {
    long X_CurrentPos = _X_stepper.currentPosition();
    Serial.println((String) "<c>" + axis + "축은 현재 " + X_CurrentPos + "에 있습니다.");
  } else if (axis == 'y') {
    long Y_CurrentPos = _Y_stepper.currentPosition();
    Serial.println((String) "<c>" + axis + "축은 현재 " + Y_CurrentPos + "에 있습니다.");
  } else if (axis == 'z') {
    long Z_CurrentPos = _Z_stepper.currentPosition();
    Serial.println((String) "<c>" + axis + "축은 현재 " + Z_CurrentPos + "에 있습니다.");
  } else if (axis == 'a') {
    long A_CurrentPos = _A_stepper.currentPosition();
    Serial.println((String) "<c>" + axis + "축은 현재 " + A_CurrentPos + "에 있습니다.");
  }
}

void AccelStepperControl::show_currentPos(char axis) {

  axis = tolower(axis);

  if (axis == 'x') {
    long X_CurrentPos = _X_stepper.currentPosition();
    Serial.println((String) "<d>" + (char)toupper(axis) + "축 :" + X_CurrentPos);
  } else if (axis == 'y') {
    long Y_CurrentPos = _Y_stepper.currentPosition();
    Serial.println((String) "<d>" + (char)toupper(axis) + "축 :" + Y_CurrentPos);
  } else if (axis == 'z') {
    long Z_CurrentPos = _Z_stepper.currentPosition();
    Serial.println((String) "<d>" + (char)toupper(axis) + "축 :" + Z_CurrentPos);
  } else if (axis == 'a') {
    long A_CurrentPos = _A_stepper.currentPosition();
    Serial.println((String) "<d>" + (char)toupper(axis) + "축 :" + A_CurrentPos);
  }
}


void AccelStepperControl::auto_mode(String mode) {

  if(mode == "Xup") {
    move('x', 1000);
    print_ToGo('x');
  }

  if(mode == "Xdown") {
    move('x', -1000);
    print_ToGo('x');
  }

  if(mode == "Yup") {
    move('y', 1000);
    print_ToGo('y');
  }

  if(mode == "Ydown") {
    move('y', -1000);
    print_ToGo('y');
  }

  if(mode == "Zup") {
    move('z', 50);
    print_ToGo('z');
  }

  if(mode == "Zdown") {
    move('z', -50);
    print_ToGo('z');
  }

  if(mode == "Aup") {
    move('a', 10);
    print_ToGo('a');
  }

  if(mode == "Adown") {
    move('a', -10);
    print_ToGo('a');
  }

  if(mode == "AllZero") {
    home();
  }

  if(mode == "Zero Setting") {
    setPosition();
  }

  if(mode == "Stop") {
    allStop();
  }
}