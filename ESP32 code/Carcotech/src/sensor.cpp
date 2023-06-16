#include "sensor.h"

#include <Wire.h>


void Sensor::init()
{
    
    // Wire.begin(21,22, 400000);
    bool isWireInitializedSuccessfully = false;
    uint8_t isSX1509InitializedSuccessfully = 0;
    do
    {
        /*initialize I2C communication where SDA is connected to GPIO21, SCL is connected to GPIO22*/
        isWireInitializedSuccessfully = Wire.begin();

        /*begin the port expander*/
        isSX1509InitializedSuccessfully = io.begin(SX1509_ADDRESS);

    } while (!isWireInitializedSuccessfully && isSX1509InitializedSuccessfully == 0);
    


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
    data[2][0] = (rawData >> 14) & 1;
    data[2][1] = (rawData >> 13) & 1;
    data[2][2] = (rawData >> 12) & 1;
    data[2][3] = (rawData >> 11) & 1;
    data[2][4] = (rawData >> 10) & 1;
    
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


void Sensor::copySensorValToAnotherVal(uint8_t (*src)[5],  uint8_t (*dst)[5])
{
    for(uint8_t i = 0; i < 3; i++)
    {
        for(uint8_t j = 0; j < 5; j++)
        {
            dst[i][j] = src[i][j];
        }
    }
}

bool Sensor::isDataEqual(uint8_t (*src)[5],  uint8_t (*dst)[5])
{
    bool isEqual = true;
    for(uint8_t i = 0; i < 3; i++)
    {
        for(uint8_t j = 0; j < 5; j++)
        {
            if(dst[i][j] != src[i][j])
                    isEqual = false;
        }
    }   
    return isEqual;
}

uint8_t (*Sensor::readStableSensor(uint8_t minNumOfReadings))[5]
{
    /*local variable to hold the old reading*/
    uint8_t oldVal[3][5];

    /*local variable to indicate whether the reading is stable or not*/
    bool isStable = true;

    /*get initial reading*/
    readAndProcessReadings();

    /*read till getting a stable value*/
    do{

        for(uint8_t i = 0; i < minNumOfReadings; i++)
        {
            /*copy the value into local variable*/
            copySensorValToAnotherVal(data, oldVal);

            /*get a reading*/
            readAndProcessReadings();

            /*determine whether old val equal to new value*/
            if(isDataEqual(data, oldVal) == false){
                isStable = false;
                break;
            }
        }

    }while(!isStable);

    /*return the whole matrix*/
    return data;
}

void Sensor::addToMatrix(uint8_t (*src)[5],  uint8_t (*dst)[5])
{
    for(uint8_t i = 0; i < 3; i++)
    {
        for(uint8_t j = 0; j < 5; j++)
        {
            dst[i][j] += src[i][j];
        }
    }
}

void Sensor::noramlizeMatrixAndAverage(uint8_t (*data)[5], float_t threshold)
{
    for(uint8_t i = 0; i < 3; i++)
    {
        for(uint8_t j = 0; j < 5; j++)
        {
            if(data[i][j] >= threshold)
                data[i][j] = 1;
            else if(data[i][j] < threshold)
                data[i][j] = 0;
            else
                data[i][j] = 0;
        }
    }   
}

uint8_t (*Sensor::readAverageSensor(uint8_t minNumOfReadings))[5]
{
    /*local variable to hold the values*/
    uint8_t oldVal[3][5];

    /*read values from sensor*/
    readAndProcessReadings();
    
    /*get initial value for oldVal*/
    copySensorValToAnotherVal(data, oldVal);

    /*loop for minNumOfReadings and get the readings*/
    for(uint8_t i = 0; i < minNumOfReadings; i++)
    {   
        /*get some readings*/
        readAndProcessReadings();

        /*add the readings to the matrix*/
        addToMatrix(data, oldVal);
    }


    /*normalize the matrix*/
    noramlizeMatrixAndAverage(oldVal, (minNumOfReadings+1)/2.0);


    /*copy the matrix*/
    copySensorValToAnotherVal(oldVal, data);

    /*return the matrix*/
    return data;
}