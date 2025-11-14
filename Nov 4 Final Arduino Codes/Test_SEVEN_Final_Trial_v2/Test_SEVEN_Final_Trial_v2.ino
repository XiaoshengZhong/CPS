/*Nov 4, Final Trial v2

**Commands:
    "1" - Left - X Axis Move 250 steps to Negative direction
    "2" - Right - X Axis Move 250 steps to Positive direction
    "3" - Down - Y Axis Move 1000 steps to Positive direction
    "4" - Up - Y Axis Move 1000 steps to Negative direction
    "5" - Stop Moving - **Apply to all commands
    "6" - Draw a square 
    "7" - Draw a Triangle (referencing codes from Vic & Ellie)
    "8" - Draw a Circle (referencing codes from Vic & Ellie)

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
int resetPin = 2;

void setup() {
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  delay(200);

  Serial.begin(9600);
  resetSpeed();         // Set Speed and acceleration for Step Motors
  pinMode(12, OUTPUT);  //initialize Solenoid
  pinMode(3, OUTPUT);   //initialize LED
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

      case '6':  // SQUARE
        xCurrent = stepper_x.currentPosition();
        yCurrent = stepper_y.currentPosition();
        xTarget = xCurrent + 125;
        yTarget = yCurrent + 500;
        stepper_x.moveTo(xTarget);
        stepper_y.moveTo(yTarget);
        runMotors();

        startDraw();

        stepper_x.move(-250);
        runMotors();
        stepper_y.move(-1000);
        runMotors();
        stepper_x.move(250);
        runMotors();
        stepper_y.move(1000);
        runMotors();

        stopStay();
        break;

      case '7':  // Triangle (Simplified based on Ellie's code)
        {
          xCurrent = stepper_x.currentPosition();
          yCurrent = stepper_y.currentPosition();
          xTarget = xCurrent - 125;  //left
          yTarget = yCurrent - 300;  //up
          stepper_x.moveTo(xTarget);
          stepper_y.moveTo(yTarget);
          runMotors();

          //Set Speed for the Triangle
          stepper_x.setMaxSpeed(80);
          stepper_x.setAcceleration(100);
          stepper_y.setMaxSpeed(400);
          stepper_y.setAcceleration(450);

          startDraw();

          //Define running positions
          long xStart = stepper_x.currentPosition();
          long yStart = stepper_y.currentPosition();
          long sideX = 300;
          long sideY = 700;
          long xTarget[3] = { xStart, xStart + sideX, xStart + sideX / 2 };  //Points go right from the start point
          long yTarget[3] = { yStart, yStart, yStart + sideY };              //Points go down from the start point

          for (int i = 0; i <= 3; i++) {
            int next = i % 3;
            stepper_x.moveTo(xTarget[next]);
            stepper_y.moveTo(yTarget[next]);
            runMotors();
          }

          stopStay();
          resetSpeed();
          break;
        }  //exit case 7

        // ------------------------
      // Circle (Simplified based on Vic's code)
      // ------------------------
      case '8':
        {
          xCurrent = stepper_x.currentPosition();
          yCurrent = stepper_y.currentPosition();

          const long radius = 400;
          const int points = 180;
          const float scaleX = 0.30;
          const float scaleY = 1.02;

          xTarget = xCurrent + lround(radius * cos(0) * scaleX);
          yTarget = yCurrent + lround(radius * sin(0) * scaleY);
          stepper_x.moveTo(xTarget);
          stepper_y.moveTo(yTarget);
          runMotors();

          startDraw();

          stepper_x.setMaxSpeed(250);
          stepper_y.setMaxSpeed(250);
          stepper_x.setAcceleration(120);
          stepper_y.setAcceleration(120);

          for (int i = 1; i <= points; i++) {
            float theta = 2.0 * PI * i / points;
            
            long xTarget = xCurrent + lround(radius * cos(theta) * scaleX);
            long yTarget = yCurrent + lround(radius * sin(theta) * scaleY);
Serial.print(xTarget);
Serial.print(",");
Serial.println(yTarget);
            stepper_x.moveTo(xTarget);
            stepper_y.moveTo(yTarget);
            runMotors();
          }

          stopStay();
          resetSpeed();
          break;
        }  //exit case 8


      default:
        break;
    }
  }

  //run motors for moving commands (case 1-4)
  stepper_x.run();
  stepper_y.run();
}

void startDraw() {
  digitalWrite(12, HIGH);
  digitalWrite(3, HIGH);
  delay(1000);  // Initiate LED and Solenoid
}

void stopStay() {
  digitalWrite(12, LOW);
  digitalWrite(3, LOW);
  delay(1000);
  xCurrent = stepper_x.currentPosition();
  stepper_x.moveTo(xCurrent);
  yCurrent = stepper_y.currentPosition();
  stepper_y.moveTo(yCurrent);
}

void runMotors() {
  while (stepper_x.distanceToGo() != 0 || stepper_y.distanceToGo() != 0) {
    if (Serial.available() > 0) {
      stopDraw = Serial.read();
      if (stopDraw == '5') {
        stopStay();
        digitalWrite(resetPin, LOW);
        //break;
        Serial.println("This would never be printed");
      }  //exit if loop
    }    //exit if loop
    stepper_x.run();
    stepper_y.run();
  }  //exit while loop


}  // exit void

void resetSpeed() {
  stepper_x.setAcceleration(400.0);
  stepper_x.setMaxSpeed(550.0);
  stepper_y.setAcceleration(400.0);
  stepper_y.setMaxSpeed(550.0);
}
