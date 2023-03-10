// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LFdrive              motor         3               
// LBdrive              motor         1               
// RFdrive              motor         4               
// RBdrive              motor         2               
// IntakeMotor          motor         5               
// ShootClose           motor         8               
// ShootFar             motor         10              
// Inertial             inertial      11              
// Endgame              motor         19              
// Gyro                 gyro          A               
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Pushkar Betsur                                   */
/*    Created:      Mon Nov 14 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

vex::controller::lcd ControllerScreen = vex::controller::lcd();

//prime first 3 motors
void primeFirst3(int deg) {
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(forward, deg, degrees);
}

//drive laterally
void driveLateral(int deg) {
  primeFirst3(deg);
  RBdrive.spinFor(forward, deg, degrees);
}

//turn the robot
void turnRobot(int deg) {
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(reverse, deg, degrees);
  RBdrive.spinFor(reverse, deg, degrees);
}

//spin all drive motors
void spinAllDriveForward() {
  LFdrive.spin(forward);
  LBdrive.spin(forward);
  RFdrive.spin(forward);
  RBdrive.spin(forward);
}

void spinAllDriveReverse() {
  LFdrive.spin(reverse);
  LBdrive.spin(reverse);
  RFdrive.spin(reverse);
  RBdrive.spin(reverse);
}

//stop all drive motors
void stopAllDrive() {
  LFdrive.stop(brakeType::brake);
  LBdrive.stop(brakeType::brake);
  RFdrive.stop(brakeType::brake);
  RBdrive.stop(brakeType::brake);
}

//set motor velocity percentage
void setDrivePercentage(int percentage) {
  LFdrive.setVelocity(percentage, percent);
  LBdrive.setVelocity(percentage, percent);
  RFdrive.setVelocity(percentage, percent);
  RBdrive.setVelocity(percentage, percent);
}

void setMotorPercentage(int percentage) {
  setDrivePercentage(percentage);
  IntakeMotor.setVelocity(percentage, percent);
  ShootClose.setVelocity(percentage, percent);
  ShootFar.setVelocity(percentage, percent);
}

//shoot discs out of robot
//@param intakeDeg = degrees to spin intake
void shootDiscs(int intakeDeg) {
  ShootClose.spin(forward);
  ShootFar.spin(forward);
  wait (2, sec);
  IntakeMotor.spinFor(reverse, intakeDeg, deg);
  ShootClose.stop(brakeType::brake);
  ShootFar.stop(brakeType::brake);
  IntakeMotor.stop(brakeType::brake);
}

void auton() {
  //front: brain
  //back: intake
  //clockwise: negative
  //counter-clockwise: positive

  //set motor speeds
  setMotorPercentage(100);
  setDrivePercentage(35);

  //shoot two preloads in blue hoop
  shootDiscs(800);
  
  //roll top rollers
  //top blue
  driveLateral(-3600);
  turnRobot(650);

  setDrivePercentage(15);
  spinAllDriveReverse();

  wait(1.5, sec);
  IntakeMotor.spin(forward);
  wait(0.6, sec);
  IntakeMotor.stop(brakeType::brake);
  stopAllDrive();

  //top red
  setDrivePercentage(50);
  driveLateral(1400);
  setDrivePercentage(35);
  turnRobot(-695);
  spinAllDriveReverse();
  IntakeMotor.spin(reverse);
  wait(3.15, sec);
  stopAllDrive();
  IntakeMotor.stop(brakeType::brake);
  driveLateral(720);
  IntakeMotor.spinFor(forward, 450, degrees);

  setDrivePercentage(35);

  /*

  //NEW AUTON

  //align with wall, pick to middle blue discs and shoot them
  //align
  turnRobot(695);
  driveLateral(1500);
  turnRobot(695);
  driveLateral(3000);

  //shoot picked up disc
  driveLateral(-720);
  turnRobot(-695);
  driveLateral(1850);
  shootDiscs(1000);

  //go back to align w/ wall
  driveLateral(-2000);
  turnRobot(695);
  driveLateral(3000);

  //mid red discs
  driveLateral(-1450);
  turnRobot(375);
  IntakeMotor.spin(reverse);
  driveLateral(-1450);
  IntakeMotor.stop(brakeType::brake);

  driveLateral(1450);
  turnRobot(-1250);
  driveLateral(1850);

  shootDiscs(1200);

  //go to bottom rollers
  driveLateral(-1850);
  turnRobot(1250);
  setDrivePercentage(50);
  driveLateral(-6000);
  setDrivePercentage(35);
  turnRobot(375);

  //bottom red roller
  setDrivePercentage(15);
  spinAllDriveReverse();

  wait(1.5, sec);
  IntakeMotor.spinFor(forward, -700, degrees);
  stopAllDrive();

  //bottom blue
  setDrivePercentage(50);
  driveLateral(1400);
  setDrivePercentage(35);
  turnRobot(-695);
  spinAllDriveReverse();
  IntakeMotor.spin(reverse);
  wait(2.75, sec);
  stopAllDrive();
  IntakeMotor.stop(brakeType::brake);
  driveLateral(720);
  IntakeMotor.spinFor(forward, 450, degrees);

  //Endgame
  Endgame.spin(forward);
  */

  

  //OLD AUTON

  //align with wall, pick to middle blue discs and shoot them
  //align
  turnRobot(695);
  driveLateral(1500);
  turnRobot(695);
  setDrivePercentage(100);
  driveLateral(720);
  setDrivePercentage(35);

  //shoot picked up disc
  driveLateral(-720);
  turnRobot(-695);
  driveLateral(2050);
  shootDiscs(1000);

  //go back to align w/ wall
  driveLateral(-2000);
  turnRobot(695);
  setDrivePercentage(100);
  driveLateral(720);
  setDrivePercentage(35);

  //mid red discs
  driveLateral(-1450);
  turnRobot(375);
  IntakeMotor.spin(reverse);
  driveLateral(-1650);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-900);
  driveLateral(750);

  shootDiscs(1200);

  driveLateral(-750);
  turnRobot(900);
  IntakeMotor.spin(reverse);
  driveLateral(-1500);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-800);
  //driveLateral(780);

  shootDiscs(1000);

  //endgame
  Endgame.spinFor(forward, 180, degrees);
  

  
  

  /** continute auton after batavia
  //roll bottom rollers
  setDrivePercentage(50);

  driveLateral(-720);
  turnRobot(1125);
  driveLateral(720);
  turnRobot(750);
  driveLateral(-360);
  IntakeMotor.spinFor(forward, 720, degrees);

  driveLateral(720);
  turnRobot(750);
  driveLateral(-720);
  IntakeMotor.spinFor(forward, 720, degrees);

  //align with wall, pick up red middle discs and shoot them
  driveLateral(360);
  turnRobot(750);
  driveLateral(720);
  turnRobot(750);
  driveLateral(1200);

  setDrivePercentage(25);

  driveLateral(-720);
  turnRobot(1125);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-750);
  driveLateral(500);

  shootDiscs(1000);

  turnRobot(750);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-1125);
  driveLateral(720);

  shootDiscs(1000);

  //endgame
  Endgame.spin(forward);
  **/
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  /*
  ControllerScreen.clearScreen();
  ControllerScreen.print("Autonomous Started");
  */
  
  auton();

}

