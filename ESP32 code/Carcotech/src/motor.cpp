#include "motor.h"


void Motor::init()
{

    /*make motor direction pins as output*/
    pinMode(MOTOR_R_IN1_PIN, OUTPUT);
    pinMode(MOTOR_L_IN1_PIN, OUTPUT);

    /*put initial values on the pins*/
    digitalWrite(MOTOR_R_IN1_PIN, LOW);
    digitalWrite(MOTOR_L_IN1_PIN, LOW);
    
    /*initalize speed channels*/
    ledcSetup(MOTOR_L_SPEED_CHANNEL, 4000, 8); 
    ledcSetup(MOTOR_R_SPEED_CHANNEL, 4000, 8);

    /*assign speed control PWM channels to speed control pins*/
    ledcAttachPin(MOTOR_L_IN2_PIN, MOTOR_L_SPEED_CHANNEL);
    ledcAttachPin(MOTOR_R_IN2_PIN, MOTOR_R_SPEED_CHANNEL);

    /*intialize the pins*/
    ledcWrite(MOTOR_L_SPEED_CHANNEL, 0);
    ledcWrite(MOTOR_R_SPEED_CHANNEL, 0);

}

void Motor::setSpeed(uint8_t LSpeed, uint8_t RSpeed)
{   
    
    /*store the values of the speed*/
    leftWheelSpeed = LSpeed;
    rightWheelSpeed = RSpeed;

    /*check for the direction of left wheel to calculate the speed*/
    if(leftWheelDirection == MOTOR_DIR_FORWARD)
    {
        LSpeed = 255 - LSpeed;
    }   
    
    /*check for the direction of right wheel to calculate the speed*/
    if(rightWheelDirection == MOTOR_DIR_FORWARD)
    {
        RSpeed = 255 - RSpeed;
    }

    /*stop the left wheel*/
    if(LSpeed == MOTOR_SPEED_BREAK)
    {
      /*stop the wheel*/
      stopLeftWheel();
    }
    else
    {
      /*assign the speed to the left wheel*/
      ledcWrite(MOTOR_L_SPEED_CHANNEL, LSpeed);
    }

    /*stop the right wheel*/
    if(RSpeed == MOTOR_SPEED_BREAK)
    {
      /*stop the wheel*/
      stopRightWheel();
    }
    else
    {
      /*assign the speed to the right wheel*/
      ledcWrite(MOTOR_R_SPEED_CHANNEL, RSpeed);
    }


}

void Motor::setDirection(uint8_t LDirection, uint8_t RDirection)
{

    /*assign the direction values to variables*/
    leftWheelDirection = LDirection;
    rightWheelDirection = RDirection;

    /*check for direction of left wheel*/
    if(LDirection == MOTOR_DIR_FORWARD)
    {
        /*make left wheel move forward*/
        digitalWrite(MOTOR_L_IN1_PIN, HIGH);
    }
    else if(LDirection == MOTOR_DIR_BACKWARD)
    {
        /*make left wheel move backward*/
        digitalWrite(MOTOR_L_IN1_PIN, LOW);
    }
    else
    {
        // do nothin
    }


    /*check for the direction of the right wheel*/
    if(RDirection == MOTOR_DIR_FORWARD)
    {
        /*make right wheel move forward*/
        digitalWrite(MOTOR_R_IN1_PIN, HIGH);
    }
    else if(RDirection == MOTOR_DIR_BACKWARD)
    {
        /*make right wheel move backward*/
        digitalWrite(MOTOR_R_IN1_PIN, LOW);
    }
    else
    {
        // do nothing
    }

}   


void Motor::stop()
{
    /*the motor is stopped by writing 1 to all pins accordding to the datasheet*/
    digitalWrite(MOTOR_L_IN1_PIN, HIGH);
    ledcWrite(MOTOR_L_SPEED_CHANNEL, 256);
    digitalWrite(MOTOR_R_IN1_PIN, HIGH);
    ledcWrite(MOTOR_R_SPEED_CHANNEL, 256);
}

void Motor::increaseMotorSpeed(uint8_t leftWheelIncreaseAmount, uint8_t rightWheelIncreaseAmount)
{
    setSpeed(leftWheelSpeed + leftWheelIncreaseAmount, rightWheelSpeed + rightWheelIncreaseAmount);
}

void Motor::decreaseMotorSpeed(uint8_t leftWheelDecreaseAmount, uint8_t rightWheelDecreaseAmount)
{
    setSpeed(leftWheelSpeed - leftWheelDecreaseAmount, rightWheelSpeed - rightWheelDecreaseAmount);
}

void Motor::stopLeftWheel()
{
    /*the motor is stopped by writing 1 to all pins accordding to the datasheet*/
    digitalWrite(MOTOR_L_IN1_PIN, HIGH);
    ledcWrite(MOTOR_L_SPEED_CHANNEL, 256);
}

void Motor::stopRightWheel()
{
    /*the motor is stopped by writing 1 to all pins accordding to the datasheet*/
    digitalWrite(MOTOR_R_IN1_PIN, HIGH);
    ledcWrite(MOTOR_R_SPEED_CHANNEL, 256);
}