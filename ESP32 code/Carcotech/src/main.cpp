#include <Arduino.h>

#include "main.h"
#include "sensor.h"
#include "bluetooth.h"
#include "button.h"
#include "mp3.h"
#include "motor.h"
#include "Wire.h"
#include "I2C_eeprom.h"                             /*https://github.com/RobTillaart/I2C_EEPROM*/
#include "neopixel.h"

#include "utils.h"
#include "linkedList.h"

// utility objects
Bluetooth bluetooth;                                /*this is the object throught which we can control the bluetooth*/
Sensor sensor;                                      /*this is the sensor object which is used to read the sensors values*/    
Button buttons;                                     /*this is the object that will represent our interface buttons*/
MP3 mp3;                                            /*this is the player through which we can play songs*/  
Motor motor;                                        /*this is the motor driver through which we can control the motor*/
Util  util;                                         /*this is a utility object that holds some useful functions*/
LinkedList linkedList;                              /*this is a utility object that will deal with our traverse array*/
I2C_eeprom eeprom(0x50, I2C_DEVICESIZE_24LC256);    /*this is the eeprom that will store the optimized path in eeprom in order not to be lost*/
NeoPixel neopixel;                                  /*this is the neopixel led strip connected to esp32*/

// global useful variables
uint32_t pathSize = 0;                            /*this is the size of the extracted path from the eeprom*/
uint32_t pathCounter = 0;                         /*this is a counter that counts how many intersections exexuted from the path*/
uint8_t ledState = LED_STATE_RADNOM_EFFECT;       /*this is the status that represents which state of leds to use*/
uint8_t scanArr[3] = {WHITE, WHITE, WHITE};       /*this is the scan matrix used to indicate which case we are in when we stop*/   
uint8_t maxLineSpeed = LOWER_LINE_SPEED;          /*this is the speed which is set by the mobile that will tell us wether to lower to higher this value*/
uint8_t* path1 = nullptr;                         /*this is the extracted path while the car is going on its way from start to end*/
uint8_t* path2 = nullptr;                         /*this is the extracted path while the car is going on its way from end to start*/
uint32_t pathSize1 = 0;                           /*this is the size of path while the car is going on its way from start to end*/
uint32_t pathSize2 = 0;                           /*this is the size of path while the car is going on its way from end to start*/


// task handlers
TaskHandle_t xHandle_bluetoothSendData = NULL;    /*this is the task handler for the bleutooth send data task*/
TaskHandle_t xHandle_bluetoothReceiveData = NULL; /*this is the task handler for the bleutooth receive data task*/
TaskHandle_t xHandle_lineTracker = NULL;          /*this is the task handler for the line tracker task*/
TaskHandle_t xHandle_mazeTrainer = NULL;          /*this is the task handler for the maze trainer task*/
TaskHandle_t xHandle_mazeSolver = NULL;           /*this is the task handler for the maze solver task*/
TaskHandle_t xHandle_taskManager = NULL;          /*this is the task handler for the task manager*/
TaskHandle_t xHandle_ledStrip = NULL;             /*this is the task handler for the led strip*/


/*struct used by bluetooth receive data task*/
typedef struct
{
  uint8_t data[BLUETOOTH_MSG_RCV_LEN];
}bluetoothRcvMsg_t;



/**
 * @brief this function increases the speed of one of the wheels to align to the line
 * @param sensorsReading: this is a matrix holding all the readings of the matrix of sensors
*/
void align(uint8_t (*sensorsReading)[5])
{
  /*this the index of the front row*/
  uint8_t frontRowIndex = 2;

  /*main condition whether or not to preform aliging*/
  if(sensorsReading[frontRowIndex][2] == WHITE && sensorsReading[frontRowIndex][0] == WHITE && sensorsReading[frontRowIndex][4] == WHITE)
  {

    /*align left*/
    if(sensorsReading[frontRowIndex][1] == BLACK){
      motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
      motor.setSpeed(MAZE_LINE_SPEED, MAZE_LINE_SPEED+MAZE_ALIGN_DIFF_SPEED);
    }
    
    /*align right*/
    else if(sensorsReading[frontRowIndex][3] == BLACK){
      motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
      motor.setSpeed(MAZE_LINE_SPEED+MAZE_ALIGN_DIFF_SPEED, MAZE_LINE_SPEED);
    }
    
    /*return back without alignment*/
    else
      return;
      

    /*wait till the car go back in the middle again*/
    do
    {
      sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
      if(sensorsReading[frontRowIndex][0] == BLACK || sensorsReading[frontRowIndex][4] == BLACK 
        || (sensorsReading[frontRowIndex][2] == WHITE && sensorsReading[frontRowIndex][1] == WHITE && sensorsReading[frontRowIndex][3] == WHITE))
        break;
    } while (!(sensorsReading[frontRowIndex][2] == BLACK && sensorsReading[frontRowIndex][1] == WHITE && sensorsReading[frontRowIndex][3] == WHITE));
    
    
    /*return back to the orignal speed*/
    motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
    motor.setSpeed(MAZE_LINE_SPEED, MAZE_LINE_SPEED);

  }

}


