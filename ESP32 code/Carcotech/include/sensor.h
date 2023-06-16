#ifndef SENSOR_H_
#define SENSOR_H_

          

/*library link: https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library */
#include "SparkFunSX1509.h"


// related to the sensor
#define BLACK  0
#define WHITE  1

class Sensor
{
private:
    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
    SX1509 io;                        // Create an SX1509 object to be used throughout

    /*this the matrix of sensorss read from the array*/
    uint8_t data[3][5];

    /*private function used to map readed data from the sensor*/
    void readAndProcessReadings();

    /*used to copy a variable to another variable*/
    void copySensorValToAnotherVal(uint8_t (*src)[5],  uint8_t (*dst)[5]);

    /*used to compare between the values of 2 sensor and return whether they are equal or not*/
    bool isDataEqual(uint8_t (*src)[5],  uint8_t (*dst)[5]);

    /*used to add matrix to another matrix*/
    void addToMatrix(uint8_t (*src)[5],  uint8_t (*dst)[5]);

    /*used to convert the matrix to either 0 or 1 based on the threshold*/
    void noramlizeMatrixAndAverage(uint8_t (*data)[5], float_t threshold);

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
     *                  -> BLACK
    */
    uint8_t* readSensor(int rowNum); 

    /**
     * @brief used to read the 3 row sensors all at the same time
     * @return  uint8_t** which is 2D array where number of rows is 3 and number of columns in 5 where each cell holds either the value:
     *                  -> BLACK
     *                  -> WHITE
    */
    uint8_t (*readSensor())[5];

    
    /**
     * @brief used to read a stable readings from the sensor given how many times to check if the reading is correct
     * @param minNumOfReadings: this is the minimum number of comparisons to decide that the readings is correct
     * @return  uint8_t** which is 2D array where number of rows is 3 and number of columns in 5 where each cell holds either the value:
     *                  -> BLACK
     *                  -> WHITE
    */
    uint8_t (*readStableSensor(uint8_t minNumOfReadings))[5];

    
    /**
     * @brief used to read a average readings from the sensor given how many times to read
     * @param minNumOfReadings: this is the minimum number of readings to get the average
     * @return  uint8_t** which is 2D array where number of rows is 3 and number of columns in 5 where each cell holds either the value:
     *                  -> BLACK
     *                  -> WHITE
    */
    uint8_t (*readAverageSensor(uint8_t minNumOfReadings))[5];

};

#endif