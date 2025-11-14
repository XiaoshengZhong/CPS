/*Oct 26, codes to test for monday, Jimmy
Test ONE!
**Goals: 
1)Test pins layout for 2 stepper motor + 1 solenoid
2)Allow each actuator to run independently
**Referenced Examples: 
1) Example of using two 28BJY-48 stepper motors:
https://arduinogetstarted.com/tutorials/arduino-controls-28byj-48-stepper-motor-using-uln2003-driver
When finished testing, we'll stay with the pin layout and settings for all the actuators. 
2) Example of using millis() function to run the solenoid independently, instead of delay() that may stop other actuators:
https://www.programmingelectronics.com/arduino-sketch-with-millis-instead-of-delay/
*/


// Include the AccelStepper Library
#include <AccelStepper.h>

// define step constant
#define FULLSTEP 4
#define STEP_PER_REVOLUTION 2048 // this value is from datasheet

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper_1(FULLSTEP, 11, 9, 10, 8);
AccelStepper stepper_2(FULLSTEP,  7, 5,  6, 4);

//Start counting time for solenoid, if we don't have it yet, we could use the LED
const unsigned long eventInterval = 1000;
unsigned long previousTime = 0;
int drawStatus = 1; 

void setup() {
  Serial.begin(9600);

  //stepper_1.setMaxSpeed(1000.0);   // set the maximum speed
  //stepper_1.setAcceleration(50.0); // set acceleration
  stepper_1.setSpeed(80000);         // set initial speed
  stepper_1.setCurrentPosition(0); // set position
  stepper_1.moveTo(STEP_PER_REVOLUTION); // set target position: 64 steps <=> one revolution

 // stepper_2.setMaxSpeed(1000.0);   // set the maximum speed
  //stepper_2.setAcceleration(50.0); // set acceleration
  stepper_2.setSpeed(40000);         // set initial speed
  stepper_2.setCurrentPosition(0); // set position
  stepper_2.moveTo(STEP_PER_REVOLUTION); // set target position: 64 steps <=> one revolution

  pinMode (12, OUTPUT); // set output pin for solenoid to pin 12
}

void loop() {
  // change direction once the motor reaches target position
  if (stepper_1.distanceToGo() == 0)
    stepper_1.moveTo(-stepper_1.currentPosition());

  if (stepper_2.distanceToGo() == 0)
    stepper_2.moveTo(-stepper_2.currentPosition());

  stepper_1.run(); // MUST be called in loop() function
  stepper_2.run(); // MUST be called in loop() function

  Serial.print(F("stepper_1# current position: "));
  Serial.println(stepper_1.currentPosition());

  Serial.print(F("stepper_2# current position: "));
  Serial.println(stepper_2.currentPosition());

unsigned long currentTime = millis();
 if (currentTime - previousTime >= eventInterval){
  drawStatus = - drawStatus;
  previousTime = currentTime;
   }

   if (drawStatus >= 0){
    digitalWrite (12, HIGH);
   } else {
    digitalWrite (12, LOW);
   }


}
