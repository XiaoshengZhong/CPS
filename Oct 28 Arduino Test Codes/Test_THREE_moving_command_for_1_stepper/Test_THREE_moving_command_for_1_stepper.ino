/*Oct 28, Tuesday test code, Jimmy
  Test THREE!
**Goals:
  1) Test moving commands for 1 stepper motor:
    "1" - Move 1000 steps to Positive direction
    "2" - Stop while function "1" is running, but do not stop while function "3" is running
  2) Test drawing commands:
    "3" - it turns the solenoid/LED on, Move 500 steps to Positive direction, then move 500 steps to negative direction at HALF SPEED 
*/

#include <AccelStepper.h>         // define step constant
#define STEP_PER_REVOLUTION 2048  // this value is from datasheet
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper_x(4, 11, 9, 10, 8);  //the first "4" refers to nm. of pins on the motor
//AccelStepper stepper_y(4, 7, 5, 6, 4);    //the first "4" refers to nm. of pins on the motor

//These values will be updated in the checkSerial section,
//and used in the runMotors section to run the actuators
long xCurrent;
long xTarget;
char receivedCommand;

void setup() {
  Serial.begin(9600);

  //initialize X axis motor
  stepper_x.setAcceleration(400.0);
  stepper_x.setMaxSpeed(550.0);
  Serial.println("ready");

  //initialize LED as replacement of solenoid
  pinMode(12, OUTPUT);
}

void loop() {

  //checkSerial
  if (Serial.available() > 0) {
    receivedCommand = Serial.read();  // pass the value to the receivedCommad variable
    switch (receivedCommand)          //we check what is the command
    {
      case '1':  //move to positive direction
        xCurrent = stepper_x.currentPosition();
        Serial.println(xCurrent);
        stepper_x.move(2048);
        break;


      case '2':
        long xCurrent = stepper_x.currentPosition();
        Serial.println(xCurrent);
        stepper_x.moveTo(xCurrent);
        break;

      case '3':  // turn LED on and run to a position
        Serial.println('s');
        digitalWrite(12, HIGH);
        stepper_x.runToNewPosition(0);
        stepper_x.runToNewPosition(500);
        stepper_x.setMaxSpeed(150);
        stepper_x.runToNewPosition(0);
        stepper_x.setMaxSpeed(500);
        digitalWrite(12, LOW);
        break;

      default:
        break;
    }
  }

  //runMotors
  stepper_x.run();
}

/*
void stopAll() {
  //read current position
  //run to current position / reset to 0?
  xPosition = stepper_x.currentPosition();
  stepper_x.runToNewPosition(xPosition);
}
*/