/*Nov 2, Sun test code, Jimmy
Test SIX!
After testing and editting, this would be the final code we use!

  1) Create & Use void stopStay() to immediately stop all actuators
  2) Create & Use void runMotors()

**Commands:
    "1" - Left - X Axis Move 250 steps to Negative direction
    "2" - Right - X Axis Move 250 steps to Positive direction
    "3" - Down - Y Axis Move 1000 steps to Positive direction
    "4" - Up - Y Axis Move 1000 steps to Negative direction
    "5" - Stop Moving
    "6" - Draw a square
    "7" - Draw a Triangle (referencing codes from Vic & Ellie)
    "8" - Draw a Circle (referencing codes from Vic & Ellie)
    "9" - Stop Drawing - It would be read only when drawing
    
*/

#include <AccelStepper.h>         // define step constant
#define STEP_PER_REVOLUTION 2048  // this value is from datasheet
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper_x(4, 7, 5, 6, 4);    //the first "4" refers to nm. of pins on the motor
AccelStepper stepper_y(4, 11, 9, 10, 8);  //the first "4" refers to nm. of pins on the motor

//These values will be updated in the checkSerial section,
//and used in the runMotors section to run the actuators
long xCurrent, xTarget, yCurrent, yTarget;
char receivedCommand;
char stopDraw;

void setup() {
  Serial.begin(9600);

  //initialize motors
  stepper_x.setAcceleration(400.0);
  stepper_x.setMaxSpeed(550.0);
  stepper_y.setAcceleration(400.0);
  stepper_y.setMaxSpeed(550.0);

  //initialize LED and Solenoid
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {

  //checkSerial
  if (Serial.available() > 0) {
    receivedCommand = Serial.read();  // pass the value to the receivedCommad variable
    switch (receivedCommand)          //we check what is the command
    {
      case '1':  //move to positive direction
        stepper_x.move(-250);
        break;
      case '2':  //move to positive direction
        stepper_x.move(250);
        break;
      case '3':  //move to positive direction
        stepper_y.move(1000);
        break;
      case '4':  //move to positive direction
        stepper_y.move(-1000);
        break;
      case '5':  //Stop and stay at current position
        stopStay();
        break;

      case '6':  // turn LED on and run to a position
        xCurrent = stepper_x.currentPosition();
        yCurrent = stepper_y.currentPosition();
        xTarget = xCurrent + 125;
        stepper_x.runToNewPosition(xTarget);  //Right
        yTarget = yCurrent + 500;
        stepper_y.runToNewPosition(yTarget);  // down

        digitalWrite(12, HIGH);
        digitalWrite(3, HIGH);
        delay(1000);
        //xTarget = xCurrent - 125;
        //stepper_x.runToNewPosition(xTarget);  //left
        stepper_x.move(-250);
        while (stepper_x.distanceToGo() != 0) {
          if (Serial.available() > 0) {
            stopDraw = Serial.read();
            if (stopDraw == 9) {
              stopStay();
              break;
            }
          }
          stepper_x.run();
        }
        //yTarget = yCurrent - 500;
        //stepper_y.runToNewPosition(yTarget);  // up
        stepper_y.move(-1000);
        while (stepper_y.distanceToGo() != 0) {
          if (Serial.available() > 0) {
            stopDraw = Serial.read();
            if (stopDraw == 9) {
              stopStay();
              break;
            }
          }
          stepper_y.run();
        }
        //xTarget = xCurrent + 125;
        //stepper_x.runToNewPosition(xTarget);  // right
        stepper_x.move(250);
        while (stepper_x.distanceToGo() != 0) {
          if (Serial.available() > 0) {
            stopDraw = Serial.read();
            if (stopDraw == 9) {
              stopStay();
              break;
            }
          }
          stepper_x.run();
        }

        //yTarget = yCurrent + 500;
        //stepper_y.runToNewPosition(yTarget);  // down
        stepper_y.move(1000);
        while (stepper_y.distanceToGo() != 0) {
          if (Serial.available() > 0) {
            stopDraw = Serial.read();
            if (stopDraw == 9) {
              stopStay();
              break;
            }
          }
          stepper_y.run();
        }

        stopStay();
        break;


      default:
        break;
    }
  }

  //runMotors
  stepper_x.run();
  stepper_y.run();
}


void stopStay() {
  digitalWrite(12, LOW);
  digitalWrite(3, LOW);
  xCurrent = stepper_x.currentPosition();
  stepper_x.moveTo(xCurrent);
  yCurrent = stepper_y.currentPosition();
  stepper_y.moveTo(yCurrent);
}