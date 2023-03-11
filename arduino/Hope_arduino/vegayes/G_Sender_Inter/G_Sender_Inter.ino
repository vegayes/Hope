// constant for mm conversions
//============================
#define MM_PER_INCH 25.4

//=====================================
// define the parameters of our machine
//=====================================
#define X_STEPS_PER_INCH 6400
#define Y_STEPS_PER_INCH 20000
#define Z_STEPS_PER_INCH 42850

//==================================
//our maximum feedrates units/minute
//==================================
#define FAST_XY_FEEDRATE_INCH 50.0
#define FAST_Z_FEEDRATE_INCH  100.0

//=======================
// Units in curve section
//=======================
#define CURVE_SECTION_MM 0.5

//=========================================
// Set to one if sensor outputs inverting
// (ie: 1 means open, 0 means closed)
// RepRap opto endstops are *not* inverting
//=========================================
#define SENSORS_INVERTING 0

//===============================================
// digital i/o pin assignment
// odd choices are for convenience to connectors
// NB Arduino pins 14-19 correspond to analog 0-5
//===============================================

//============================
// pin 0 used for serial comms
// pin 1 used for serial comms
//============================
#define SERIAL_RX_PIN 0
#define SERIAL_TX_PIN 1

//=======================================================
// shared enable pin (for rapid shutdown off all motors!)
//=======================================================
#define X_ENABLE_PIN 8
#define Y_ENABLE_PIN 18  // sb8
#define Z_ENABLE_PIN 19  // sb8

//===========================
// cartesian bot pins X-plane
// 6 7 A0 A1
//===========================
#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define X_MIN_PIN 10
#define X_MAX_PIN 11

//===========================
// cartesian bot pins Y-plane
// 2 3 4 5
//===========================
#define Y_STEP_PIN 3
#define Y_DIR_PIN 6
#define Y_MIN_PIN 12
#define Y_MAX_PIN 14

//==================================================
// pin 13 used for heartbeat as it already has a LED
//==================================================
# define HEARTBEAT_PIN 13

//===========================
// cartesian bot pins Z-plane
// 9 10 11 12
//===========================
#define Z_STEP_PIN 4
#define Z_DIR_PIN 7
#define Z_MIN_PIN 15  // sb 11
#define Z_MAX_PIN 16  // sb 12

//==============
// motor control
// A4
//==============
#define MOTOR_ON_PIN 18           // HIGH = ON, LOW = OFF

//========================================
// motor direction doesn't have a pin yet!
//========================================
// #define MOTOR_DIRECTION_PIN 20    // HIGH = Clockwise, LOW = Anti-clockwise

//======================


//--------------------------------------------------------------------------------------------------------------------
// Arduino G-code Interpreter
// v1.0 by Mike Ellery - initial software (mellery@gmail.com)
// v1.1 by Zach Hoeken - cleaned up and did lots of tweaks (hoeken@gmail.com)
// v1.2 by Chris Meighan - cleanup / G2&G3 support (cmeighan@gmail.com)
// v1.3 by Zach Hoeken - added thermocouple support and multi-sample temp readings. (hoeken@gmail.com)

// v1.3a mmcp - removed extruder functionality
//            - changed variable array called word (!) to array called aWord
//            - added checksum to serial comms

// v1.4  mmcp - extended M114 to report limit switches as well as co-ordinates
//============================================================================

#define FIRMWARE_VERSION "v1.4"

#include <HardwareSerial.h>

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
//========================
char c;
long code;
long line;
long LastLine = 0;

// coordinate mode
//================
boolean abs_mode = true;

// measurement mode
//=================
boolean inches = true;

// steppers state
//===============
boolean steppers = false;

// debug level
// 0 = no debug
// 1 = short
// 2 = verbose
//=============
int DebugLevel = 0;

void setup()
{
  // open comms port
  //================
  Serial.begin(230400);
  Serial.println("start");

  // initialise subsystems
  //======================
  init_process_string();
  init_steppers();
}

