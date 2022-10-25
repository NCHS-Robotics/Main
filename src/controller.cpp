#include "main.h"

#define LEFT_FRONT_WHEEL_PORT 3
#define LEFT_BACK_WHEEL_PORT 11
#define RIGHT_FRONT_WHEEL_PORT 4
#define RIGHT_BACK_WHEEL_PORT 2
#define INTAKE_MOTOR_PORT 5

pros::Motor left_front_wheel(LEFT_FRONT_WHEEL_PORT, true); 
pros::Motor left_back_wheel(LEFT_BACK_WHEEL_PORT, true);
pros::Motor right_front_wheel(RIGHT_FRONT_WHEEL_PORT, true);
pros::Motor right_back_wheel(RIGHT_BACK_WHEEL_PORT, true);

pros::Motor intake_motor(INTAKE_MOTOR_PORT);

pros::Controller master (pros::E_CONTROLLER_MASTER);


void driveTask(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_back_wheel.move((dtX + dtY));
    left_front_wheel.move((dtX + dtY));
    right_back_wheel.move((dtX - dtY));
    right_front_wheel.move((dtX - dtY));
}


void intakeIn(void* param) {
    /**
        while (master.get_digital(DIGITAL_L1) == 1) {
            intake_motor.move_velocity(200);
        }
        intake_motor.move_velocity(0);
     **/ //doesn't stop

    //intake_motor.move_velocity(200 * master.get_digital(DIGITAL_L1));
    intake_motor.move_velocity(200 * (-1*(master.get_digital(DIGITAL_L2))));
}

void intakeOut(void* param) {
    /**
     while (master.get_digital(DIGITAL_L2) == 1) {
            intake_motor.move_velocity(-200);
        }
        intake_motor.move_velocity(0);
    **/ //doesn't stop    

    //intake_motor.move_velocity(-200 * master.get_digital(DIGITAL_L2));
    //intake_motor.move_velocity(200 * (-1*(master.get_digital(DIGITAL_L2))));
    intake_motor.move_velocity(200  * master.get_digital(DIGITAL_L2));
}

void intakeController(void* param) {
    int y = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    intake_motor.move(y);
}

/**
void intakeTask(void* param) {
    while (master.get_digital(DIGITAL_L1) == 1) {
            intake_motor.move_velocity(200);
        }
        intake_motor.move_velocity(0);
    while (master.get_digital(DIGITAL_L2) == 1) {
            intake_motor.move_velocity(-200);
        }
        intake_motor.move_velocity(0);    
    //intake_motor.move_velocity(200 * master.get_digital(DIGITAL_L1));
    //intake_motor.move_velocity(-200 * master.get_digital(DIGITAL_L2));
}
**/
/**
void leftBackDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_back_wheel.move((dtX + dtY));
}

void leftFrontDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_front_wheel.move((dtX + dtY));
}

void rightBackDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    right_back_wheel.move((dtX - dtY));
}

void rightFrontDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    right_front_wheel.move((dtX - dtY));
}
**/

void opcontrol() {
    while (true) {
        /**
        //drive code
        int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        left_back_wheel.move((dtX + dtY));
        left_front_wheel.move((dtX + dtY));
        right_back_wheel.move((dtX - dtY));
        right_front_wheel.move((dtX - dtY)); 
        **/


        /**
        //intake code
        while (master.get_digital(DIGITAL_L1) == 1) {
            intake_motor.move_velocity(200);
        }
        intake_motor.move_velocity(0);
        while (master.get_digital(DIGITAL_L2) == 1) {
            intake_motor.move_velocity(-200);
        }
        intake_motor.move_velocity(0);  
        **/
          
        //pros::Task leftBDrive(leftBackDrive);
        //pros::Task leftFDrive(leftFrontDrive);
        //pros::Task rightBDrive(rightBackDrive);
        //pros::Task rightFDrive(rightFrontDrive);  
        pros::Task drive(driveTask);

        //pros::Task intake(intakeTask);
        //pros::Task intakeI(intakeIn);
        //pros::Task intakeO(intakeOut);
        pros::Task intakeC(intakeController);

        

        pros::delay(2);
    }
}