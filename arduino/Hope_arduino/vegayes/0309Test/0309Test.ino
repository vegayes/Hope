// G코드 가져오는 또 다른 방법

#include <AccelStepper.h>

// Define the stepper motor pins
#define STEP_PIN 2
#define DIR_PIN 5

//#define X_DIR 5   // X axis, direction pin
#define Y_DIR 6   // Y
#define Z_DIR 7   // Z
#define A_DIR 13  // A

//#define X_STP 2   // X axis, step pin
#define Y_STP 3   // Y
#define Z_STP 4   // Z
#define A_STP 12  // A

// Define the acceleration and max speed values
#define ACCELERATION 1000
#define MAX_SPEED 5000

// Create an instance of the AccelStepper class
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the max speed and acceleration
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);

  // Set the initial position of the stepper motor
  stepper.setCurrentPosition(0);
  
  // Initialize the serial communication
  Serial.begin(9600);
  Serial.println("로딩완료");
}

void loop() {
  // Check if there is serial data available
  if (Serial.available() > 0) {
    // Read the serial data
    String input = Serial.readStringUntil('\n');

    // Split the input string into command and value
    char command = input.charAt(0);
    int value = input.substring(1).toInt();

    // Execute the command
    switch (command) {
      case 'X':
        // Move the stepper motor to the specified position
        stepper.moveTo(value);
        // Wait for the motor to reach the position
        while (stepper.distanceToGo() != 0) {
          stepper.run();
        }
        break;
      case 'Y':
        // Move the stepper motor to the specified position
        stepper.moveTo(value);
        // Wait for the motor to reach the position
        while (stepper.distanceToGo() != 0) {
          stepper.run();
        }
        break;
      case 'Z':
        // Move the stepper motor to the specified position
        stepper.moveTo(value);
        // Wait for the motor to reach the position
        while (stepper.distanceToGo() != 0) {
          stepper.run();
        }
        break;
      default:
        // Invalid command
        break;
    }
  }
}