void loop()
{
  // read in characters if we got them
  //==================================
  if (Serial.available() > 0) {
    // read next character
    //====================
    c = Serial.read();
    no_data = 0;

    // newline is end of command
    //==========================
    if (c != '\n') {
      aWord[serial_count] = c;
      serial_count++;
    }
  }
  else {
    // start counting no-data clicks to timeout steppers 스테퍼 시간 초과에 대한 데이터 없는 클릭 수 계산 시작
    //==================================================
    no_data++;
    delayMicroseconds(25);
  }

  // if we got a command, do it
  //===========================
  if (serial_count && (c == '\n')) {
    //Serial.println("command");

    // extract line number if present
    //===============================
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
          //================
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

  // if no data turn off steppers
  //=============================
  if (no_data > 10000)
    disable_steppers();
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
      Serial.println(csum, DEC); //DEC => 10진수
    }
    return false;
  }
}

//------------------------------------------------------------------------------------------------------

// init our string processing
//===========================
void init_process_string()
{
  // init our command
  //=================
  for (byte i = 0; i < COMMAND_SIZE; i++)
    aWord[i] = 0;
  serial_count = 0;
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

//---------------------------------------------------------------------------------------------------

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


//--------------------------------------------------------------------------------------------------

FloatPoint current_units;
FloatPoint target_units;
FloatPoint delta_units;

FloatPoint current_steps;
FloatPoint target_steps;
FloatPoint delta_steps;

// default to inches for units
//============================
float x_units = X_STEPS_PER_INCH;
float y_units = Y_STEPS_PER_INCH;
float z_units = Z_STEPS_PER_INCH;
float curve_section = CURVE_SECTION_MM / MM_PER_INCH;

// our feedrate variables
///======================
float feedrate;
long feedrate_micros;
float FastFeedRateXY;
float FastFeedRateZ;

boolean ProcessGcode (char instruction[], int size) {

  float ParameterValue;
  float angleA, angleB, angle, radius, length, aX, aY, bX, bY;
  long steps, s, one_step;
  long p;
  FloatPoint newPoint;
  FloatPoint fp;

  feedrate = 0.0;
  feedrate_micros = 0;

  // find which G code
  //==================
  code = (int)search_string('G', instruction, size);

  // Get co-ordinates if required by the code type given
  // G0 G1 G2 G3 all need co-ordinates
  //====================================================
  switch (code) {
    case 0:  // rapid position
    case 1:  // linear interpolation
    case 2:  // circular interpolation (clockwise)
    case 3:  // circular interpolation (anti-clockwise)
      if (abs_mode) {
        // we do it like this to save time. makes curves better
        // eg. if only x and y are specified, we dont have to waste time looking up z.
        //============================================================================

        if (has_command('X', instruction, size))
          fp.x = search_string('X', instruction, size);
        else
          fp.x = current_units.x;

        if (has_command('Y', instruction, size))
          fp.y = search_string('Y', instruction, size);
        else
          fp.y = current_units.y;

        if (has_command('Z', instruction, size))
          fp.z = search_string('Z', instruction, size);
        else
          fp.z = current_units.z;
      }
      else {
        fp.x = search_string('X', instruction, size) + current_units.x;
        fp.y = search_string('Y', instruction, size) + current_units.y;
        fp.z = search_string('Z', instruction, size) + current_units.z;
      }
      break;
  }
  // now execute command
  //====================
  switch (code) {

    // G0 G1 ========================================================================

    // Rapid Positioning
    // Linear Interpolation
    // these are basically the same thing.
    //====================================
    case 0:  // rapid position
    case 1:  // linear interpolation

      // set our target
      //===============
      set_target(fp.x, fp.y, fp.z);

      // do we have a set speed?
      //========================
      switch (code) {
        case 0:
          if (DebugLevel > 1)
            Serial.print("Rapid position ");

          // use our max for normal moves
          //=============================
          feedrate_micros = getMaxSpeed();
          break;

        case 1:
          if (DebugLevel > 1)
            Serial.print("Linear interpolation ");

          // how fast do we move?
          //=====================
          feedrate = search_string('F', instruction, size);
          if (feedrate > 0)
            // we have a feed rate
            //====================
            feedrate_micros = calculate_feedrate_delay(feedrate);
          else
            // no feed rate so use maximum
            //============================
            feedrate_micros = getMaxSpeed();
          break;
      }

      // finally execute the move
      //=========================
      dda_move(feedrate_micros);

      if (DebugLevel > 1) {
        if (has_command('X', instruction, size)) {
          Serial.print("X: ");
          Serial.print(fp.x, 4);
        }
        if (has_command('Y', instruction, size)) {
          Serial.print(" Y: ");
          Serial.print(fp.y, 4);
        }
        if (has_command('Z', instruction, size)) {
          Serial.print(" Z: ");
          Serial.print(fp.z, 4);
        }
        if (has_command('F', instruction, size)) {
          Serial.print(" F: ");
          Serial.print(float(feedrate), 4);
        }
        Serial.println("");
      }

      break;

    // G2 G3 ========================================================================

    case 2:  // Clockwise arc
    case 3:  // Counterclockwise arc

      switch (code) {
        case 2:
          if (DebugLevel > 1)
            Serial.println("clockwise arc");
          break;

        case 3:
          if (DebugLevel > 1)
            Serial.println("anti-clockwise arc");
          break;
      }
      FloatPoint cent;

      // Centre coordinates are always relative
      //=======================================
      cent.x = search_string('I', instruction, size) + current_units.x;
      cent.y = search_string('J', instruction, size) + current_units.y;

      aX = (current_units.x - cent.x);
      aY = (current_units.y - cent.y);
      bX = (fp.x - cent.x);
      bY = (fp.y - cent.y);

      switch (code) {
        case 2: // Clockwise
          angleA = atan2(bY, bX);
          angleB = atan2(aY, aX);
          break;

        case 3: // Counterclockwise
          angleA = atan2(aY, aX);
          angleB = atan2(bY, bX);
          break;
      }

      // Make sure angleB is always greater than angleA
      // and if not add 2PI so that it is (this also takes
      // care of the special case of angleA == angleB,
      // ie we want a complete circle)
      //===================================================
      if (angleB <= angleA) angleB += 2 * M_PI;
      angle = angleB - angleA;

      radius = sqrt(aX * aX + aY * aY);
      length = radius * angle;
      steps = (int) ceil(length / curve_section);

      for (s = 1; s <= steps; s++) {
        if (DebugLevel > 2) {
          Serial.print("start x: ");
          Serial.print(current_units.x, 2);
          Serial.print(" start y: ");
          Serial.println(current_units.y, 2);
          Serial.print("end x: ");
          Serial.print(fp.x, 2);
          Serial.print(" end y: ");
          Serial.println(fp.y, 2);
          Serial.print("centre x: ");
          Serial.print(cent.x, 2);
          Serial.print(" centre y: ");
          Serial.println(cent.y, 2);
          Serial.print("steps: " );
          Serial.println(steps, DEC);
          Serial.print("angle A: ");
          Serial.println(angleA, DEC);
          Serial.print("angle B: ");
          Serial.println(angleB, DEC);
          Serial.print("angle: ");
          Serial.println(angle, DEC);
          Serial.print("radius: ");
          Serial.println(radius, DEC);
        }
        one_step = (code == 3) ? s : steps - s; // Work backwards for CW
        newPoint.x = cent.x + radius * cos(angleA + angle * ((float) one_step / steps));
        newPoint.y = cent.y + radius * sin(angleA + angle * ((float) one_step / steps));
        set_target(newPoint.x, newPoint.y, fp.z);

        // Need to calculate rate for each section of curve
        //=================================================
        if (feedrate > 0)
          feedrate_micros = calculate_feedrate_delay(feedrate);
        else
          feedrate_micros = getMaxSpeed();

        // Make step
        //==========
        dda_move(feedrate_micros);
      }
      break;

    // G4 ========================================================================

    case 4:  // Dwell (milliseconds)
      if (DebugLevel > 1)
        Serial.print("dwell: ");
      ParameterValue = search_string('P', instruction, size);

      // turn on red and green to indicate pause
      //========================================
      disable_steppers();
      InitialiseHeartBeat(true);
      for (p = 1; p <= (int)ParameterValue * 2; p++) {
        delayMicroseconds(500);
        Heartbeat();
      }
      if (DebugLevel > 1) {
        Serial.print(ParameterValue, DEC);
        Serial.println(" mSeconds");
      }
      break;

    // G20 ========================================================================

    case 20:  // Inches for Units
      if (DebugLevel > 1)
        Serial.println("units inches");
      UnitsInches();
      if (DebugLevel > 1) {
        DebugUnits();
      };
      break;

    // G21 ========================================================================

    case 21:  // mm for Units
      if (DebugLevel > 1)
        Serial.println("units mm");
      UnitsMM();
      if (DebugLevel > 1) {
        DebugUnits();
      };
      break;

    // G28 ========================================================================

    case 28:  // go home
      if (DebugLevel > 1) {
        Serial.println("go to end stops");
      };
      set_target(0.0, 0.0, 0.0);
      dda_move(getMaxSpeed());

      break;

    //========================================================================

    case 90:  // Absolute Positioning
      if (DebugLevel > 1) {
        Serial.println("absolute");
      }
      abs_mode = true;
      break;

    // G91 ========================================================================

    case 91:  // Incremental Positioning
      if (DebugLevel > 1) {
        Serial.println("relative");
      }
      abs_mode = false;
      break;

    // G92 ========================================================================

    case 92:  // Set as home
      if (DebugLevel > 1) {
        Serial.println("set current as home");
      }
      set_position(0.0, 0.0, 0.0);
      break;

    // G?? ========================================================================

    default:
      Serial.print("huh? G");
      Serial.println(code, DEC);
  }
  return true;
}   // end of G code processing
//=========================

void DebugUnits() {
  Serial.print("x units: ");
  Serial.print(x_units, DEC);
  Serial.print(" y units: ");
  Serial.print(y_units, DEC);
  Serial.print(" z units: ");
  Serial.println(z_units, DEC);
}

void UnitsInches() {
  // basic units are defined in steps per inch
  //==========================================

  x_units = X_STEPS_PER_INCH;
  y_units = Y_STEPS_PER_INCH;
  z_units = Z_STEPS_PER_INCH;
  FastFeedRateXY = FAST_XY_FEEDRATE_INCH;
  FastFeedRateZ = FAST_Z_FEEDRATE_INCH;

  // this is deliberatly different
  // as we are defining a size not no. of steps
  //===========================================
  curve_section = CURVE_SECTION_MM / MM_PER_INCH;

  if (!inches) {
    current_units.x = current_units.x / MM_PER_INCH;
    current_units.y = current_units.y / MM_PER_INCH;
    current_units.z = current_units.z / MM_PER_INCH;
  }
  inches = true;

  calculate_deltas();
}

void UnitsMM() {
  // basic units are defined in steps per inch
  //==========================================

  x_units = X_STEPS_PER_INCH / MM_PER_INCH;
  y_units = Y_STEPS_PER_INCH / MM_PER_INCH;
  z_units = Z_STEPS_PER_INCH / MM_PER_INCH;
  FastFeedRateXY = FAST_XY_FEEDRATE_INCH * MM_PER_INCH;
  FastFeedRateZ = FAST_Z_FEEDRATE_INCH * MM_PER_INCH;

  // curve section is defined in mm
  //===============================
  curve_section = CURVE_SECTION_MM;

  if (inches) {
    current_units.x = current_units.x * MM_PER_INCH;
    current_units.y = current_units.y * MM_PER_INCH;
    current_units.z = current_units.z * MM_PER_INCH;
  }

  inches = false;

  calculate_deltas();
}


//-------------------------------------------------------------------------------------
boolean ProcessMcode(char instruction[], int size) {

  float ParameterValue;

  // find which M code
  //==================
  code = search_string('M', instruction, size);
  switch (code) {

    //========================================================================

    case -1:  // no code found
      return true;
      break;

    // M0 ========================================================================

    case 0: // program stop
      if (DebugLevel > 1)
        Serial.println("program stop");

      // finished with engines
      //======================
      StopSpindle();
      disable_steppers();
      return true;
      break;

    // M2 ========================================================================

    case 2:  //program end
      if (DebugLevel > 1)
        Serial.println("program end");

      // finished with engines
      //======================
      StopSpindle();
      disable_steppers();
      return true;
      break;

    // M3 ========================================================================

    case 3:  // turn spindle clockwise
      if (DebugLevel > 1)
        Serial.println("turn spindle clockwise");

      // gentlemen, start your engines
      //==============================
      StartSpindle(true);
      return true;
      break;

    // M4 ========================================================================

    case 4:  // turn spindle anti-clockwise
      if (DebugLevel > 1)
        Serial.println("turn spindle anti-clockwise");

      // gentlemen, start your engines
      //==============================
      StartSpindle(false);
      return true;
      break;

    // M5 ========================================================================

    case 5:  // stop spindle
      if (DebugLevel > 1)
        Serial.println("stop spindle");

      // stop the spindle
      //=================
      StopSpindle();
      return true;
      break;

    // M110 ========================================================================

    case 110:  // set line number
      // since this arrives with an "N" line number, there's nothing to do!
      //===================================================================
      if (DebugLevel > 1) {
        Serial.print("set line number: ");
        Serial.println(line, DEC);
      }
      return true;
      break;

    // M111 ========================================================================

    case 111:  // set debug level
      ParameterValue = search_string('P', instruction, size);
      DebugLevel = ParameterValue;
      if (DebugLevel > 1) {
        Serial.print("debug level ");
        Serial.println(DebugLevel, DEC);
      }
      return true;
      break;

    // M114 ========================================================================

    case 114:  // get position

      // send current co-ordinates
      //==========================
      Serial.print("ok ");
      Serial.print(line, DEC);
      Serial.print(" C: X:");
      Serial.print(current_units.x, 4);
      Serial.print(" Y:");
      Serial.print(current_units.y, 4);
      Serial.print(" Z:");
      Serial.print(current_units.z, 4);

      // send limit switches
      //====================
      Serial.print(" XMin: ");
      Serial.print(read_switch(X_MIN_PIN), DEC);
      Serial.print(" XMax: ");
      Serial.print(read_switch(X_MAX_PIN), DEC);
      Serial.print(" YMin: ");
      Serial.print(read_switch(Y_MIN_PIN), DEC);
      Serial.print(" YMax: ");
      Serial.print(read_switch(Y_MAX_PIN), DEC);
      Serial.print(" ZMin: ");
      Serial.print(read_switch(Z_MIN_PIN), DEC);
      Serial.print(" ZMax: ");
      Serial.print(read_switch(Z_MAX_PIN), DEC);
      Serial.println();
      return false;
      break;

    // M115 ========================================================================

    case 115:  // get firmware version
      // send version info
      //==================
      Serial.print("ok ");
      Serial.print(line, DEC);
      Serial.print(" PROTOCOL VERSION: ");
      Serial.print(FIRMWARE_VERSION);
      Serial.print(" FIRMWARE_NAME: mmcpix CNC");
      Serial.print(" MACHINE_TYPE: mmcpix");
      Serial.println(" EXTRUDER_COUNT: 0");
      return false;
      break;


    // M254 ========================================================================

    case 254:  // Program start

      // reset debug level and line number
      //==================================
      DebugLevel = 0;
      line = 0;

      // and initialise the stepper sub-system
      //======================================
      init_steppers();

      // units = inches
      //===============
      UnitsInches();

      // absolute mode
      //==============
      abs_mode = true;
      return true;

    // M255 ========================================================================

    case 255:  // request command completion status

      // set line to last completed
      //===========================
      line = LastLine;

      // return line number of last completed command
      //=============================================
      Serial.print("cp ");
      Serial.println(LastLine, DEC);
      return false;
      break;

    // M?? ========================================================================

    default:
      Serial.print("Huh? M");
      Serial.println(code, DEC);
      return true;
  }
}   // end of M code processing
//=========================



//----------------------------------------------------------------------------------------------------------------------
//================
// spindle control
//================

void StartSpindle(boolean clockwise) {
  // start spindle in required direction
  //====================================

#ifdef MOTOR_DIRECTION_PIN
  if (clockwise)
    digitalWrite(MOTOR_DIRECTION_PIN, HIGH);
  else
    digitalWrite(MOTOR_DIRECTION_PIN, LOW);
#endif
  digitalWrite(MOTOR_ON_PIN, HIGH);
}

void StopSpindle() {
  // stop the spindle
  //=================
  digitalWrite(MOTOR_ON_PIN, LOW);
}


//--------------------------------------------------------
//================
// Stepper Control
//================

long milli_delay;

boolean panic;

boolean x_can_step;
boolean y_can_step;
boolean z_can_step;

// our direction vars
//===================
boolean x_direction = true;
boolean y_direction = true;
boolean z_direction = true;

// Heartbeat variables
//====================
int g_Beat;
boolean g_Heart;

void init_steppers() {

  // turn them off to start
  //=======================
  disable_steppers();

  // init our points
  //================
  current_units.x = 0.0;
  current_units.y = 0.0;
  current_units.z = 0.0;
  target_units.x = 0.0;
  target_units.y = 0.0;
  target_units.z = 0.0;
  current_steps.x = 0.0;
  current_steps.y = 0.0;
  current_steps.z = 0.0;

  inches = true;

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT);
  pinMode(Z_MAX_PIN, INPUT);

  pinMode(HEARTBEAT_PIN, OUTPUT);



  // start in not-panic mode
  //========================
  panic = false;

  // figure our stuff.
  //=================
  calculate_deltas();

  // initialise Heartbeat
  //=====================
  InitialiseHeartBeat(false);
}

