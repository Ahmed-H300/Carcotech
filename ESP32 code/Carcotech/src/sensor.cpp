#include "sensor.h"
#include "constants.h"

#include <Wire.h>


void Sensor::init()
{
    /*initialize I2C communication where SDA is connected to GPIO21, SCL is connected to GPIO22*/
    Wire.begin();

    /*begin the port expander*/
    io.begin(SX1509_ADDRESS);

    /*make all the pins input*/
    io.writeWord(REG_DIR_B, ~0);
}

void Sensor::readAndProcessReadings()
{
    /*read the raw data (all sensors)*/
    uint16_t rawData = io.readWord(REG_DATA_B);

    /*map the values of back row to the matrix*/
    data[0][0] = (rawData >> 4) & 1;
    data[0][1] = (rawData >> 3) & 1;
    data[0][2] = (rawData >> 2) & 1;
    data[0][3] = (rawData >> 1) & 1;
    data[0][4] = (rawData >> 0) & 1;
    
    /*map the values of middle row to the matrix*/
    data[1][0] = (rawData >> 9) & 1;
    data[1][1] = (rawData >> 8) & 1;
    data[1][2] = (rawData >> 7) & 1;
    data[1][3] = (rawData >> 6) & 1;
    data[1][4] = (rawData >> 5) & 1;
    
    /*map the values of front row to the matrix*/
    data[2][0] = (rawData >> 10) & 1;
    data[2][1] = (rawData >> 11) & 1;
    data[2][2] = (rawData >> 12) & 1;
    data[2][3] = (rawData >> 13) & 1;
    data[2][4] = (rawData >> 14) & 1;
    
}

uint8_t* Sensor::readSensor(int rowNum)
{
    /*process and read raw data from the sensor*/
    readAndProcessReadings();

    /*return the corresponding needed row from the matrix*/
    return data[rowNum];
}


uint8_t (*Sensor::readSensor())[5]
{
    /*process and read raw data from the sensor*/
    readAndProcessReadings();

    /*return the whole matrix*/
    return data;
}