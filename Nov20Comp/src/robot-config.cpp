#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LFdrive = motor(PORT3, ratio6_1, false);
motor LBdrive = motor(PORT1, ratio6_1, false);
motor RFdrive = motor(PORT4, ratio6_1, true);
motor RBdrive = motor(PORT2, ratio6_1, true);
motor IntakeMotor = motor(PORT5, ratio36_1, false);
motor ShootClose = motor(PORT8, ratio18_1, false);
motor ShootFar = motor(PORT10, ratio18_1, true);
controller Controller1 = controller(primary);
inertial Inertial = inertial(PORT11);
motor Endgame = motor(PORT19, ratio18_1, false);
gyro Gyro = gyro(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}