/**
 * @brief this function constructs the scan array as while the car moving
 * @param sensorsReading: this is a matrix holding all the readings of the matrix of sensors
 * @return bool which is a bool variable telling wehther or not to allign
*/
bool scan(uint8_t (*sensorsReading)[5])
{
  /*local variable to indicate whether or not to preform aligning or not in ( we don't preform aligning incase we met a fork)*/
  bool align = true;

  /*this the index of the front row*/
  uint8_t frontRowIndex = 2;

  /*left fork*/
  if(sensorsReading[frontRowIndex][0] == BLACK){
    scanArr[0] = BLACK;
    align = false;
  }

  /*right fork*/
  if(sensorsReading[frontRowIndex][4] == BLACK){
    scanArr[2] = BLACK;
    align = false;
  }

  return align;
}

/**
 * @brief this function only converts the scanArray to valuable information where:
 *                ->  scanArr = {BLACK , WHITE, WHITE}    incase that the there is left fork
 *                ->  scanArr = {WHITE , WHITE, BLACK}    incase that there is a right fork
 *                ->  scanArr = {BLACK , WHITE, BLACK}    incase that there is 'T' fork
 *                ->  scanArr = {WHITE , BLACK, BLACK}    incase that there is 'T' fork rotated by 90     
 *                ->  scanArr = {BLACK , BLACK, WHITE}    incase that there is 'T' fork rotated by -90
 *                ->  scanArr = {BLACK , BLACK, BLACK}    incase that there is a '+' fork
 *                ->  scanArr = {WHITE , WHITE, WHITE}    incase that is a deadend
 * @return unit8_t which detects the status of the fork, refer to @CAR_FORK_CASE
*/
uint8_t evaluateScanArray()
{ 
  if(scanArr[0] == BLACK && scanArr[1] == WHITE && scanArr[2] == WHITE)
    return LINE_FORK_LEFT;

  else if(scanArr[0] == WHITE && scanArr[1] == WHITE && scanArr[2] == BLACK)
    return LINE_FORK_RIGHT;

  else if(scanArr[0] == BLACK && scanArr[1] == WHITE && scanArr[2] == BLACK)
    return LINE_FORK_T;

  else if(scanArr[0] == BLACK && scanArr[1] == BLACK && scanArr[2] == BLACK)
    return LINE_FORK_CROSS;

  else if(scanArr[0] == WHITE && scanArr[1] == WHITE && scanArr[2] == WHITE)
    return LINE_FORK_DEADEND;
  
  else if(scanArr[0] == WHITE && scanArr[1] == BLACK && scanArr[2] == BLACK)
    return LINE_FORK_T_ANG_90;

  else if(scanArr[0] == BLACK && scanArr[1] == BLACK && scanArr[2] == WHITE)
    return LINE_FORK_T_ANG_NEG_90;

  else
    return UNDEFINED_BEHAVIOUR;
}



