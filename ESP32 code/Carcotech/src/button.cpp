#include <Arduino.h>

#include "button.h"

void Button::init()
{
    /*configure all the pins input pull ups*/
    pinMode(LINE_TRACKER_PIN, INPUT_PULLUP);
    pinMode(MAZE_TRAIN_PIN, INPUT_PULLUP);
    pinMode(MAZE_SOLVER_PIN, INPUT_PULLUP);
}

bool Button::isPressed(int pinNumber)
{
    /*define 2 variables that will halp with the problem of button debouncing*/
    int currentReading;
    int previuosReading;

    /*loop untill the previus reading is same as current reading*/
    do
    {
        /*get the previous reading*/
        previuosReading = digitalRead(pinNumber);
        
        /*wait for 1 ms*/
        delay(1);

        /*get the current reading*/
        currentReading = digitalRead(pinNumber);

    } while (previuosReading != currentReading);

    /*return the value*/
    if(currentReading == HIGH)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Button::isBeginLineTrackingPressed()
{
    return isPressed(LINE_TRACKER_PIN);    
}

bool Button::isBeginMazeSolvingPressed()
{
    return isPressed(MAZE_SOLVER_PIN);
}

bool Button::isBeginMazeTrainingPressed()
{
    return isPressed(MAZE_TRAIN_PIN);
}