void dda_move(long micro_delay)
{
  long max_delta;
  long x_counter;
  long y_counter;
  long z_counter;

  // initialise Heartbeat
  //=====================
  InitialiseHeartBeat(true);

  // enable our steppers
  //====================
  enable_steppers();

  // figure out our deltas
  //======================
  max_delta = max(delta_steps.x, delta_steps.y);
  max_delta = max(delta_steps.z, max_delta);

  // init stuff
  //===========
  x_counter = -max_delta / 2;
  y_counter = -max_delta / 2;
  z_counter = -max_delta / 2;

  // our step flags
  //===============
  x_can_step = false;
  y_can_step = false;
  z_can_step = false;

  if (micro_delay >= 16383)
    milli_delay = micro_delay / 1000;
  else
    milli_delay = 0;

  if (DebugLevel > 2) {
    Serial.print("MaxDelta: ");
    Serial.print(max_delta, DEC);
    Serial.print(" MilliDelay: ");
    Serial.print(milli_delay, DEC);
    Serial.print(" MicroDelay: ");
    Serial.println(micro_delay, DEC);
  }
  // do our DDA line!
  do {
    x_can_step = can_step(X_MIN_PIN, X_MAX_PIN, current_steps.x, target_steps.x, x_direction);
    y_can_step = can_step(Y_MIN_PIN, Y_MAX_PIN, current_steps.y, target_steps.y, y_direction);
    z_can_step = can_step(Z_MIN_PIN, Z_MAX_PIN, current_steps.z, target_steps.z, z_direction);

    if (x_can_step) {
      x_counter += delta_steps.x;

      if (x_counter > 0) {
        do_step(X_STEP_PIN);
        x_counter -= max_delta;

        if (x_direction)
          current_steps.x++;
        else
          current_steps.x--;
      }
    }

    if (y_can_step) {
      y_counter += delta_steps.y;

      if (y_counter > 0) {
        do_step(Y_STEP_PIN);
        y_counter -= max_delta;

        if (y_direction)
          current_steps.y++;
        else
          current_steps.y--;
      }
    }

    if (z_can_step) {
      z_counter += delta_steps.z;

      if (z_counter > 0) {
        do_step(Z_STEP_PIN);
        z_counter -= max_delta;

        if (z_direction)
          current_steps.z++;
        else
          current_steps.z--;
      }
    }

    // wait for next step
    //===================
    if (milli_delay > 0)
      delay(milli_delay);
    else
      delayMicroseconds(micro_delay);
    Heartbeat();
  } while (x_can_step || y_can_step || z_can_step);

  // set our current to target
  //==============================
  current_units.x = target_units.x;
  current_units.y = target_units.y;
  current_units.z = target_units.z;
  calculate_deltas();

  // reset Heartbeat
  //================
  InitialiseHeartBeat(false);
}