/**
 * @brief this function is used to make the car move forward, this usaually happens by making both of the motor wheels move in same direction
 *        till sensors[middleRow][0th Index || 4th Index] = BLACK  or sensors[frontRow][*] = WHITE then stop the motors as there is fork in the line and return case of the fork
 * @return uint8_t which represents the case through which current fork case, refer to @CAR_FORK_CASE
*/
uint8_t moveForwardTillBlackLine()
{

  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5];   

  /*this is the variable that indicates what is the front row*/
  uint8_t frontRowIndex;

  /*this is a variable that indicates the status of the fork*/
  uint8_t forkStatus = UNDEFINED_BEHAVIOUR;

  /*this is a variable the will hold the stop condition*/
  bool stopCondition = false;
  
  /*the front row now is the actual fron row*/
  frontRowIndex = 2;

  /*assign the direction to the motor*/
  motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);

  /*assign the initial speed to the motor*/
  motor.setSpeed(MAZE_LINE_SPEED, MAZE_LINE_SPEED);

  /*begin looping till the end condition*/
  do
  {
    /*read sonsor data*/
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
    
    /*construct scan array*/
    bool executeAlign = scan(sensorsReading);

    /*align the motor to the line*/
    if(executeAlign)
      align(sensorsReading);

    /*there is comming fork so stop when arriving (evaluate stop condition)*/
    if(!executeAlign){
      do
        {
          sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
          stopCondition = (sensorsReading[1][0] == BLACK) || (sensorsReading[1][4] == BLACK);

          /*just in case the forward sensors didn't get it right*/
          if(sensorsReading[1][0] == BLACK || sensorsReading[frontRowIndex][0] == BLACK)
            scanArr[0] =  BLACK;
          if(sensorsReading[1][4] == BLACK || sensorsReading[frontRowIndex][4] == BLACK)
            scanArr[2] =  BLACK;
          
        } while (!stopCondition);
  
      break;
    }
    
    /*stop whenever there is a dead end*/
    stopCondition = (sensorsReading[1][0] == WHITE && sensorsReading[1][1] == WHITE && sensorsReading[1][2] == WHITE && sensorsReading[1][3] == WHITE && sensorsReading[1][4] == WHITE 
                      && sensorsReading[frontRowIndex][2] == WHITE && sensorsReading[frontRowIndex][1] == WHITE && sensorsReading[frontRowIndex][3] == WHITE);

    
  } while (!(stopCondition));
  
  /*check for a forward black line*/
  if(sensorsReading[frontRowIndex][2] == BLACK || sensorsReading[frontRowIndex][1] == BLACK || sensorsReading[frontRowIndex][3] == BLACK)
    scanArr[1] = BLACK;


  /*get the status of the fork*/
  forkStatus = evaluateScanArray();

  /*check if we reached the end of the maze*/
  sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  if(sensorsReading[frontRowIndex][0] == BLACK && sensorsReading[frontRowIndex][1] == BLACK && sensorsReading[frontRowIndex][2] == BLACK && sensorsReading[frontRowIndex][3] == BLACK && sensorsReading[frontRowIndex][4] == BLACK)
    forkStatus = LINE_FORK_END;

  /*clear the scan array*/
  scanArr[0] = WHITE;
  scanArr[1] = WHITE;
  scanArr[2] = WHITE;

  /*construct the state of the fork*/
  return forkStatus;

}


/**
 * @brief this function makes the car moves from the round black circle to the straight black line, it does this simply by moving forward,
 * with aligning till the middle isn't black completly
*/
void moveFromBlackCircleToBlackLine()
{
  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5]; 

  /*set speed and direction of the car*/
  motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
  motor.setSpeed(MAZE_LINE_SPEED, MAZE_LINE_SPEED);

  /*move forward till exit the black circle*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  }while(sensorsReading[1][0] == BLACK && sensorsReading[1][1] == BLACK && sensorsReading[1][2] == BLACK && sensorsReading[1][3] == BLACK && sensorsReading[1][4] == BLACK);

}

/**
 * @brief this function is used to make the car turn left by 90 degree, this usaually happens by making one of the motor wheels move in direction
 *        while the other wheel moves in the reverse direction then rotate till read the (sensors[frontRow][0th index] = WHITE)  then 
 *        rotate again in the same direction till (sensors[frontRow][middle index] = BLACK and sensors[frontRow][middle index-1] = WHITE and sensors[frontRow][middle index+1] = BLACK) then continue moving forward
*/
void turnLeftTillBlackLine()
{
  /*this is a variable that indicates which sensor array to read from*/
  uint8_t frontRowIndex = 2;

  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5]; 

  /*set the direction of the motors*/
  motor.setDirection(MOTOR_DIR_BACKWARD, MOTOR_DIR_FORWARD);
  motor.setSpeed(MAZE_LINE_SPEED_OPPOSITE_DIR, MAZE_LINE_SPEED);
  
  /*rotate till the car front sensor become in the white space*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (sensorsReading[frontRowIndex][2] == BLACK || sensorsReading[frontRowIndex][0] == WHITE);

  /*continue rotating till the sensor return back to the black line again*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (!(sensorsReading[frontRowIndex][2] == BLACK && sensorsReading[frontRowIndex][1] == WHITE && sensorsReading[frontRowIndex][3] == WHITE));

}

/**
 * @brief this function is used to make the car turn right by 90 degree, this usaually happens by making one of the motor wheels move in direction
 *        while the other wheel moves in the reverse direction then rotate till read the (sensors[frontRow][0th index] = WHITE)  then 
 *        rotate again in the same direction till (sensors[frontRow][middle index] = BLACK and sensors[frontRow][middle index-1] = WHITE and sensors[frontRow][middle index+1] = BLACK) then continue moving forward
*/
void turnRightTillBlackLine()
{
  /*this is a variable that indicates which sensor array to read from*/
  uint8_t frontRowIndex = 2;

  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5]; 

  /*set the direction of the motors*/
  motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_BACKWARD);
  motor.setSpeed(MAZE_LINE_SPEED, MAZE_LINE_SPEED_OPPOSITE_DIR);
  
  /*rotate till the car front sensor become in the white space*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (sensorsReading[frontRowIndex][2] == BLACK || sensorsReading[frontRowIndex][4] == WHITE);

  /*continue rotating till the sensor return back to the black line again*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (!(sensorsReading[frontRowIndex][2] == BLACK && sensorsReading[frontRowIndex][1] == WHITE && sensorsReading[frontRowIndex][3] == WHITE));

}


/**
 * @brief this function is used to make the car turn right by 180 degree, this usaually happens by making one of the motor wheels move in direction
 *        while the other wheel moves in the reverse direction then rotate till read the (sensors[frontRow][2th index] = BLACK) wthen continue moving forward
*/
void UTurnTillBlackLine()
{
  /*this is a variable that indicates which sensor array to read from*/
  uint8_t frontRowIndex = 2;

  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5]; 

  /*set the direction of the motors*/
  motor.setDirection(MOTOR_DIR_BACKWARD ,MOTOR_DIR_FORWARD);
  motor.setSpeed(MAZE_LINE_SPEED_OPPOSITE_DIR, MAZE_LINE_SPEED);
  
  /*rotate till the car front sensor become on the line again*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (sensorsReading[frontRowIndex][2] == WHITE);

}

/**
 * @brief this function is used to make a u-turn when reaching the end so that it can visit an extra path, this usaually happens by making one of the motor wheels move in direction
 *        while the other wheel moves in the reverse direction then rotate till read the (sensors[frontRow][2th index] = WHITE) hen rotate till read the (sensors[frontRow][2th index] = BLACK) 
 *        then continue moving forward
*/
void UTurnAtEndCircle()
{
  /*this is a variable that indicates which sensor array to read from*/
  uint8_t frontRowIndex = 2;

  /*local variables used in reading sensors values*/
  uint8_t (*sensorsReading)[5]; 

  /*return back till the begin of the line*/
  motor.setDirection(MOTOR_DIR_BACKWARD , MOTOR_DIR_BACKWARD);
  motor.setSpeed(MAZE_LINE_SPEED_OPPOSITE_DIR, MAZE_LINE_SPEED_OPPOSITE_DIR);
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while ((sensorsReading[1][0] == BLACK || sensorsReading[1][1] == BLACK) && (sensorsReading[1][3] == BLACK || sensorsReading[1][4] == BLACK));

  /*wait for motor to stop*/
  motor.stop();
  vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));

  /*set the direction of the motors*/
  motor.setDirection(MOTOR_DIR_BACKWARD ,MOTOR_DIR_FORWARD);
  motor.setSpeed(MAZE_LINE_SPEED_OPPOSITE_DIR, MAZE_LINE_SPEED);

  /*rotate till the car front sensor become on the line again*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (sensorsReading[frontRowIndex][2] == BLACK);

  /*rotate till the car front sensor become on the line again*/
  do
  {
    sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
  } while (sensorsReading[frontRowIndex][2] == WHITE);

}


