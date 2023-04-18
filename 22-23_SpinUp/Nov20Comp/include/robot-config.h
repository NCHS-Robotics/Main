using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LFdrive;
extern motor LBdrive;
extern motor RFdrive;
extern motor RBdrive;
extern motor IntakeMotor;
extern motor ShootClose;
extern motor ShootFar;
extern controller Controller1;
extern inertial Inertial;
extern motor Endgame;
extern gyro Gyro;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );