/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LBdrive              motor         1               
// LFdrive              motor         11              
// RBdrive              motor         2               
// RFdrive              motor         4               
// IntakeMotor          motor         5               
// ShootClose           motor         6               
// ShootFar             motor         7               
// Controller1          controller                    
// Lift                 motor         8               
// LimitSwitchFar       limit         A               
// LimitSwitchClose     limit         B               
// Inertial             inertial      12              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

/*
PRE-AUTON
*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Lift.setVelocity(100, percent);
  IntakeMotor.setVelocity(100, percent);
}

/*
AUTON
*/

//set drive percentage
void setDrivePercentage(int p) {
  LFdrive.setVelocity(p, percent);
  LBdrive.setVelocity(p, percent);
  RFdrive.setVelocity(p, percent);
  RBdrive.setVelocity(p, percent);
}

//spin all the motors in a given direction
void driveAll(vex::directionType dir) {
  LFdrive.spin(dir);
  LBdrive.spin(dir);
  RFdrive.spin(dir);
  RBdrive.spin(dir);
}

//driveAll with set degrees
void driveAllFor(vex::directionType dir, int degrees) {
  LFdrive.startRotateFor(dir, degrees, deg);
  LBdrive.startRotateFor(dir, degrees, deg);
  RFdrive.startRotateFor(dir, degrees, deg);
  RBdrive.spinFor(dir, degrees, deg);
}

//stop all the motors given a brakeType
void stopAll(vex::brakeType stopType) {
  LFdrive.stop(stopType);
  LBdrive.stop(stopType);
  RFdrive.stop(stopType);
  RBdrive.stop(stopType);
}

//turning w/ inertial sensor
void turnLeftInertial(int headingVal) {
  setDrivePercentage(20);
  Inertial.setHeading(355, degrees);

  while(Inertial.heading(degrees) >= (360 - headingVal) || (Inertial.heading(degrees) < 1)) {
    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  stopAll(brake);
  wait(0.5, sec);
}

void turnRightInertial(int headingVal) {
  setDrivePercentage(20);
  Inertial.setHeading(5, degrees);

  while(Inertial.heading(degrees) <= headingVal || (Inertial.heading(degrees) > 359)) {
    LFdrive.spin(reverse);
    LBdrive.spin(reverse);
    RFdrive.spin(forward);
    RBdrive.spin(forward);
  }
  stopAll(brake);
  wait(0.5, sec);
}

//shoot the discs
void shootDiscs(double volts) {
  ShootClose.spin(forward, volts, volt);
  ShootFar.spin(forward, volts, volt);
}

//stop the discs
void stopDiscs() {
  IntakeMotor.stop(brake);
  ShootClose.stop(brake);
  ShootFar.stop(brake);
}

//15 sec auton
void autonomous(void) {
  IntakeMotor.spin(forward);
  wait(0.3, sec);
  IntakeMotor.stop();
  driveAllFor(forward, 400);  
  turnLeftInertial(6);
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);
  shootDiscs(8.5);
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  wait(1, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
}

/*
USER CONTROL
*/

void usercontrol(void) {
  while (1) {

    //finished controller code here

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
