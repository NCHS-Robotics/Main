#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LBdrive = motor(PORT1, ratio6_1, false);
motor LFdrive = motor(PORT11, ratio6_1, false);
motor RBdrive = motor(PORT2, ratio6_1, true);
motor RFdrive = motor(PORT4, ratio6_1, true);
motor IntakeMotor = motor(PORT5, ratio18_1, false);
motor ShootClose = motor(PORT6, ratio18_1, false);
motor ShootFar = motor(PORT7, ratio18_1, false);
controller Controller1 = controller(primary);
motor Lift = motor(PORT8, ratio18_1, false);
inertial Inertial = inertial(PORT12);
limit LimitSwitchFar = limit(Brain.ThreeWirePort.A);
limit LimitSwitchIntake = limit(Brain.ThreeWirePort.B);
encoder EncoderC = encoder(Brain.ThreeWirePort.C);

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