void InitialiseHeartBeat(boolean Light) {
  // prepare Heartbeat
  // in desired state
  //==================
  g_Beat = 0;
  g_Heart = Light;
  if (g_Heart)
    digitalWrite(HEARTBEAT_PIN, HIGH);
  else
    digitalWrite(HEARTBEAT_PIN, LOW);
}

void Heartbeat() {
  // a) flash LED on PIN 13 to indicate still alive
  // b) send char over serial interface to stop it timing out
  //=========================================================

  g_Beat++;
  if (g_Beat > 1000) {
    g_Beat = 0;
    g_Heart = ! g_Heart;

    // flash the LED
    //==============
    if (g_Heart) {
      digitalWrite(HEARTBEAT_PIN, HIGH);
      // keep serial port alive
      //=======================
      Serial.print("~");
    }
    else
      digitalWrite(HEARTBEAT_PIN, LOW);
  }
}

bool can_step(byte min_pin, byte max_pin, long current, long target, byte direction) {
  if (target == current)
    // stop us if we're on target
    //===========================
    return false;
  else if (read_switch(min_pin) && !direction)
    // stop us if we're at home and still going
    //=========================================
    return false;
  else if (read_switch(max_pin) && direction)
    // stop us if we're at max and still going
    //========================================
    return false;

  // default to being able to step
  //==============================
  return true;
}

