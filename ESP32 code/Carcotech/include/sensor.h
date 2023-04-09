#ifndef SENSOR_H_
#define SENSOR_H_

#include <Wire.h>           

/*library link: https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library*/
#include "SparkFunSX1509.h"


class Sensor
{
private:
    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
    SX1509 io;                        // Create an SX1509 object to be used throughout

public:

    /**
     * @brief: this is the initialization function, has to be called before anything
     */
    void init();
    
    /**
     * @brief used to read a sensor row given the sensor row number 
     * @param rowNum: it's the row of sensors you want to read where :
     *                  -> 0 means the back row of sensors  
     *                  -> 1 means the middle row of sensors
     *                  -> 2 means the front row of sensors
     * @return uint8_t* which is an array of length 5 and each cell in the array holds either the value :
     *                  -> WHITE
     *                  -> BLACk
    */
    uint8_t* readSensor(int rowNum); 

    
};

#endif