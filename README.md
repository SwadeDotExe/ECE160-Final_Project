# ECE160-Final_Project
ECE160 Final Project
Written By: Swade, Brendan, and Justin
Last Updated: 2/20/2021

This code contains all the information required to complete the final project
for ECE160, which is driving a robot (Boebot) to deliver an item of food from
a specified pickup location. During this task, a portion of it is required to
be done autonomously using line following.

The robot has two main modes of operation:

   Manual: Reads the user input from the PlayStation controller and moves the
            robot according to the input.
   Automatic: Firstly, spins roughly 180 degrees until a line is detected. Then,
              Reads the 3 line following sensors and makes judgements on what
              motion is needed to stay following the line. Continues until the
              intersection is detect, then spins 90 degrees and continues forward
              until manual mode is necessary.

Materials Used:
  - Boebot robot
  - Arduino Uno
  - Gripper w/ Servo Motor
  - 5x NiMH Batteries
  - PlayStation 2 Controller
  - PlayStation 2 Receiver
  - Line Following Module
  - 1x RGB LED
  - Various Resistors
  - Various Jumper Wires

Script Overview:
  - Sets up all the pins, variables, and libraries used in the script.
  - Sees what mode it is currently in (manual by default):
      - Manual
          - Read the controller input
          - Runs a function based off if button was pressed
          - Loop back to see what mode it is in
      - Automatic
          - Spin the robot ~160 degrees
          - Keeps spinning until line follower detects ideal
            conditions for line following
          - Begin line following loop
              - If left or right thresholds are met, spin opposite direction
              - If intersection conditions are met, spin in the direction
                of the intersection
              - Break when robot is in manual control area
          - Set mode to manual when robot exits line following area
