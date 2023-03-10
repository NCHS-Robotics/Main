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
// Inertial             inertial      10              
// LimitSwitchFar       limit         A               
// LimitSwitchIntake    limit         B               
// ShaftEncoderFlywheel encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;
using namespace std;


// A global instance of competition
competition Competition;

vex::controller::lcd ControllerScreen = vex::controller::lcd();

//initialize tasks
vex::task liftArmIntakeTask;
vex::task liftArmFarTask;

//initialize shoot discs variables
bool enableShooter;
int r = 0;
double v = 0.0;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

}

//lift arm up task
int liftArmIntake() {
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("lift intake thread");
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(hold);
  
  return 0;
}

//lift arm down task
int liftArmFar() {
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("lift arm thread");
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(hold);

  return 0;
}

//calculate voltage for shooter
int vCalc(int v) {
  while(ShaftEncoderFlywheel.velocity(rpm) <= 2500 || v >= 12) {
    v = v * 1.2;
  }
  return v;
}

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

//spin all the motors given a direction and voltage
void spinVolts(directionType dir, int volts) {
  LFdrive.spin(dir, volts, volt);
  LBdrive.spin(dir, volts, volt);
  RFdrive.spin(dir, volts, volt);
  RBdrive.spin(dir, volts, volt);
}

//turn the robot
void turnLeft(double volts) {
  LFdrive.spin(forward, volts, volt);
  LBdrive.spin(forward, volts, volt);
  RFdrive.spin(reverse, volts, volt);
  RBdrive.spin(reverse, volts, volt);
}

void turnRight(double volts) {
  LFdrive.spin(reverse, volts, volt);
  LBdrive.spin(reverse, volts, volt);
  RFdrive.spin(forward, volts, volt);
  RBdrive.spin(forward, volts, volt);
}

//turn the robot for degrees
void turnLeftFor(int degrees) {
  LFdrive.startRotateFor(forward, degrees, deg);
  LBdrive.startRotateFor(forward, degrees, deg);
  RFdrive.startRotateFor(reverse, degrees, deg);
  RBdrive.spinFor(reverse, degrees, deg);
}

void turnRightFor(int degrees) {
  LFdrive.startRotateFor(reverse, degrees, deg);
  LBdrive.startRotateFor(reverse, degrees, deg);
  RFdrive.startRotateFor(forward, degrees, deg);
  RBdrive.spinFor(forward, degrees, deg);
}

