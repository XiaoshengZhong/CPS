/*Oct 26, codes to test for monday, Jimmy
  Test TWO!
**Goals:
  1) Test the Code structures of inputing diverse commands for 1 stepper motor
  2) Test the max speed of the stepper motor
**Referenced Examples:
  https://curiousscientist.tech/blog/arduino-accelstepper-tb6600-stepper-motor


    "R" - Change speed, eg. type in R400

    "A" - Keep Moving to Positive direction
    "S" - Stop immediately
    "D" - Keep Moving to Negative direction

    "T" - Move to Positive direction, then move to negative direction at HALF SPEED 
    enter two numbers behind for Positive steps and negative steps, eg "T500 250"
    (As a test to draw shapes)

*/

//Need to Modify based on previous Test ONE codes
#include <AccelStepper.h>
// define step constant
#define FULLSTEP 4
#define STEP_PER_REVOLUTION 2048 // this value is from datasheet
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper_x(FULLSTEP, 11, 9, 10, 8);
AccelStepper stepper_y(FULLSTEP,  7, 5,  6, 4);


//User-defined values
long receivedSpeed = 550; //Steps / second
char receivedCommand;
//-------------------------------------------------------------------------------
bool newData, runallowed = false; // booleans for new data from serial, and runallowed flag


void setup()
{
  Serial.begin(9600); //define baud rate
  Serial.println("ready");

  //setting up some default values for maximum speed and maximum acceleration
  Serial.println("Default speed: 550");
  
  stepper_x.setMaxSpeed(receivedSpeed); //SPEED = Steps / second
  stepper_x.disableOutputs(); //disable outputs
}


void loop()
{
  checkSerial(); //check serial port for new commands
  RunTheMotor(); //function to handle the motor
}

void RunTheMotor() //function for the motor
{
  if (runallowed == true)
  {
    stepper_x.enableOutputs(); //enable pins
    stepper_x.run(); //step the motor (this will step the motor by 1 step at each loop)
  }
  else //program enters this part if the runallowed is FALSE, we do not do anything
  {
    stepper_x.disableOutputs(); //disable outputs
    return;
  }
}


void checkSerial() //function for receiving the commands
{
  if (Serial.available() > 0) //if something comes from the computer
  {
    receivedCommand = Serial.read(); // pass the value to the receivedCommad variable
    newData = true; //indicate that there is a new data by setting this bool to true

    if (newData == true) //we only enter this long switch-case statement if there is a new command from the computer
    {
      switch (receivedCommand) //we check what is the command
      {

        case 'R': // set rotate speed, For example, type in R400
          runallowed = false;
          receivedSpeed = Serial.parseFloat(); //value for the speed
          Serial.print("speed =");
          Serial.println(receivedSpeed);
          stepper_x.setMaxSpeed(receivedSpeed);
          break;

        /*case 'A'://move to positive direction, just type in A
          Serial.println("Positive direction."); //print the action
          runallowed = true; //allow running - this allows entering the RunTheMotor() function.
          //stepper_x.setMaxSpeed(receivedSpeed); //set speed
          //stepper_x.setSpeed(receivedSpeed);	
          stepper_x.move(500); //set relative distance and direction
          break;

        /* case 'S':  // Stop the motor, just type in S
          //stepper_x.stop(); //stop motor
          stepper_x.disableOutputs(); //disable power
          Serial.println("Stopped."); //print action
          runallowed = false; //disable running
          break;

        /*case 'D':// move to negative direction, just type in D
          Serial.println("Negative direction."); //print the action
          runallowed = true; //allow running - this allows entering the RunTheMotor() function.
          //stepper_x.setMaxSpeed(receivedSpeed); //set speed
          //stepper_x.setSpeed(receivedSpeed);	
          stepper_x.move(-500); //set relative distance and direction
          break;
          */

        case 'T':         // Move to positive direction for specific steps
                          //then move to negative direction for specific steps at half speed
                          //For example, type in "T500 250"
          //long pSteps = Serial.parseFloat(); //Number of Positive steps
          //long nSteps = Serial.parseFloat(); // Number of Nefative steps
          Serial.println("T");
          runallowed = true; //allow running - this allows entering the RunTheMotor() function.
          stepper_x.move(0); //set relative distance and direction
          stepper_x.setMaxSpeed(receivedSpeed/2);
          stepper_x.move(2048); //set relative distance and direction
          stepper_x.setMaxSpeed(receivedSpeed);
          break;

        default:
          break;
      }
    }
    //after we went through the above tasks, newData is set to false again, so we are ready to receive new commands again.
    newData = false;
  }
}