/**
 * @brief this is the task used for receiving data by esp32 from a mobile using bluetooth to change the max speed value on the line
*/
void receiveData(void* pvParameters)
{
  /*for sending and receiving structs*/
  bluetoothRcvMsg_t receivedMsg;

  /*suspend the bluetooth task*/
  /*docs: https://www.freertos.org/a00130.html */
  vTaskSuspend(NULL);

  while (true)
  {

    /*receiving message form the receiver from the receiver and put it in a queue to be send back*/
    strcpy((char*)receivedMsg.data, bluetooth.getData(BLUETOOTH_MSG_RCV_LEN, 0).c_str());

    /*for some reason we received an empty message*/
    if(receivedMsg.data[0] != '\0' && receivedMsg.data[0] != '\n') 
    {
      
      /*check for the sent character*/
      if(receivedMsg.data[0] == 'h' || receivedMsg.data[0] == 'H')
      {
        /*the mobile app let us use the maximum speed*/
        maxLineSpeed = UPPER_LINE_SPEED;
      }
      else if(receivedMsg.data[0] == 'l' || receivedMsg.data[0] == 'L')
      {
        /*the mobile app prohibited us from using the maximum speed*/
        maxLineSpeed = LOWER_LINE_SPEED;
      }
      else
      {
        // invalid char so do nothing
      }

    }

    /*to preform context switching*/
    taskYIELD();
  }

  /*to prevent a task from returning as if the task returned, it will cause an exception*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);

}

/**
 * @brief this is the task used for line tracker
*/
void lineTracker(void *pvParamters)
{ 

  while (true)
  {

    /*this is a flag to indicate whether user wanted to end the line tracker or not*/
    bool endLineTracker = false; 

    /*wait for a notification from the task manager*/
    /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /*begin the song*/
    mp3.changeSong(YA_SAYAED, false);

    /*local variables used in reading sensors values*/
    uint8_t (*sensorsReading)[5];   

    /*assign the direction to the motor*/
    motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);

    do
    {
        
      /*assign the initial speed to the motor*/
      motor.setSpeed(maxLineSpeed, maxLineSpeed);

      /*get the readings of the sensors*/
      sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);

      /*check for the case of the sensors whether to align or not*/
      if(sensorsReading[2][0] == WHITE && sensorsReading[2][1] == WHITE && (sensorsReading[2][3] == BLACK || sensorsReading[2][4] == BLACK))
      { 
        /*align right*/
        motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
        motor.setSpeed(CURVE_SPEED, MAX_SPEED);
        do
        {
          sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
        } while (sensorsReading[2][2] == WHITE);

        /*return to original forward speed*/
        motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
        motor.setSpeed(maxLineSpeed, maxLineSpeed);

      }
      else if ((sensorsReading[2][0] == BLACK || sensorsReading[2][1] == BLACK) && sensorsReading[2][3] == WHITE && sensorsReading[2][4] == WHITE)
      {
        /*align left*/
        motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
        motor.setSpeed(MAX_SPEED, CURVE_SPEED);
        do
        {
          sensorsReading = sensor.readAverageSensor(MIN_NUM_OF_READINGS);
        } while (sensorsReading[2][2] == WHITE);

        /*return to original forward speed*/
        motor.setDirection(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD);
        motor.setSpeed(maxLineSpeed, maxLineSpeed);
      }
      else
      {
        // do nothing
      }

      /*check if user wanted to end the line tracker task*/
      if(buttons.isBeginLineTrackingPressed())
      {
        /*stop the motor*/
        motor.stop();

        /*give control back to user*/
        endLineTracker = true;    

        /*stop play the music*/
        mp3.stopMusic();  

        /*delay for button debouncing*/
        vTaskDelay(pdMS_TO_TICKS(250)); 
      }

    }while(endLineTracker == false);

    /*wake up the task manager to tell it that I finished*/
    /*docs: https://www.freertos.org/xTaskNotifyGive.html */
    xTaskNotifyGive(xHandle_taskManager);   

  }
  
  /*safety: preventy the task from returning*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);

}

/**
 * @brief this is the task to get the correct flow of maze to solve it
*/
void MazeTrainingTask(void *pvParamters)
{

  while (true)
  {
 
    /*this is a local variable to indicate whether we reached first end or second end*/
    uint8_t didWeReachedEnd = 0;
    
    /*wait for a notification from the task manager*/
    /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /*zero out the angle*/
    util.setAngle(0);

    /*begin the song*/
    mp3.changeSong(EL_3GALA, false);

    /*move forward till exit the black circle*/
    moveFromBlackCircleToBlackLine();

    while (didWeReachedEnd < 2)
    {
      /*first we move forward till we intercept with intersection or any thing that will change direction of motion*/
      uint8_t forkState = moveForwardTillBlackLine();

      /*check for which case of intersection*/
      if(forkState == LINE_FORK_LEFT)
      {
        /*we turn left*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));
        turnLeftTillBlackLine();    
        linkedList.addToList(util.getActualDirection(DIR_LEFT));
        bluetooth.sendData(String(DIR_LEFT)+"\n");  // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));
      }
      else if(forkState == LINE_FORK_RIGHT)
      {
        /*we turn right*/
        motor.stop(); 
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));
        turnRightTillBlackLine();
        linkedList.addToList(util.getActualDirection(DIR_RIGHT));
        bluetooth.sendData(String(DIR_RIGHT)+"\n"); // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
      }
      else if(forkState == LINE_FORK_T)
      {
        /*we choose the left direction*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));
        turnLeftTillBlackLine();
        linkedList.addToList(util.getActualDirection(DIR_LEFT));
        bluetooth.sendData(String(DIR_LEFT)+"\n");  // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));
      }
      else if(forkState == LINE_FORK_CROSS)
      {
        /*we choose the left direction*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        turnLeftTillBlackLine();    
        linkedList.addToList(util.getActualDirection(DIR_LEFT));
        bluetooth.sendData(String(DIR_LEFT)+"\n");  // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
      }
      else if(forkState == LINE_FORK_T_ANG_90)
      {
        /*we choose the forward direction so do nothing*/
        linkedList.addToList(util.getActualDirection(DIR_FORWARD));
        bluetooth.sendData(String(DIR_FORWARD)+"\n"); // send info to the mobile
      }
      else if(forkState == LINE_FORK_T_ANG_NEG_90)
      {
        /*we choose the left direction*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        turnLeftTillBlackLine();    
        linkedList.addToList(util.getActualDirection(DIR_LEFT));  
        bluetooth.sendData(String(DIR_LEFT)+"\n");  // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
      }
      else if(forkState == LINE_FORK_END)
      {
        /*we reached the end of the maze*/
        motor.stop();

        /*getting the optimized path*/ 
        uint8_t* path = linkedList.convertToArr();
        util.optimizePath(path);
        util.convertToCarSpace(path);
        uint32_t pathSize = util.getArrSize(path);

        /*increment the variable*/
        didWeReachedEnd++;
        

        // check which path we extracted, is it on the way to end or on the way to home
        if(didWeReachedEnd == 1)
        {
            /*this is the path on the way from start to end*/
            path1 = path;
            pathSize1 = pathSize;
            path = nullptr;

            /*send the extracted path to the bluetooth*/
            bluetooth.sendData(util.convertToString(path1));  // send info to the mobile
            bluetooth.sendData("\n");

            /*make a u-turn and begin traversing again*/
            util.setAngle(0);
            UTurnAtEndCircle();   
            motor.stop();
            vTaskDelay(pdMS_TO_TICKS(STOP_DELAY));

        }
        else if(didWeReachedEnd == 2)
        {   
            /*this is the path on the way from end to start*/
            path2 = path;
            pathSize2 = pathSize; 
            path = nullptr; 
            util.reversePath(path2);

            /*send the extracted path to the bluetooth*/
            bluetooth.sendData(util.convertToString(path2));  // send info to the mobile
            bluetooth.sendData("\n");
        }
        else
        {
          // do nothing
        }

      }
      else if(forkState == LINE_FORK_DEADEND)
      {
        /*change the direction of the car*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        UTurnTillBlackLine();
        linkedList.addToList(util.getActualDirection(DIR_BACKWARD));
        bluetooth.sendData(String(DIR_BACKWARD)+"\n");    // send info to the mobile
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
      
      }
      else
      {
        // do nothing
      }

    } 


    /*a variable that will have the paramters of the shortest path*/
    uint8_t* shortestPath = nullptr;      /*will hold a pointer to array that contains the shortest path*/
    uint32_t shortestSize = 0;            /*will hold the length of the shortest path*/

    /*check which path is shorter*/
    if(pathSize2 < pathSize1)
    {
      shortestPath = path2;
      shortestSize = pathSize2;
    }
    else if(pathSize1 < pathSize2)
    {
      shortestPath = path1;
      shortestSize = pathSize1;     
    }
    else
    {
      // make path1 to be the default path
      shortestPath = path1;
      shortestSize = pathSize1;         
    }
      
    /*store results in eeprom*/
    eeprom.writeBlock(STORE_ADDR, (uint8_t*)&shortestSize, sizeof(shortestSize));
    eeprom.writeBlock(STORE_ADDR+sizeof(shortestSize), shortestPath, shortestSize);

    /*send result to bluetooth*/
    bluetooth.sendData("-------------------------------\n");
    bluetooth.sendData(util.convertToString(path1) + "\n");
    bluetooth.sendData(util.convertToString(path2) + "\n");
    bluetooth.sendData("-------------------------------\n");
    bluetooth.sendData(String(shortestSize) + "\n");
    bluetooth.sendData(util.convertToString(shortestPath));
    bluetooth.sendData("\n");

    /*free the memory*/
    delete [] path1;
    delete [] path2;
    path1 = nullptr;
    path2 = nullptr;
    shortestPath = nullptr;
    pathSize1 = 0;
    pathSize2 = 0;
    shortestSize = 0;

    /*run the end song*/
    mp3.changeSong(UWU, false);

    /*wake up the task manager to tell it that I finished*/
    /*docs: https://www.freertos.org/xTaskNotifyGive.html */
    xTaskNotifyGive(xHandle_taskManager);

  }

  /*safety: preventy the task from returning*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);
    
}



/**
 * @brief this is the task to solve maze given that the maze trainer task extruded the optimized path
*/
void MazeSolverTask(void *pvParamters)
{
  
  while (true)
  {

    /*wait for a notification from the task manager*/
    /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    /*start the start song*/
    mp3.changeSong(L3BT_YA_ZAHR, false);

    /*get the stored optimized path from EEPROM*/
    eeprom.readBlock(STORE_ADDR, (uint8_t*)&pathSize, sizeof(pathSize));
    uint8_t path[pathSize+1];
    eeprom.readBlock(STORE_ADDR+sizeof(pathSize), path, pathSize);
    path[pathSize] = '\0';

    /*move forward till exit the black circle*/
    moveFromBlackCircleToBlackLine();


    do
    {
      /*first we move forward till we intercept with intersection or any thing that will change direction of motion*/
      moveForwardTillBlackLine();

      /*check for which case of intersection*/
      if(path[pathCounter] == DIR_LEFT)
      {
        /*we turn left*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        turnLeftTillBlackLine();    
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        pathCounter++;
      }
      else if(path[pathCounter] == DIR_RIGHT)
      {
        /*we turn right*/
        motor.stop();   
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        turnRightTillBlackLine();
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        pathCounter++;
      }
      else if(path[pathCounter] == DIR_FORWARD)
      {
        /*we choose the forward direction so do nothing*/
        pathCounter++;
      }
      else if(path[pathCounter] == DIR_BACKWARD)
      {
        /*change the direction of the car*/
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        UTurnTillBlackLine();
        motor.stop();
        vTaskDelay(pdMS_TO_TICKS(STOP_DELAY)); 
        pathCounter++;
      }
      else
      {
        // do nothing
      }

      /*check if we reached the end of the path*/ 
      if(pathCounter == pathSize)
      {
        /*move till end then stop*/
        moveForwardTillBlackLine();
        motor.stop();

        /*start end song*/
        mp3.changeSong(DOCTOR_BAHGAT, false);

        /*break from the loop*/
        break;
      }

    }while (pathCounter < pathSize);  

    /*zero out global variables*/
    pathCounter = 0;
    pathSize = 0;

    /*wake up the task manager to tell it that I finished*/
    /*docs: https://www.freertos.org/xTaskNotifyGive.html */
    xTaskNotifyGive(xHandle_taskManager);

  }
  
  /*safety: preventy the task from returning*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);

}



/**
 * @brief this task is used to control different patterns for led strip
*/
void ledStripTask(void *pvParamters)
{ 

  /*suspend the led task*/
  /*docs: https://www.freertos.org/a00130.html */
  vTaskSuspend(NULL);

  while (true)
  {
      
    /*check for the state of led*/
    if(ledState == LED_STATE_RADNOM_EFFECT)
    {
      // Fill along the length of the strip in various colors...
      neopixel.colorWipe(neopixel.Color(255,   0,   0), 50);    // RED
      taskYIELD();
      neopixel.colorWipe(neopixel.Color(  0, 255,   0), 50);    // Green
      taskYIELD();
      neopixel.colorWipe(neopixel.Color(  0,   0, 255), 50);    // Blue
      taskYIELD();

      // Do a theater marquee effect in various colors...
      neopixel.theaterChase(neopixel.Color(127, 127, 127), 50); // White, half brightness
      taskYIELD();
      neopixel.theaterChase(neopixel.Color(127,   0,   0), 50); // Red, half brightness
      taskYIELD();
      neopixel.theaterChase(neopixel.Color(  0,   0, 127), 50); // Blue, half brightness
      taskYIELD();

      // Flowing rainbow cycle along the whole strip
      neopixel.rainbow(10);   
      taskYIELD();          

      // Rainbow-enhanced theaterChase variant
      neopixel.theaterChaseRainbow(50); 
      taskYIELD();
    }
    else if(ledState == LED_STATE_BATTERY_EFFECT  && pathSize != 0)
    {
      // show how much of maze we completed
      neopixel.batteryChargeEffect((float_t)pathCounter/pathSize);

      /*make the task sleep for 1 second*/
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if(ledState == LED_STATE_BLUETOOTH)
    {
      // turn on all leds in case esp32 received HIGH otherwise, turn it off
      if(maxLineSpeed == UPPER_LINE_SPEED)
      {
        /*turn on all leds in case the speed became the higher speed due to bluetooth sending high*/
        neopixel.turnOnAllLedsBatteryEffect();
      }
      else if(maxLineSpeed == LOWER_LINE_SPEED)
      {
        /*turn off all leds as that means the bluetooth is sending low*/
        neopixel.clearStrip();
      }
      else
      {
        // do nothing
      }
      
      /*make the task sleep for 250 milli second*/
      vTaskDelay(pdMS_TO_TICKS(250));
    }
    else
    {
      // do nothing
    }

  }

  /*safety: preventy the task from returning*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);

  
}

/**
 * @brief this task is used to manage all other tasks (stop and resume), to choose between different modes of the car
*/
void tasksManager(void *pvParamters)
{

  while (true)
  {
    /*check for whcih button was pressed*/
    if(buttons.isBeginLineTrackingPressed())
    {
      /*wait for some time so that user can lift his hand from button*/
      vTaskDelay(pdMS_TO_TICKS(250));

      /*begin leds task*/
      /*docs: https://www.freertos.org/a00131.html */
      ledState = LED_STATE_BLUETOOTH;
      vTaskResume(xHandle_ledStrip);

      /*begin the line tracker*/
      /*docs: https://www.freertos.org/xTaskNotifyGive.html */
      xTaskNotifyGive(xHandle_lineTracker);

      /*begin the bluetooth receiving task*/
      /*docs: https://www.freertos.org/a00131.html */
      vTaskResume(xHandle_bluetoothReceiveData);

      /*suspend till the maze solver task finishes*/
      /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

      /*suspend the bluetooth task*/
      /*docs: https://www.freertos.org/a00130.html */
      vTaskSuspend(xHandle_bluetoothReceiveData);

      /*suspend the led task*/
      /*docs: https://www.freertos.org/a00130.html */
      vTaskSuspend(xHandle_ledStrip);
      neopixel.clearStrip();

    }
    else if(buttons.isBeginMazeTrainingPressed())
    {
      /*begin leds task*/
      /*docs: https://www.freertos.org/a00131.html */
      ledState = LED_STATE_RADNOM_EFFECT;
      vTaskResume(xHandle_ledStrip);

      /*begin maze trainer*/
      /*docs: https://www.freertos.org/xTaskNotifyGive.html */
      xTaskNotifyGive(xHandle_mazeTrainer);

      /*suspend till the maze solver task finishes*/
      /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

      /*suspend the led task*/
      /*docs: https://www.freertos.org/a00130.html */
      vTaskSuspend(xHandle_ledStrip);
      neopixel.clearStrip();
    }
    else if(buttons.isBeginMazeSolvingPressed())
    {     
      /*begin leds task*/
      /*docs: https://www.freertos.org/a00131.html */
      ledState = LED_STATE_BATTERY_EFFECT;
      vTaskResume(xHandle_ledStrip);

      /*begin the maze solver task*/
      /*docs: https://www.freertos.org/xTaskNotifyGive.html */
      xTaskNotifyGive(xHandle_mazeSolver);

      /*suspend till the maze solver task finishes*/
      /*docs: https://www.freertos.org/ulTaskNotifyTake.html */
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

      /*suspend the led task*/
      /*docs: https://www.freertos.org/a00130.html */
      vTaskSuspend(xHandle_ledStrip);
      neopixel.clearStrip();

    }
    else
    {
      // do nothing
    }

  }

  /*safety: preventy the task from returning*/
  /*docs: https://www.freertos.org/a00126.html */
  vTaskDelete(NULL);

  
}



void setup() {
    
  /*disable watch dog timer exceptions*/
  rtc_wdt_protect_off();
  rtc_wdt_disable();

  /*delay for power up initialization*/
  delay(500);

  // initialization code
  buttons.init();     /*intialization for the buttons*/
  bluetooth.init();   /*initialization for the bluetooth*/
  mp3.init();         /*initialization for the speaker*/
  sensor.init();      /*intialization for the sensors*/
  motor.init();       /*initalization for the motor*/
  neopixel.init();    /*initialization for the led strip*/

  /*for debugging purposes*/
  Serial.begin(115200); 

  /*wait till someone connects on esp32 bluetooth*/
  bluetooth.waitTillConnection();


  /*create needed tasks, the last paramter in xTaskCreatePinnedToCore is to determine which core to run on where: 0 -> 1st core, 1-> 2nd core as esp32 is a dual core */
  /*docs: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_idf.html#_CPPv423xTaskCreatePinnedToCore14TaskFunction_tPCKcK22configSTACK_DEPTH_TYPEPCv11UBaseType_tPC12TaskHandle_tK10BaseType_t*/
  xTaskCreatePinnedToCore(receiveData, "receive data from the bluetooth", BLUETOOTH_STACK_RCV_DATA, nullptr, tskIDLE_PRIORITY+3 , &xHandle_bluetoothReceiveData, 1);
  xTaskCreatePinnedToCore(lineTracker, "line tracker", LINE_TRACKER_STACK_DATA, nullptr, tskIDLE_PRIORITY+5, &xHandle_lineTracker, 0);
  xTaskCreatePinnedToCore(MazeTrainingTask, "maze trainer", MAZE_TRAINER_STACK_DATA, nullptr, tskIDLE_PRIORITY+5, &xHandle_mazeTrainer, 0);
  xTaskCreatePinnedToCore(MazeSolverTask, "maze solver", MAZE_SOLVER_STACK_DATA, nullptr, tskIDLE_PRIORITY+5, &xHandle_mazeSolver, 0);
  xTaskCreatePinnedToCore(ledStripTask, "led strip", LED_STRIP_STACK_DATA, nullptr, tskIDLE_PRIORITY+3, &xHandle_ledStrip, 1);
  xTaskCreatePinnedToCore(tasksManager, "tasks manager", TASK_MANAGER_STACK_DATA, nullptr, tskIDLE_PRIORITY+15, &xHandle_taskManager, 1);

} 

void loop() {
  /*this is the idle task, it will not be executed as its priority is tskIDLE_PRIORITY, the only case when this task will be executed is
    when all other tasks are suspended or at maximum 1 task is executing as this MCU is dual core*/
}
