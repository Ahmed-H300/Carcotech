#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>
#include "constants.h"

// note that IN1 will represent direction while IN2 will represent the speed

// motor speed channels
#define MOTOR_L_SPEED_CHANNEL   0
#define MOTOR_R_SPEED_CHANNEL   1


// related to the motor pins
#define MOTOR_L_SENS1_PIN   26
#define MOTOR_L_SENS2_PIN   14

#define MOTOR_R_SENS1_PIN   18
#define MOTOR_R_SENS2_PIN   5

#define MOTOR_L_IN1_PIN     33      // direction pin
#define MOTOR_L_IN2_PIN     32      // speed pin

#define MOTOR_R_IN1_PIN     27      // direction pin
#define MOTOR_R_IN2_PIN     13      // speed pin


// @POSSIBLE_SPEED_VALS
#define MOTOR_SPEED_1       25
#define MOTOR_SPEED_2       50
#define MOTOR_SPEED_3       75
#define MOTOR_SPEED_4       100
#define MOTOR_SPEED_5       125
#define MOTOR_SPEED_6       150
#define MOTOR_SPEED_7       175
#define MOTOR_SPEED_8       200
#define MOTOR_SPEED_9       225
#define MOTOR_SPEED_MAX     255


// @POSSIBLE_DIRECTION
#define MOTOR_DIR_FORWARD   1
#define MOTOR_DIR_BACKWARD  0


// @POSSIBLE_ANGLE_VALS
#define MOTOR_ANGLE_180             0
#define MOTOR_ANGLE_NEG_90          1
#define MOTOR_ANGLE_90              2


// values for needed amount of rotation for car
#define MOTOR_NUM_OF_ROT_180              50
#define MOTOR_NUM_OF_ROT_90               10


/*this is a variable that indicates the number of rotations made by the left wheel*/
static volatile float_t numOfRotationsLeft = 0;

/*this is a variable that indicates the number of rotations made by the right wheel*/
static volatile float_t numOfRotationsRight = 0;

/*this is a counter to count the number of pulses maded by the left wheel*/
static volatile int counterLeft = 0;

/*this is a counter to count the number of pulses maded by the right wheel*/
static volatile int counterRight = 0;


class Motor
{
private:

    /*this is the current speed of the left wheel*/
    uint8_t leftWheelSpeed = 0;

    /*this is the current speed of the right wheel*/
    uint8_t rightWheelSpeed = 0;
    
    /*this is the current direction of the left wheel*/
    uint8_t leftWheelDirection = MOTOR_DIR_FORWARD;

    /*this is the current direction of the right wheel*/
    uint8_t rightWheelDirection = MOTOR_DIR_FORWARD;
    
public:

    /**
     * @brief this is the initialization function , it has to be called before using any other functions
    */
   void init();

   /**
    * @brief this function sets the speed of motor
    * @param LSpeed : this is the speed of the left wheel, refer to @POSSIBLE_SPEED_VALS to know the paramters of this function 
    * @param RSpeed : this is the speed of the right wheel,  refer to @POSSIBLE_SPEED_VALS to know the paramters of this function
   */
   void setSpeed(uint8_t LSpeed, uint8_t RSpeed);

    /**
     * @brief this function controls the direction of the motor
     * @param LDirection: this is the direction of the left wheel, refer to @POSSIBLE_DIRECTION to know possible parameters
     * @param RDirection: this is the direction of the right wheel, refer to @POSSIBLE_DIRECTION to know possible parameters 
    */
   void setDirection(uint8_t LDirection, uint8_t RDirection);


   /**
    * @brief this function makes the car turns in place by a specific angle
    * @param angleValue: this is the angle by which the car will rotate around itself, refer to 
   */
   void rotate(uint8_t angleValue);


   /**
    * @brief this function only zeros out the counter to count the number of iterations
   */
   void beginCounting();
    

    /**
     * @brief this function gets the average number of needed rotations
     * @return float_t which represents the average number of rotations made by the motor since beginCounting() was called
    */
    float_t getAvgNumOfRotations();

    /**
     * @brief this function disable the external interrupts given back by the motor feedback
    */
    void turnOffInterrups();

    /**
     * @brief this function is used to stop the motor immediatly
    */
    void stop();
};  


#endif