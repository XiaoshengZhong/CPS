Software
We used a single Python script in Visual Studio Code to send and receive messages to and from Wekinator. The Python code utilizes MediaPipe (hand_pose), which we modified to fit the requirements of our project. Additionally, a Python-friendly environment was set up within Visual Studio Code to support MediaPipe.

In Arduino, we controlled two stepper motors and one solenoid. To achieve this, we used the AccelStepper library. The numeric messages received from Python corresponded to cases numbered 1 to 9.

Machine learning
In Wekinator, we used classifiers with two outputs, each having five classes. We chose classifiers because we were primarily interested in categorizing data into discrete classes (9 combinations). The machine learning model we used was k-nearest neighbors (k-NN), which achieved 99.78% and 99.74% accuracy for outputs 1 and 2, respectively.

We trained the model using hand gestures. Two-hand gestures corresponded to drawing commands, whereas one-hand gestures corresponded to movement commands.

Below is a breakdown of the combinations acquired by the two outputs and the corresponding command numbers sent to the Arduino:

WEKINATOR_TO_ARDUINO_MAP = {
("1", "1"): 5, # stop all (one-hand: palm)
("1", "2"): 8, # draw circle (two hands: circle gesture)
("1", "3"): 7, # draw triangle (two hands: triangle gesture)
("1", "4"): 6, # draw square (two hands: rectangle gesture)
("5", "5"): 9, # default (no hands shown)
("4", "5"): 1, # point left (one hand)
("3", "5"): 2, # point right (one hand)
("2", "5"): 3, # point down (one hand)
("1", "5"): 4, # point up (one hand)
}

Limitations
The biggest limitation in our training was likely diagonal hand positions. These positions were neither clearly vertical nor horizontal, which created ambiguity. The model would try to force them into one of the two categories, often resulting in multiple commands being triggered at once.

Future improvements
To collect more varied training examples, including faster motion of the hand while entering each pose, as well as diagonal poses. These diagonal gestures can then be classified as either vertical or horizontal movements to avoid triggering mixed commands. A straightforward improvement would also be to train the model with more users performing all gestures, and to explore different machine learning models for a better performance.

Mechanical & Hardware Designs
image
The solenoid and stepper motors are all saftly powered by an external power bank.

Possible short-term improvements for the project include redesigning the cup-clamping mechanism to provide greater stability and ensure the cup remains centered at all times. Additionally, the pen-holding mechanism could be improved by introducing a clip-on design rather than relying on screws to secure the pen in place, which would also reduce the overall weight of the mechanism.

Since we currently have a limited power supply, we would also replace the solenoid with a servo motor, which is more power-efficient. The servo would move between two fixed positions to attach and detach the pen from the cup reliably.
