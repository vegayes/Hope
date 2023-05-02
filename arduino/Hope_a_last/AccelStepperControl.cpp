#include "AccelStepperControl.h"
#include "ctype.h"

AccelStepperControl::AccelStepperControl(AccelStepper& X_stepper, AccelStepper& Y_stepper, AccelStepper& Z_stepper, AccelStepper& A_stepper)
  : _X_stepper(X_stepper),
    _Y_stepper(Y_stepper),
    _Z_stepper(Z_stepper),
    _A_stepper(A_stepper) {

      doGCode = false;
      isNewFileOpen = false;
      NowAcalibration = 0;
}




bool AccelStepperControl::move(char axis, double pos) {

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

bool AccelStepperControl::moveTo(char axis, double pos) {

  // 소문자로 바꿈.
  axis = tolower(axis);

  switch (axis) {
    case 'x':
      if (_X_stepper.distanceToGo() == 0) {
        _X_stepper.moveTo(pos);
        return true;
      }
      return false;
    case 'y':
      if (_Y_stepper.distanceToGo() == 0) {
        _Y_stepper.moveTo(pos);
        return true;
      }
      return false;
    case 'z':
      if (_Z_stepper.distanceToGo() == 0) {
        _Z_stepper.moveTo(pos);
        return true;
      }
      return false;
    case 'a':
      if (_A_stepper.distanceToGo() == 0) {
        _A_stepper.moveTo(pos);
        return true;
      }
      return false;
  }
}

void AccelStepperControl::run() {
  _X_stepper.run();
  _Y_stepper.run();
  _Z_stepper.run();
  _A_stepper.run();
}

void AccelStepperControl::run(char axis) {
  axis = tolower(axis);
  if (axis == 'x') {
    _X_stepper.run();
  } else if (axis == 'y') {
    _Y_stepper.run();
  } else if (axis == 'z') {
    _Z_stepper.run();
  } else if (axis == 'a') {
    _A_stepper.run();
  }
}

void AccelStepperControl::runSpeed() {
  _X_stepper.runSpeed();
  _Y_stepper.runSpeed();
  _Z_stepper.runSpeed();
  _A_stepper.runSpeed();
}

void AccelStepperControl::runSpeed(char axis) {
  axis = tolower(axis);
  if (axis == 'x') {
    _X_stepper.runSpeed();
  } else if (axis == 'y') {
    _Y_stepper.runSpeed();
  } else if (axis == 'z') {
    _Z_stepper.runSpeed();
  } else if (axis == 'a') {
    _A_stepper.runSpeed();
  }
}

void AccelStepperControl::setSpeed(char axis, double speed) {

  axis = tolower(axis);

  if (axis == 'x') {
    _X_stepper.setSpeed(speed);
  } else if (axis == 'y') {
    _Y_stepper.setSpeed(speed);
  } else if (axis == 'z') {
    _Z_stepper.setSpeed(speed);
  } else if (axis == 'a') {
    _A_stepper.setSpeed(speed);
  }
}

void AccelStepperControl::setAccel(char axis, double accel) {

  axis = tolower(axis);

  if (axis == 'x') {
    _X_stepper.setAcceleration(accel);
  } else if (axis == 'y') {
    _Y_stepper.setAcceleration(accel);
  } else if (axis == 'z') {
    _Z_stepper.setAcceleration(accel);
  } else if (axis == 'a') {
    _A_stepper.setAcceleration(accel);
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

  _X_stepper.move(0);
  _Y_stepper.move(0);
  _Z_stepper.move(0);
  _A_stepper.move(0);

  _X_stepper.setSpeed(0);
  _Y_stepper.setSpeed(0);
  _Z_stepper.setSpeed(0);
  _A_stepper.setSpeed(0);
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
  NowAcalibration = 0;
  print_currentPos('a');
}

void AccelStepperControl::setPosition(char axis, long value) {
  axis = tolower(axis);

  if (axis == 'x') {
    _X_stepper.setCurrentPosition(value);
  } else if (axis == 'y') {
    _Y_stepper.setCurrentPosition(value);
  } else if (axis == 'z') {
    _Z_stepper.setCurrentPosition(value);
  } else if (axis == 'a') {
    _A_stepper.setCurrentPosition(value);
  }
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

  Serial.println((String) "<c>" + axis + "축은 현재 " + currentPos(axis) + "에 있습니다.");
}

void AccelStepperControl::show_currentPos(char axis) {

  axis = tolower(axis);

  Serial.println((String) "<d>" + (char)toupper(axis) + "축 :" + currentPos(axis));
}

// 오직 A 축 전용
void AccelStepperControl::show_AcurrentPos() {
  Serial.println((String) "<d>A축 :" + (currentPos('a') + NowAcalibration));
}

bool AccelStepperControl::isSteppersMove() {
  return (_X_stepper.distanceToGo() == 0 && _Y_stepper.distanceToGo() == 0 && _Z_stepper.distanceToGo() == 0 && _A_stepper.distanceToGo() == 0) ? false : true;
}

bool AccelStepperControl::isStepperStop() {
  return (_X_stepper.speed() == 0 && _Y_stepper.speed() == 0 && _Z_stepper.speed() == 0 && _A_stepper.speed() == 0) ? true : false;
}

double AccelStepperControl::getSpeed(char axis) {
  axis = tolower(axis);
  double speed;
  if (axis == 'x') {
    speed = _X_stepper.speed();
  } else if (axis == 'y') {
    speed = _Y_stepper.speed();
  } else if (axis == 'z') {
    speed = _Z_stepper.speed();
  } else if (axis == 'a') {
    speed = _A_stepper.speed();
  }
  return speed;
}

double AccelStepperControl::getMaxSpeed(char axis) {
  axis = tolower(axis);
  double maxSpeed;
  if (axis == 'x') {
    maxSpeed = _X_stepper.maxSpeed();
  } else if (axis == 'y') {
    maxSpeed = _Y_stepper.maxSpeed();
  } else if (axis == 'z') {
    maxSpeed = _Z_stepper.maxSpeed();
  } else if (axis == 'a') {
    maxSpeed = _A_stepper.maxSpeed();
  }
  return maxSpeed;
}

long AccelStepperControl::currentPos(char axis) {
  long currentPos = 0;
  if (axis == 'x') {
    currentPos = _X_stepper.currentPosition();
  } else if (axis == 'y') {
    currentPos = _Y_stepper.currentPosition();
  } else if (axis == 'z') {
    currentPos = _Z_stepper.currentPosition();
  } else if (axis == 'a') {
    currentPos = _A_stepper.currentPosition();
  }
  return currentPos;
}


void AccelStepperControl::auto_mode(String mode) {

  if (mode == "Xup") {
    move('x', 1000);
    print_ToGo('x');
  }

  if (mode == "Xdown") {
    move('x', -1000);
    print_ToGo('x');
  }

  if (mode == "Yup") {
    move('y', 1000);
    print_ToGo('y');
  }

  if (mode == "Ydown") {
    move('y', -1000);
    print_ToGo('y');
  }

  if (mode == "Zup") {
    move('z', 50);
    print_ToGo('z');
  }

  if (mode == "Zdown") {
    move('z', -50);
    print_ToGo('z');
  }

  if (mode == "Aup") {
    move('a', 10);
    print_ToGo('a');
  }

  if (mode == "Adown") {
    move('a', -10);
    print_ToGo('a');
  }

  if (mode == "AllZero") {
    home();
  }

  if (mode == "Zero Setting") {
    setPosition('x', 0);
    setPosition('y', 0);
    setPosition('z', 0);
    setPosition('a', 0);
    NowAcalibration = 0;
  }

  if (mode == "Stop") {
    doGCode = false;
    _X_speed = getSpeed('x');
    _Y_speed = getSpeed('y');
    _Z_speed = getSpeed('z');
    _A_speed = getSpeed('a');
    allStop();
  }

  if (mode == "Start") {
    allStop();
    move('x', 0);
    move('y', 0);
    move('z', 0);
    move('a', 0);
    setSpeed('x', _X_speed);
    setSpeed('y', _Y_speed);
    setSpeed('z', _Z_speed);
    setSpeed('a', _A_speed);
    doGCode = true;
  }

  if(mode == "NewFileOpen") {
    isNewFileOpen = true;
    doGCode = false;
    allStop();
  }
}
