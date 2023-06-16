#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "soc/rtc_wdt.h"

// specific led modes
#define LED_STATE_BATTERY_EFFECT    0
#define LED_STATE_RADNOM_EFFECT     1
#define LED_STATE_BLUETOOTH         2


// specific speeds used by the line tracker
#define MAX_SPEED           110     // this is the speed of the lower speed wheel when taking curves 
#define CURVE_SPEED         240     // this is the speed of the higher speed wheel when taking curves
#define UPPER_LINE_SPEED    250     // this is the speed that we will use in case of straight line and the mobile app allowed us to use max speed
#define LOWER_LINE_SPEED    210     // this is the speed that we will use in case of straight line and the mobile app didn't allow us to use max speed


// specific speeds used by the maze
#define MAZE_LINE_SPEED                 110 //100
#define MAZE_ALIGN_DIFF_SPEED           100
#define MAZE_LINE_SPEED_OPPOSITE_DIR    150 //150
#define STOP_DELAY                      100


// this is the fork status @CAR_FORK_CASE
#define LINE_FORK_CROSS         0           // which looks like the plus sign + so possible directions are right, left and forward
#define LINE_FORK_T             1           // which looks like letter T so possible directions are right and left
#define LINE_FORK_T_ANG_90      2           // which looks like letter T but rotated with angle 90 so possible directions are right and forward
#define LINE_FORK_T_ANG_NEG_90  3           // which looks like letter T but rotated with angle -90 so possible directions are left and forward
#define LINE_FORK_LEFT          4           // which means a left only branch so possible directions are only left
#define LINE_FORK_RIGHT         5           // which means a right only branch  so possible directions are only right
#define LINE_FORK_DEADEND       6           // this is a deadlock line so no possible directions
#define LINE_FORK_END           7           // this is the end of the maze so stop 


// related to the bluetooth task
#define BLUETOOTH_STACK_SEND_DATA       1024      // used by freeRTOS to create task for sending data
#define BLUETOOTH_MSG_SEND_LEN          5       // used by queues for sending data to the task
#define BLUETOOTH_SEND_DATA_QUEUE_LEN   10      // used by queues to create the length of the queue

#define BLUETOOTH_STACK_RCV_DATA       1024      // used by freeRTOS to create task for sending data
#define BLUETOOTH_MSG_RCV_LEN          1       // used by queues for sending data to the task
#define BLUETOOTH_RCV_DATA_QUEUE_LEN   10      // used by queues to create the length of the queue


// related to line tracker task 
#define LINE_TRACKER_STACK_DATA        2048     // used by freeRTOS to crate task for tracking line


// related to maze trainer task     
#define MAZE_TRAINER_STACK_DATA        2048     // used by freeRTOS to crate task for training on the maze to solve it

// related to maze solver task         
#define MAZE_SOLVER_STACK_DATA         2048     // used by freeRTOS to crate task for solving the maze

// related to task manager task
#define TASK_MANAGER_STACK_DATA        1024     // used by freeRTOS to crate task for creating a task that manages other tasks

// related to led strip tasl   
#define LED_STRIP_STACK_DATA           1024     // used by freeRTOS to crate task for manipulating leds


// related to EEPROM
#define STORE_ADDR      0           // this is the address that we will begin storing the values in it


// related to the sensor
#define MIN_NUM_OF_READINGS     2  // used to read stable value where the sensor is determined to be stable when reading 10 times has same readings


// undefined behaviour define
#define UNDEFINED_BEHAVIOUR         255

// time period of alignments
#define TIME_PERIOD_ALIGNM_MS       10


#endif