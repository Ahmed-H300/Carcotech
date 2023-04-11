#ifndef CONSTANTS_H_
#define CONSTANTS_H_


// related to the bluetooth
#define BLUETOOTH_STACK_SEND_DATA       1024      // used by freeRTOS to create task for sending data
#define BLUETOOTH_MSG_SEND_LEN          5       // used by queues for sending data to the task
#define BLUETOOTH_SEND_DATA_QUEUE_LEN   10      // used by queues to create the length of the queue

#define BLUETOOTH_STACK_RCV_DATA       1024      // used by freeRTOS to create task for sending data
#define BLUETOOTH_MSG_RCV_LEN          5       // used by queues for sending data to the task
#define BLUETOOTH_RCV_DATA_QUEUE_LEN   10      // used by queues to create the length of the queue

// related to the sensor
# define BLACK  0
# define WHITE  1

// related to the buttons
#define LINE_TRACKER_PIN    23
#define MAZE_TRAIN_PIN      4
#define MAZE_SOLVER_PIN     19


#endif