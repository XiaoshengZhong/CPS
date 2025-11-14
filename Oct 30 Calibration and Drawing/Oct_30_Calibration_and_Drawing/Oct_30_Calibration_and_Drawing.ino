/*Oct 29, Wednesday test code, Jimmy
  Test FOUR!
**Goals:
  1) Test moving commands for 1 stepper motor:
    Wek"5-5" - "0" - Stop function "1-4" 
    Wek"4-5" - "1" - Left - X Axis Move 250 steps to Negative direction
    Wek"3-5" - "2" - Right - X Axis Move 250 steps to Positive direction
    Wek"2-5" - "3" - Down - Y Axis Move 1000 steps to Positive direction
    Wek"1-5" - "4" - Up - Y Axis Move 1000 steps to Negative direction
  2) Test drawing commands:
    Wek"5-5" - "5" - Stop function "6-9"
    Wek"5-5" - "6" - Draw a square
    Wek"5-5" - "7" - Draw a  triangle
    Wek"5-5" - "8" - Draw a  circle
    Wek"5-5" - "9" - Draw another shape
    
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

void setup() {
  Serial.begin(9600);

  //initialize X axis motor
  stepper_x.setAcceleration(400.0);
  stepper_x.setMaxSpeed(550.0);
  stepper_y.setAcceleration(400.0);
  stepper_y.setMaxSpeed(550.0);
  Serial.println("ready");

  //initialize LED or Solenoid
  pinMode(12, OUTPUT);
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
      case '5': //Stop and stay at current position
        xCurrent = stepper_x.currentPosition();
        Serial.println(xCurrent);
        stepper_x.moveTo(xCurrent);
        yCurrent = stepper_y.currentPosition();
        Serial.println(yCurrent);
        stepper_y.moveTo(yCurrent);
        break;

      case '6':  // turn LED on and run to a position
        xCurrent = stepper_x.currentPosition();
        yCurrent = stepper_y.currentPosition();
        xTarget = xCurrent + 125;
        stepper_x.runToNewPosition(xTarget); //Right
        yTarget = yCurrent + 500;
        stepper_y.runToNewPosition(yTarget); // down

        digitalWrite(12, HIGH);
        xTarget = xCurrent - 125;
        stepper_x.runToNewPosition(xTarget); //left 
        yTarget = yCurrent - 500;
        stepper_y.runToNewPosition(yTarget); // up
        xTarget = xCurrent + 125;
        stepper_x.runToNewPosition(xTarget); // right
        yTarget = yCurrent + 500;
        stepper_y.runToNewPosition(yTarget); // down

        digitalWrite(12, LOW);
        break;

      case '7':  //Draw Triangle
        xCurrent = stepper_x.currentPosition();
        yCurrent = stepper_y.currentPosition();
        yTarget = yCurrent + 500;
        stepper_y.runToNewPosition(yTarget);
        digitalWrite(12, HIGH);

        xTarget = xCurrent + 500 * cos(210);
        yTarget = yCurrent + 500 * sin(210);
        stepper_x.setMaxSpeed(550 * 433 / 750);  // 317 / Need to test the actual speed required
        stepper_x.moveTo(xTarget);
        stepper_y.moveTo(yTarget);
        while (stepper_x.distanceToGo() != 0 || stepper_y.distanceToGo() != 0) {
          stepper_x.run();
          stepper_y.run();
        }

        xTarget = xCurrent + 500 * cos(330);
        stepper_x.moveTo(xTarget);
        stepper_x.setMaxSpeed(550.0);
        while (stepper_x.distanceToGo() != 0) {
          stepper_x.run();
        }

        xTarget = xCurrent;
        yTarget = yCurrent + 500;
        stepper_x.setMaxSpeed(550 * 433 / 750);  // 317 / Need to test the actual speed required
        stepper_x.moveTo(xTarget);
        stepper_y.moveTo(yTarget);
        while (stepper_x.distanceToGo() != 0 || stepper_y.distanceToGo() != 0) {
          stepper_x.run();
          stepper_y.run();
        }

        stepper_x.setMaxSpeed(550.0);
        digitalWrite(12, LOW);
        break;

      case '8':  //Draw Circle!
        xCurrent = stepper_x.currentPosition();
        yCurrent = stepper_y.currentPosition();
        xTarget = xCurrent + 500;
        stepper_y.runToNewPosition(yTarget);
        digitalWrite(12, HIGH);

        for (int i = 0; i <= 40; i++) {
          xTarget = xCurrent + 500 * cos(i*9);
          yTarget = yCurrent + 500 * sin(i*9);
          stepper_x.moveTo(xTarget);
          stepper_y.moveTo(yTarget);
          while (stepper_x.distanceToGo() != 0 || stepper_y.distanceToGo() != 0) {
            stepper_x.run();
            stepper_y.run();
          }
        }
        digitalWrite(12, LOW);
        break;

        case '9': // Circle by Vic
         digitalWrite(12, HIGH);

  long xCenter = stepper_x.currentPosition();
  long yCenter = stepper_y.currentPosition();
  long R = 500;           // radius in steps
  int stepsPerCircle = 100;

  // Scale factors
  float X_scale = 1.2;     // optional: stretch X slightly
  float Y_scale = 0.333;   // shrink Y to 1/3

  // High speed and acceleration
  stepper_x.setMaxSpeed(2000.0);
  stepper_y.setMaxSpeed(2000.0);
  stepper_x.setAcceleration(1000.0);
  stepper_y.setAcceleration(1000.0);

  for (int i = 0; i <= stepsPerCircle; i++) {
    float theta = (PI/2) * ((float)i/stepsPerCircle);

    long xTarget = xCenter + (long)((R * (1 - cos(theta))) * X_scale);
    long yTarget = yCenter + (long)((R *sin(theta)) * Y_scale);

    stepper_x.moveTo(xTarget);
    stepper_y.moveTo(yTarget);

    while (stepper_x.distanceToGo() != 0 || stepper_y.distanceToGo() != 0) {
      stepper_x.run();
      stepper_y.run();
    }
  }

  digitalWrite(12, LOW);


      default:
        break;
    }
  }

  //runMotors
  stepper_x.run();
  stepper_y.run();
}