void do_step(byte step_pin) {
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(step_pin, LOW);
}

bool read_switch(byte pin) {
  // dual read as crude debounce
  //============================

  if ( SENSORS_INVERTING )
    return !digitalRead(pin) && !digitalRead(pin);
  else
    return digitalRead(pin) && digitalRead(pin);
}

long to_steps(float steps_per_unit, float units) {
  return steps_per_unit * units;
}

void set_target(float x, float y, float z) {
  target_units.x = x;
  target_units.y = y;
  target_units.z = z;

  calculate_deltas();
}

void set_position(float x, float y, float z) {
  current_units.x = x;
  current_units.y = y;
  current_units.z = z;

  calculate_deltas();
}

void calculate_deltas() {
  // figure our deltas
  //==================

  delta_units.x = abs(target_units.x - current_units.x);
  delta_units.y = abs(target_units.y - current_units.y);
  delta_units.z = abs(target_units.z - current_units.z);

  // set our steps current, target, and delta
  //=========================================
  current_steps.x = to_steps(x_units, current_units.x);
  current_steps.y = to_steps(y_units, current_units.y);
  current_steps.z = to_steps(z_units, current_units.z);

  target_steps.x = to_steps(x_units, target_units.x);
  target_steps.y = to_steps(y_units, target_units.y);
  target_steps.z = to_steps(z_units, target_units.z);

  delta_steps.x = abs(target_steps.x - current_steps.x);
  delta_steps.y = abs(target_steps.y - current_steps.y);
  delta_steps.z = abs(target_steps.z - current_steps.z);

  // what is our direction
  //======================
  x_direction = (target_units.x >= current_units.x);
  y_direction = (target_units.y >= current_units.y);
  z_direction = (target_units.z >= current_units.z);

  if (DebugLevel > 2) {
    Serial.print("currentX: ");
    Serial.print(current_steps.x, 4);
    Serial.print(" targetX: ");
    Serial.print(target_steps.x, 4);
    Serial.print(" direction: ");
    Serial.println(x_direction, DEC);
    Serial.print("currentY: ");
    Serial.print(current_steps.y, 4);
    Serial.print(" targetY: ");
    Serial.print(target_steps.y, 4);
    Serial.print(" direction: ");
    Serial.println(y_direction, DEC);
  }

  //set our direction pins as well
  //==============================
  digitalWrite(X_DIR_PIN, x_direction);
  digitalWrite(Y_DIR_PIN, y_direction);
  digitalWrite(Z_DIR_PIN, z_direction);
}