//using inertial sensor
void turnLeftInertial(int headingVal) {
  Inertial.setHeading(355, degrees);

  while(Inertial.heading(degrees) >= (360 - headingVal) || (Inertial.heading(degrees) < 1)) {
    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  stopAll(brake);
  wait(0.1, sec);
}

void turnRightInertial(int headingVal) {
  Inertial.setHeading(5, degrees);

  while(Inertial.heading(degrees) <= headingVal || (Inertial.heading(degrees) > 359)) {
    LFdrive.spin(reverse);
    LBdrive.spin(reverse);
    RFdrive.spin(forward);
    RBdrive.spin(forward);
  }
  stopAll(brake);
  wait(0.1, sec);
}

//shoot the discs
void shootDiscs(double volts) {
  ShootClose.spin(forward, volts, volt);
  ShootFar.spin(forward, volts, volt);
}

void shootDiscsFor(int deg) {
  ShootClose.spinFor(forward, deg, degrees);
  ShootFar.spinFor(forward, deg, degrees);
}

//stop the flywheel
void stopDiscs() {
  IntakeMotor.stop(brake);
  ShootClose.stop(brake);
  ShootFar.stop(brake);
  enableShooter = false;
}

//reset the lift for shooting far
int resetLiftFar() {
  while (!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);

  return 0;
}

//reset the lift for the intake
int resetLiftIntake() {
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(brake);

  return 0;
}

void autonomous(void) {
  setDrivePercentage(35);

  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();

  liftFarTask.resume();
  
  driveAllFor(forward, 1500);
  turnLeftInertial(74);
  shootDiscs(12);
  wait(3.75, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(1.35, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  liftFarTask.suspend();
  liftIntakeTask.resume();

  turnLeftInertial(6);
  IntakeMotor.spin(forward);
  driveAll(reverse);
  wait(0.55, sec);//calibrate timing at state
  IntakeMotor.stop();

  driveAllFor(forward, 500);
  turnRightInertial(122);
  resetLiftIntake();
  setDrivePercentage(60);
  IntakeMotor.spin(reverse);
  driveAllFor(reverse, 4400);
  IntakeMotor.stop();
}

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
  liftArmFarTask.suspend();
  liftArmIntakeTask = vex::task(liftArmIntake);
  liftArmIntakeTask.suspend();

 
  // place driver control in this while loop
  while (true) {

    /*
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1,40,"RPM:%f",ShootClose.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,80,"RPM:%f",ShootFar.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,120,"RPM:%f",IntakeMotor.velocity(vex::velocityUnits::rpm));\
    Brain.Screen.render();
    
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print(EncoderC.velocity(rpm));
    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print(ShootClose.velocity(rpm));
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print(ShootFar.velocity(rpm));
    */
    double turnVal = Controller1.Axis4.position(percent);
    double forwardVal = Controller1.Axis3.position(percent);

    double turnVolts = -(turnVal) * 0.12;
    double forwardVolts = forwardVal * 0.12 * (1 - (std::abs(turnVolts)/12.0) * turnImportance);

    LFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);

    //ControllerScreen.clearScreen();
    //ControllerScreen.setCursor(0,0);
    //ControllerScreen.print(Inertial.heading(degrees));
    
    
    if (Controller1.ButtonL2.pressing()){ //in
      IntakeMotor.spin(forward, 12.0 , voltageUnits::volt);
    }
    else if (Controller1.ButtonL1.pressing()){ //out
      IntakeMotor.spin(forward, -12.0 , voltageUnits::volt);
    }
    else{
      IntakeMotor.stop();
    }
    
    //user shoot that prevents voltage dropoff
    int r = 1800; //2100 --> 1800 --> 500 (target rpm)
    int rpmPrev = 1800;
    int temp = 1800;
    //changed v from int to double
    double v = 7.25; //6.25 --> 5.25
    //user shoot
    if (Controller1.ButtonR1.pressing()) {   
      temp = r;
      r = ShaftEncoderFlywheel.velocity(rpm);
      rpmPrev = temp;
      if(rpmPrev - r >= 70) {
        v = 12;
        
      }
      ShootClose.spin(forward, 12, volt); //7
      ShootFar.spin(forward, 12, volt); //7
      v = v >= 7.25 ? v * .9 : 7.25;
      
    }
    else {
      ShootClose.stop(brake);
      ShootFar.stop(brake);
    }

    //moving the lift 
    //far
    if (Controller1.ButtonB.pressing()) {
      Lift.spin(reverse);
    } 
    else if (Controller1.ButtonY.pressing()) {
      liftArmFarTask.resume();
      if (liftArmFar() == 0) {
        liftArmFarTask.suspend();
      }
    }
    //intake
    else if (Controller1.ButtonX.pressing()){
      Lift.spin(forward);
    } 
    else if (Controller1.ButtonA.pressing()) {
      liftArmIntakeTask.resume();
      if (liftArmIntake() == 0) {
        liftArmIntakeTask.suspend();
      }
    }
    else {
      Lift.stop(hold);
    }
    
    //distance shoot
    if (Controller1.ButtonR2.pressing()) {
      ShootClose.spin(forward, 12, volt);
      ShootFar.spin(forward, 12, volt);
    }

    //flywheel spins backwards to activate endgame
    if (Controller1.ButtonUp.pressing()) {
      ShootClose.spin(forward, -12, volt);
      ShootFar.spin(forward, -12, volt);
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
