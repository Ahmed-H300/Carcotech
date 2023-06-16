#include "utils.h"
#include "Wire.h"

/**
 * to check the function called 'convertToCarSpace' , here are all the possible converstions
 * --------
 * LL -> LF
 * LF -> LR
 * LB -> LL
 * --------
 * RR -> Rf
 * RF -> RL
 * RB -> RR
 * --------
 * FL -> FL
 * FR -> FR
 * FF -> FF
 * --------
 * BL -> BR
 * BR -> BL
 * BB -> BF
 * --------
 * LR, RL, FB, BF (not possible as they are removed by 'optimizePath')
*/



// these are some private defines for the code
#define FORWARD_ANGLE   0
#define LEFT_ANGLE      270
#define RIGHT_ANGLE     90
#define BACKWARD_ANGLE  180  

uint32_t Util::getArrSize(uint8_t* arr)
{
    uint8_t* tempPtr = arr;
    uint32_t size = 0;
    while (*tempPtr != DIR_END)
    {
        size++;
        tempPtr++;
    }
    return size;
}


uint8_t Util::getActualDirection(uint8_t carSeenDirection)
{
    /*map the car seen direction to an angle*/
    uint16_t currentAngle =  (carSeenDirection == DIR_FORWARD)   ? FORWARD_ANGLE    :
                             (carSeenDirection == DIR_BACKWARD)  ? BACKWARD_ANGLE   :
                             (carSeenDirection == DIR_LEFT)      ? LEFT_ANGLE       :
                             (carSeenDirection == DIR_RIGHT)     ? RIGHT_ANGLE      :
                                                                   0                ;                        

    /*add to angle*/
    angle = (angle + currentAngle) % 360;

    /*map the angle to user space direction*/
    uint8_t userSeenDirection = (angle == FORWARD_ANGLE)    ?   DIR_FORWARD     :
                                (angle == LEFT_ANGLE)       ?   DIR_LEFT        :
                                (angle == RIGHT_ANGLE)      ?   DIR_RIGHT       :
                                (angle == BACKWARD_ANGLE)   ?   DIR_BACKWARD    :
                                                                '0'             ;
                                

    return userSeenDirection;
}

void Util::optimizePath(uint8_t* traverseArray)
{
    /*get the size of the input array*/
    uint32_t size = getArrSize(traverseArray);

    /*a variable to indicate whether to traverse the array again or not*/
    bool thereismove = true;
    
    // while there is change
    while(thereismove){

        thereismove = false;
        
        // Loop through the input moves
        for (int32_t i = 0; i < size; i++) {
            // If the next move cancels the current move, skip both moves
            if ((traverseArray[i] == 'L' && traverseArray[i+1] == 'R')
                || (traverseArray[i] == 'R' && traverseArray[i+1] == 'L')
                || (traverseArray[i] == 'F' && traverseArray[i+1] == 'B')
                || (traverseArray[i] == 'B' && traverseArray[i+1] == 'F')) {
                thereismove = true;
                uint32_t j = i;
                for (; j < size-2; j++)
                {
                    traverseArray[j] = traverseArray[j+2];
                }
                traverseArray[j] = DIR_END;
                traverseArray[j+1] = DIR_END;
                i--;
                size -= 2;
            }
        }
    }
}



void Util::convertToCarSpace(uint8_t* optimizedArr)
{
    /*get the size of the array*/
    uint32_t size = getArrSize(optimizedArr);

    /*convert to car space*/
    for (int32_t i = size-1; i > 0; i--)
    {
        /*map each number to an angle*/
        uint16_t angle1 =       (optimizedArr[i] == DIR_FORWARD)   ? FORWARD_ANGLE    :
                                (optimizedArr[i] == DIR_BACKWARD)  ? BACKWARD_ANGLE   :
                                (optimizedArr[i] == DIR_LEFT)      ? LEFT_ANGLE       :
                                (optimizedArr[i] == DIR_RIGHT)     ? RIGHT_ANGLE      :
                                                                    0                 ; 

        uint16_t angle2 =       (optimizedArr[i-1] == DIR_FORWARD)   ? FORWARD_ANGLE    :
                                (optimizedArr[i-1] == DIR_BACKWARD)  ? BACKWARD_ANGLE   :
                                (optimizedArr[i-1] == DIR_LEFT)      ? LEFT_ANGLE       :
                                (optimizedArr[i-1] == DIR_RIGHT)     ? RIGHT_ANGLE      :
                                                                       0                 ; 
                             

        /*get the relative angle of direction given the previous direction*/
        int16_t differenceAngle = angle1 - angle2;
        if(differenceAngle < 0)
            differenceAngle += 360;

        /*map the difference angle to a character*/
        uint8_t carSeenDirection =  (differenceAngle == FORWARD_ANGLE)    ?   DIR_FORWARD     :
                                    (differenceAngle == LEFT_ANGLE)       ?   DIR_LEFT        :
                                    (differenceAngle == RIGHT_ANGLE)      ?   DIR_RIGHT       :
                                    (differenceAngle == BACKWARD_ANGLE)   ?   DIR_BACKWARD    :
                                                                              '0'             ; 
        /*store the result in the array*/
        optimizedArr[i] = carSeenDirection;       
    }
    
}


String Util::convertToString(uint8_t* arr)
{
    int i;
    String s = "";
    for (i = 0; arr[i] != '\0'; i++) {
        s = s + String((char)arr[i]);
    }
    return s;
}


void Util::I2CScanner()
{
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);     // PCF8574 7 bit address
      Serial.println (")");
      count++;
    }
  }
  Serial.print ("Found ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
}


void Util::setAngle(uint16_t val)
{
    angle = val;
}

void Util::reversePath(uint8_t* path)
{
    /*get size of the array*/
    uint32_t size = getArrSize(path);

    /*copy data from path to tempArr*/
    for(uint32_t i = 0; i < size/2; i++)
    {
        uint8_t temp = path[i];
        path[i] = path[size-1-i];
        path[size-1-i] = temp;
    }

    /*replace every L with R and every R with L*/
    for(uint32_t i = 0; i < size; i++)
    {
        if(path[i] == 'L')
        {
            path[i] = 'R';
        }
        else if(path[i] == 'R')
        {
            path[i] = 'L';
        }
        else
        {
            // do nothing
        }
        
    }
}

void Util::EEPROM_printPath()
{
    uint32_t pathSize = 0;

    I2C_eeprom eeprom(0x50, I2C_DEVICESIZE_24LC256);
    eeprom.readBlock(0, (uint8_t*)&pathSize, sizeof(pathSize));
    uint8_t path[pathSize+1];
    eeprom.readBlock(sizeof(pathSize), path, pathSize);
    path[pathSize] = '\0';

    Serial.println(pathSize);
    Serial.println(convertToString(path));   
}