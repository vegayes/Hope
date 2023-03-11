// G_code_Interpreter 가져오려다가 포기한 듯.

#include <AccelStepper.h>
#include <HardwareSerial.h>


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

// define motor
AccelStepper Test_Stepper(HALFSTEP, 8, 10, 9, 11);           // 28motor(8, IN1, IN3, IN2, In4)
AccelStepper X_Stepper(AccelStepper::DRIVER, X_STP, X_DIR);  // X motor
AccelStepper Y_Stepper(AccelStepper::DRIVER, Y_STP, Y_DIR);  // Y motor
AccelStepper Z_Stepper(AccelStepper::DRIVER, Z_STP, Z_DIR);  // Z motor
AccelStepper A_Stepper(AccelStepper::DRIVER, A_STP, A_DIR);  // A motor

AccelStepper *steppers[5] = { &Test_Stepper, &X_Stepper, &Y_Stepper, &Z_Stepper, &A_Stepper };


// our point structure to make things nice
//========================================
struct LongPoint {
  long x;
  long y;
  long z;
};

struct FloatPoint {
  float x;
  float y;
  float z;
};

// our command string
//===================
#define COMMAND_SIZE 128
char aWord[COMMAND_SIZE];

long serial_count;
long no_data = 0;

// used to decode commands
char c;
long code;
long line;
long LastLine = 0;

// coordinate mode
boolean abs_mode = true;

// measurement mode
boolean inches = true;

//// steppers state
//boolean steppers = false;

// debug level
// 0 = no debug
// 1 = short
// 2 = verbose
//=============
int DebugLevel = 0;


long StartTime = 0;

// Define the acceleration and max speed values
#define ACCELERATION 1000
#define MAX_SPEED 5000


void setup()
{ Serial.begin(230400);
  Serial.println("start");

  set_StepperSetting(Test_Stepper, 5000, 8000);
  set_StepperSetting(X_Stepper, 1200, 1250); // 1000, 1300
  set_StepperSetting(Y_Stepper, 2200, 1800); // 1800, 2500
  set_StepperSetting(Z_Stepper, 8000, 8000); // 3000, 4000
  set_StepperSetting(A_Stepper, 500, 500);

   // initialise subsystems
    init_process_string();      

  StartTime = millis();
}


void set_StepperSetting(AccelStepper &stepper, int Speed, int accel) {
  stepper.setMaxSpeed(Speed);
  stepper.setAcceleration(accel);
}

void print_position(String axis, int value, String type) {
  Serial.println((String)type + "현재 " + axis + "축 위치 :" + value);
}

void init_process_string()
{
  for (byte i = 0; i < COMMAND_SIZE; i++)
    aWord[i] = 0;
  serial_count = 0;
}



// 루프문!!
void loop() {

  getDataFromPython();

  X_Stepper.run();
  Y_Stepper.run();
  Z_Stepper.run();
  A_Stepper.run();

  //  if ((millis() - StartTime) >= 50) {
  //    StartTime = millis();
  //    print_position("X", X_Stepper.currentPosition(), "<d>");
  //    print_position("Y", Y_Stepper.currentPosition(), "<d>");
  //    print_position("Z", Z_Stepper.currentPosition(), "<d>");
  //    print_position("A", A_Stepper.currentPosition(), "<d>");
  //  }
}



void getDataFromPython() {
  if (Serial.available() > 0) {
    c = Serial.read();
    no_data = 0;

    if (c != '\n') {
      aWord[serial_count] = c;
      serial_count++;
    }

    // if we got a command, do it
    //===========================
    if (serial_count && (c == '\n')) {
      if (has_command('N', aWord, serial_count)) {
        line = (long)search_string('N', aWord, serial_count);
      }
      else {
        line = -1;
      }

      // check the checksum
      //===================
      if (Checksum(aWord, serial_count) == true) {
        if (line != LastLine + 1 && line != 0 && line != -1) {
          // line number out of synch
          //=========================
          if (line < LastLine + 1) {
            // repeated line so ignore
            //========================
            Serial.print("rp ");
            Serial.println(line, DEC);
          }
          else {
            // bad line number
            Serial.print("ex ");
          }
          Serial.println(LastLine + 1, DEC);
        }
        else {
          // now process our command!
          //=========================
          process_string(aWord, serial_count - 1);

          // update line number
          //===================
          LastLine = line;
        }
      }
      else {
        // checksum fail so request resend
        //================================
        Serial.print("rs ");
        Serial.println(LastLine + 1, DEC);
      }

      // and clear the command buffer
      //=============================
      init_process_string();
      no_data = 0;
      c = ' ';
    }
}

boolean Checksum(char instruction[], int size) {
  // compute checksum
  // return TRUE if ok
  // return FALSE if not
  //====================

  int i;
  int csum;
  int rsum;

  // start with 0
  //=============
  csum = 0;

  // xor each byte
  //==============
  for (i = 0; i < size; i++) {
    if (instruction[i] == '*')
      break;
    csum = csum ^ instruction[i];
  }

  // now see if digits match checksum
  //=================================
  rsum = search_string('*', instruction, size);
  if (rsum == csum)
    return true;
  else {
    if (DebugLevel > 1) {
      Serial.print("line: >");
      Serial.print(instruction);
      Serial.print("< length: ");
      Serial.print(size, DEC);
      Serial.print(" received: ");
      Serial.print(rsum, DEC);
      Serial.print(" calculated: ");
      Serial.println(csum, DEC);
    }
    return false;
  }
}


// see if required command exists
//===============================
bool has_command(char key, char instruction[], int string_size)
{
  int i;

  for (i = 0; i < string_size; i++) {
    if (instruction[i] == key)
      return true;
  }
  return false;
}

// look for the number that appears after the char key and return it
//==================================================================
double search_string(char key, char instruction[], int string_size)
{
  int k;
  char temp[10];
  int i;

  for (i = 0; i < 10; i++)
    temp[i] = 0;

  for (i = 0; i < string_size; i++) {
    if (instruction[i] == key) {
      i++;
      k = 0;
      // skip leading spaces
      //====================
      while (i < string_size && k < 10 && instruction[i] == ' ')
        i++;

      while (i < string_size && k < 10) {
        if (instruction[i] == 0 || instruction[i] == ' ')
          break;

        temp[k] = instruction[i];
        i++;
        k++;
      }
      return strtod(temp, NULL);
    }
  }
  return 0;
}

// Read the string and execute instructions
//=========================================
void process_string(char instruction[], int size)
{
  boolean result;

  if (DebugLevel > 1) {
    Serial.println(instruction);
  }

  result = true;

  if (has_command('/', instruction, size))
    result = ProcessComment(instruction, size);
  else if (has_command('G', instruction, size))
    result = ProcessGcode(instruction, size);
  else if (has_command('M', instruction, size))
    result = ProcessMcode(instruction, size);

  if (result) {
    // tell our host we're done
    // ========================
    Serial.print("ok ");
    Serial.println(line, DEC);
  }
}

boolean ProcessComment (char instruction[], int size) {

  // the / character means delete block... used for comments and stuff
  //==================================================================
  if (instruction[0] == '/') {
    if (DebugLevel > 1) {
      Serial.println("/ comment");
    }
  }
  return true;
}
