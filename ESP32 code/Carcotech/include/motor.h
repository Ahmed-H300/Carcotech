#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>

// note that IN1 will represent direction while IN2 will represent the speed

// motor speed channels
#define MOTOR_L_SPEED_CHANNEL   0
#define MOTOR_R_SPEED_CHANNEL   1


// related to the motor pins
#define MOTOR_L_IN1_PIN     33      // direction pin
#define MOTOR_L_IN2_PIN     32      // speed pin

#define MOTOR_R_IN1_PIN     27      // direction pin
#define MOTOR_R_IN2_PIN     13      // speed pin


// @POSSIBLE_SPEED_VALS_PWM
#define MOTOR_SPEED_BREAK    0
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

// @WHICH_MOTOR
#define MOTOR_LEFT          0
#define MOTOR_RIGHT         1


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
    * @param LSpeed : this is the speed of the left wheel, refer to @POSSIBLE_SPEED_VALS_PWM to know the paramters of this function 
    * @param RSpeed : this is the speed of the right wheel,  refer to @POSSIBLE_SPEED_VALS_PWM to know the paramters of this function
   */
   void setSpeed(uint8_t LSpeed, uint8_t RSpeed);

    /**
     * @brief this function controls the direction of the motor
     * @param LDirection: this is the direction of the left wheel, refer to @POSSIBLE_DIRECTION to know possible parameters
     * @param RDirection: this is the direction of the right wheel, refer to @POSSIBLE_DIRECTION to know possible parameters 
    */
   void setDirection(uint8_t LDirection, uint8_t RDirection);


    /**
     * @brief this function is used to stop the motor immediatly
    */
    void stop();

    /**
     * @brief this function is used to stop left wheel of the motor immediatly
    */
    void stopLeftWheel();

    /**
     * @brief this function is used to stop right wheel of  the motor immediatly
    */
    void stopRightWheel();

    /**
     * @brief this function is used to increase the speed of the motor wheels
     * @param leftWheelIncreaseAmount: a value between 0 to 255 that tells the incrase amount of the speed of the left wheel
     * @param righWheelIncreaseAmount: a value between 0 to 255 that tells the incrase amount of the speed of the right wheel    
    */
    void increaseMotorSpeed(uint8_t leftWheelIncreaseAmount, uint8_t rightWheelIncreaseAmount);

    /**
     * @brief this function is used to decrease the speed of the motor wheels
     * @param leftWheelDecreaseAmount: a value between 0 to 255 that tells the incrase amount of the speed of the left wheel
     * @param righWheelDecreaseAmount: a value between 0 to 255 that tells the incrase amount of the speed of the right wheel    
    */
    void decreaseMotorSpeed(uint8_t leftWheelDecreaseAmount, uint8_t rightWheelDecreaseAmount);
    
};  


#endif