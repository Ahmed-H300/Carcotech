#include "motor.h"

/*this is the ISR for interrupt signal received by the left motor feedback*/
void IRAM_ATTR isrL()
{
    /*for every 50 pulses, there is 1 rotation*/
    counterLeft++;
    if(counterLeft == 50)
    {
        counterLeft = 0;
        numOfRotationsLeft++;
    }
}

/*this is the ISR for interrupt signal received by the right motor feedback*/
void IRAM_ATTR isrR()
{
    /*for every 50 pulses, there is 1 rotation*/
    counterRight++;
    if(counterRight == 50)
    {
        counterRight = 0;
        numOfRotationsRight++;
    }
}

void Motor::init()
{

    /*make motor direction pins as output*/
    pinMode(MOTOR_R_IN1_PIN, OUTPUT);
    pinMode(MOTOR_L_IN1_PIN, OUTPUT);

    /*put initial values on the pins*/
    digitalWrite(MOTOR_R_IN1_PIN, LOW);
    digitalWrite(MOTOR_L_IN1_PIN, LOW);
    
    
    /*assign speed control PWM channels to speed control pins*/
    ledcAttachPin(MOTOR_L_IN2_PIN, MOTOR_L_SPEED_CHANNEL);
    ledcAttachPin(MOTOR_R_IN2_PIN, MOTOR_R_SPEED_CHANNEL);

    /*initalize speed channels*/
    ledcSetup(MOTOR_L_SPEED_CHANNEL, 4000, 8);
    ledcSetup(MOTOR_R_SPEED_CHANNEL, 4000, 8);
    
    /*configure interrupt pins to be input*/
    pinMode(MOTOR_L_SENS1_PIN, INPUT_PULLUP);
    pinMode(MOTOR_R_SENS1_PIN, INPUT_PULLUP);

    /*intialize the pins*/
    ledcWrite(MOTOR_L_IN2_PIN, 0);
    ledcWrite(MOTOR_R_IN2_PIN, 0);

}

void Motor::setSpeed(uint8_t LSpeed, uint8_t RSpeed)
{   

    /*check for the direction of left wheel to calculate the speed*/
    /*if(leftWheelDirection == MOTOR_DIR_FORWARD)
    {
        LSpeed = 255 - LSpeed;
    }*/   
    
    /*check for the direction of right wheel to calculate the speed*/
    /*if(rightWheelDirection == MOTOR_DIR_FORWARD)
    {
        RSpeed = 255 - RSpeed;
    }*/

    /*assign the speed to the left wheel*/
    ledcWrite(MOTOR_L_SPEED_CHANNEL, LSpeed);
    leftWheelSpeed = LSpeed;

    /*assign the speed to the right wheel*/
    ledcWrite(MOTOR_R_SPEED_CHANNEL, RSpeed);
    rightWheelSpeed = RSpeed;
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
    else if(LDirection == MOTOR_DIR_BACKWARD)
    {
        /*make right wheel move backward*/
        digitalWrite(MOTOR_R_IN1_PIN, LOW);
    }
    else
    {
        // do nothing
    }

}   


void Motor::rotate(uint8_t angleValue)
{

    /*stop the car*/
    stop();

    /*wait for 1 second*/
    delay(1000);        // TODO: edit it
    
    /*a variable the holds number of average rotations*/
    float numOfNeededRotations = 0;

    if(angleValue == MOTOR_ANGLE_180)
    {
        /*assign a value to the vriable*/
        numOfNeededRotations = MOTOR_NUM_OF_ROT_180;

        /*change the direction of the motors*/
        setDirection(MOTOR_DIR_BACKWARD, MOTOR_DIR_FORWARD);
    }
    else if(angleValue == MOTOR_ANGLE_90)
    {
        /*assign a value to the vriable*/
        numOfNeededRotations = MOTOR_NUM_OF_ROT_90;
        
        /*change the direction of the motors*/
        setDirection(MOTOR_DIR_BACKWARD, MOTOR_DIR_FORWARD);
    }
    else if(angleValue == MOTOR_ANGLE_NEG_90)
    {
        /*assign a value to the vriable*/
        numOfNeededRotations = MOTOR_NUM_OF_ROT_90;
        
        /*change the direction of the motors*/
        setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_BACKWARD);
    }
    else
    {
        // do nothing
    }

    /*set old speeds again*/
    setSpeed(leftWheelSpeed, rightWheelSpeed);
    
    /*start counting the number of rotations*/
    beginCounting();

    /*wait till the amount of rotation is required*/
    while(numOfNeededRotations > getAvgNumOfRotations());

    /*disable the external interrupts*/
    turnOffInterrups();

    /*stop the motor*/
    stop();

    /*change the directions to the default (forward)*/
    //setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);

    /*go back to original speed*/
    //setSpeed(leftWheelSpeed, rightWheelSpeed);
}

void Motor::beginCounting()
{
    /*zero out all the counters*/
    numOfRotationsLeft = 0;
    numOfRotationsRight = 0;
    counterLeft = 0;
    counterRight = 0;

    /*attach the interrup code*/
    attachInterrupt(MOTOR_L_SENS1_PIN, isrL, RISING);
    attachInterrupt(MOTOR_R_SENS1_PIN, isrR, RISING);
}

float_t Motor::getAvgNumOfRotations()
{   
    /*return the average of numOfRotationsLeft and numOfRotationsRight*/
    return (numOfRotationsLeft+numOfRotationsRight)/2.0 + ((counterLeft/50) + (counterRight/50))/2.0;
}

void Motor::turnOffInterrups()
{
    /*remove interrupts*/   
    detachInterrupt(MOTOR_L_SENS1_PIN);
    detachInterrupt(MOTOR_R_SENS1_PIN);
}


void Motor::stop()
{
    /*the motor is stopped by writing 1 to all pins accordding to the datasheet*/
    digitalWrite(MOTOR_L_IN1_PIN, HIGH);
    ledcWrite(MOTOR_L_SPEED_CHANNEL, 255);
    digitalWrite(MOTOR_R_IN1_PIN, HIGH);
    ledcWrite(MOTOR_R_SPEED_CHANNEL, 255);
}