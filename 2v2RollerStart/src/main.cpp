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
// LimitSwitchIntake    limit         B               
// Inertial             inertial      12              
// EncoderC             encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;
using namespace std;

//initialize tasks
vex::task liftArmIntakeTask;
vex::task liftArmFarTask;

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

//lift arm up task
int liftArmIntake() {
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("lift intake thread");
  if (Controller1.ButtonX.pressing()) {
      while(!(LimitSwitchIntake.pressing())) {
        Lift.spin(forward);
      }
      Lift.stop();
    }
  else {
    Lift.stop();
  }
  return 0;
}

//lift arm down task
int liftArmFar() {
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("lift arm thread");
  if (Controller1.ButtonB.pressing()) {
      while(!(LimitSwitchFar.pressing())) {
        Lift.spin(reverse);
      }
      Lift.stop(brake);
    }
  else {
    Lift.stop();
  }
  return 0;
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
  double turnImportance = 0.5;

  //initialize sensors and motor speeds
  Lift.setPosition(0, degrees);
  Inertial.resetHeading();
  Inertial.resetRotation();
  Lift.setVelocity(100, percent);
  IntakeMotor.setVelocity(100, percent);

  //define tasks
  liftArmFarTask = vex::task(liftArmFar);
  liftArmIntakeTask = vex::task(liftArmIntake);

 
  // place driver control in this while loop
  while (true) {

    /*
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1,40,"RPM:%f",ShootClose.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,80,"RPM:%f",ShootFar.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,120,"RPM:%f",IntakeMotor.velocity(vex::velocityUnits::rpm));\
    Brain.Screen.render();
    */
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print(EncoderC.velocity(rpm));
    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print(ShootClose.velocity(rpm));
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print(ShootFar.velocity(rpm));
    double turnVal = Controller1.Axis4.position(percent);
    double forwardVal = Controller1.Axis3.position(percent);

    double turnVolts = -(turnVal) * 0.12;
    double forwardVolts = forwardVal * 0.12 * (1 - (std::abs(turnVolts)/12.0) * turnImportance);

    LFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);

    //double suckVal = Controller1.ButtonL1.pressing();
    //double suckVolts = suckVal * 0.12;
    
    if (Controller1.ButtonL2.pressing()){ //in
      IntakeMotor.spin(forward, -12.0 , voltageUnits::volt);
    }
    else if (Controller1.ButtonL1.pressing()){ //out
      IntakeMotor.spin(forward, 12.0 , voltageUnits::volt);
    }
    else{
      IntakeMotor.stop();
    }
    
    //user shoot that prevents voltage dropoff
    int r = 2100;
    int rpmPrev = 2100;
    int temp = 2100;
    //changed v from int to double
    double v = 6.25;
    //user shoot
    if (Controller1.ButtonR1.pressing()) {   
      temp = r;
      r = EncoderC.velocity(rpm);
      rpmPrev = temp;
      if(rpmPrev - r >= 100) {
        v = 10;
      }
      ShootClose.spin(forward, v, volt); //7
      ShootFar.spin(reverse, v, volt); //7
      v = 6.25;
      
    }
    else {
      ShootClose.stop(brake);
      ShootFar.stop(brake);
    }

    /*
    //lift arm intake
    if (Controller1.ButtonX.pressing()) {
      liftArmIntake();
    }

    //lift arm far
    if (Controller1.ButtonB.pressing()) {
      liftArmFar();
    }
    */

    //lift manual up (rebinded to X and B until lift can move with sensors without stopping everything else)
    if (Controller1.ButtonX.pressing()) {
      Lift.spin(forward);
    } else if (Controller1.ButtonB.pressing()){
      Lift.spin(reverse);
    } else {
      Lift.stop();
    }

    //moving the lift to the limit sensors
    if (Controller1.ButtonX.pressing()) {
      while(!(LimitSwitchFar.pressing())) {
        Lift.spin(reverse);
      }
      Lift.stop(brake);
    }
    if (Controller1.ButtonA.pressing()) {
      while(!(LimitSwitchIntake.pressing())) {
        Lift.spin(forward);
      }
      Lift.stop();
    }

    //distance shoot
    if (Controller1.ButtonR2.pressing()) {
      ShootClose.spin(forward, 12, volt);
      ShootFar.spin(reverse, 12, volt);
    }

    //flywheel spins backwards to activate endgame
    if (Controller1.ButtonUp.pressing()) {
      ShootClose.spin(forward, -12, volt);
      ShootFar.spin(reverse, -12, volt);
    }

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