long calculate_feedrate_delay(float feedrate) {
  // calculate feed rate delay depending on distance and feed rate
  //==============================================================

  float delay;
  long master_steps;
  float distance;

  // how long is our traverse distance
  //==================================
  distance = sqrt(delta_units.x * delta_units.x + delta_units.y * delta_units.y + delta_units.z * delta_units.z);
  master_steps = 0;

  // find the dominant axis
  //=======================
  if (delta_steps.x > delta_steps.y) {
    if (delta_steps.z > delta_steps.x)
      master_steps = delta_steps.z;
    else
      master_steps = delta_steps.x;
  }
  else {
    if (delta_steps.z > delta_steps.y)
      master_steps = delta_steps.z;
    else
      master_steps = delta_steps.y;
  }

  // calculate delay between steps in microseconds.  this is sort of tricky, but not too bad.
  // the formula has been condensed to save space.  here it is in english:
  // distance / feedrate * 60000000.0 = move duration in microseconds
  // move duration / master_steps = time between steps for master axis.
  //===================================================================

  delay = ((distance * 60000000.0) / feedrate) / master_steps;
  return delay;
}

long getMaxSpeed() {
  // get maximum speed
  //==================

  if (delta_steps.z > 0)
    // Z move so use max Z speed
    //==========================
    return calculate_feedrate_delay(FastFeedRateZ);
  else
    // use max XY speed
    //=================
    return calculate_feedrate_delay(FastFeedRateXY);
}

void disable_steppers() {
  // disable all steppers
  //=====================

  digitalWrite(X_ENABLE_PIN, HIGH);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
  if (DebugLevel > 2) {
    if (steppers) {
      Serial.println("disable steppers");
    }
  }
  steppers = false;
}

void enable_steppers() {
  // enable all steppers
  //====================

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);
  if (DebugLevel > 2) {
    if (!steppers) {
      Serial.println("enable steppers");
    }
  }
  steppers = true;
}
