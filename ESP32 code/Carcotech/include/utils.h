#ifndef UTILS_H_
#define UTILS_H_

#include <Arduino.h>
#include "I2C_eeprom.h"

// this is the possible direction, @MAZE_TRAVERSE_POSSIBLE_DIR
#define DIR_FORWARD     'F'
#define DIR_BACKWARD    'B'
#define DIR_RIGHT       'R'
#define DIR_LEFT        'L'
#define DIR_END         '\0'

class Util
{
private:
    
    /*this is the current angle (used by wall follower algorithm)*/
    uint16_t angle = 0;
    
public:
   
    /**
     * @brief this function is used to get the size of array given only a pointer and the array is terminated by '\0'
     * @param arr: pointer to array that we want to get its size
     * @note the array must end with '\0'
     * @return uint32_t: the size of the array execluding the '\0'
    */
    uint32_t getArrSize(uint8_t* arr);


    /**
     * @brief this function converts the direction from car space to actual user space as if car took a left direction (car space) this may mean 
     * that the car has taken north direction (user space), it's what car sees versus what the user sees (so it converts from just simple car Directions to meaningful users direction)
     * @param carSeenDirection: this is the direction what the car has taken, refer to @MAZE_TRAVERSE_POSSIBLE_DIR
     * @return uint8_t : this is the direction seen by the user
    */
    uint8_t getActualDirection(uint8_t carSeenDirection);


    /**
     * @brief this function is used to optimize and find the optimal path to solve the maze
     * @param traverseArray: this is the original traversal array
    */
    void optimizePath(uint8_t* traverseArray);

    /**
     * @brief this function is used to convert from the user space to the car space in order to traverse the maze correctly (it do the inverse of function called 'getActualDirection')
     * @param this is the array that contains our optimized path and it must end with '\0' and this function will modify this array
    */
    void convertToCarSpace(uint8_t* optimizedArr);

    /**
     * @brief this function is just used in testing to convert a character array to string to print it on the serial monitor
     * @param arr: this is the array to be converted to string (it must end with '\0')
     * @note the array to be converted to string must end with '\0'
     * @return String: this is the string representing the original character array
    */
    String convertToString(uint8_t* arr);


    /**
     * @brief this function is used to detect the addresses of all connected devices on the I2C bus and display it
     * @note the Serial must be intialized before calling this function
    */
    void I2CScanner();

    /**
     * @brief this function is used to print what path is store in the eeprom
    */
    void EEPROM_printPath();

    /**
     * @brief this function is used to set the value of angle
     * @param val: this is the value assigned to angle
    */
    void setAngle(uint16_t val);


    /**
     * @brief this function is used to reverse a path, to convert:
     *                  end point -> start point
     *                  start point -> end point
     *         this is quite useful in case of backward traversal of maze
     * @param path: array ending with '\0' to be reversed
     * @note the array passed as argument must end with '\0'
    */
   void reversePath(uint8_t* path);
    
};

#endif