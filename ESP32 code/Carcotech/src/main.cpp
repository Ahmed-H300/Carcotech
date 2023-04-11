#include <Arduino.h>

#include "constants.h"
#include "sensor.h"
#include "bluetooth.h"
#include "button.h"

// utility objects
Bluetooth bluetooth;  /*this is the object throught which we can control the bluetooth*/
Sensor sensor;        /*this is the sensor object which is used to read the sensors values*/    
Button buttons;       /*this is the object that will represent our interface buttons*/


// task handlers
TaskHandle_t xHandle_bluetoothSendData = NULL;    /*this is the task handler for the bleutooth send data task*/
TaskHandle_t xHandle_bluetoothReceiveData = NULL; /*this is the task handler for the bleutooth receive data task*/


// Queue handlers
QueueHandle_t xQueue_bluetoothSendData = NULL;    /*this is the queue used for bluetooth send data queue task*/
QueueHandle_t xQueue_bluetoothRcvData = NULL;     /*this is the queue used for bluetooth receive data queue task*/


// task queues contents structures
typedef struct
{
  char data[BLUETOOTH_MSG_SEND_LEN];
}bluetoothSendMsg_t;

typedef struct
{
  char data[BLUETOOTH_MSG_RCV_LEN];
}bluetoothRcvMsg_t;



/**
 * @brief this is the task used for sending data from esp32
 * TODO: make the function use notifications instead of just waiting on queues
*/
void sendData(void* pvParameters)
{
  /*struct for sending data*/
  bluetoothSendMsg_t *sendMessage;

  while (1)
  { 
    /*delay for good luck*/
    vTaskDelay(pdMS_TO_TICKS(10));

    /*get what's in the queue*/
    if(xQueue_bluetoothSendData != NULL && xQueueReceive(xQueue_bluetoothSendData, &sendMessage, (TickType_t) 1000) == pdPASS)
    {
      bluetooth.sendData(sendMessage->data);      /*sending the message received back to receiver*/ 
    }
  }
}

/**
 * @brief this is the task used for reveiving data by esp32
 * TODO: make the function use notifications instead of just waiting on queues
*/
void receiveData(void* pvParameters)
{
  /*for sending and receiving structs*/
  bluetoothRcvMsg_t receivedMsg;
  bluetoothSendMsg_t *messageTobeSend;

  while (1)
  {
    /*delay for good luck*/
    vTaskDelay(pdMS_TO_TICKS(10));

    /*receiving message form the receiver from the receiver and put it in a queue to be send back*/
    strcpy(receivedMsg.data, bluetooth.getData(BLUETOOTH_MSG_RCV_LEN, 0).c_str());

    /*for some reason we received an empty message*/
    if(receivedMsg.data[0] != '\0')
    {

      messageTobeSend = (bluetoothSendMsg_t*) pvPortMalloc(sizeof(bluetoothSendMsg_t)); /*make a place in the heap for this new message*/
      strcpy(messageTobeSend->data, receivedMsg.data);  /*copying received message to another struct that will be send back*/ 
      xQueueSend(xQueue_bluetoothSendData, (void*) &messageTobeSend, (TickType_t) 1000); // send back to the queue
      receivedMsg.data[0] = '\0'; // for safety
    }
  }
  
}

void setup() {
  
  // put your setup code here, to run once:
  //bluetooth.init(); /*initialization for the code*/
  sensor.init();      // initialization for the bluetooth

  /*for debugging purposes*/
  Serial.begin(115200); 
  while(!Serial);

  

  /*create needed queues for inter-task communication*/
  /*docs: https://www.freertos.org/a00116.html*/
  //xQueue_bluetoothSendData = xQueueCreate(BLUETOOTH_SEND_DATA_QUEUE_LEN, sizeof(bluetoothSendMsg_t*));
  //xQueue_bluetoothRcvData = xQueueCreate(BLUETOOTH_RCV_DATA_QUEUE_LEN, sizeof(bluetoothRcvMsg_t*));
  

  /*create needed tasks*/
  /*docs: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_idf.html#_CPPv423xTaskCreatePinnedToCore14TaskFunction_tPCKcK22configSTACK_DEPTH_TYPEPCv11UBaseType_tPC12TaskHandle_tK10BaseType_t*/
  //xTaskCreatePinnedToCore(sendData, "Send data to bluetooth", BLUETOOTH_STACK_SEND_DATA, nullptr, tskIDLE_PRIORITY , &xHandle_bluetoothSendData, tskNO_AFFINITY);
  //xTaskCreatePinnedToCore(receiveData, "receive dat from the bluetooth", BLUETOOTH_STACK_RCV_DATA, nullptr, tskIDLE_PRIORITY , &xHandle_bluetoothReceiveData, tskNO_AFFINITY);

  buttons.init();

} 

void loop() {
  Serial.print("the value for line tracking button = ");
  Serial.println(buttons.isBeginLineTrackingPressed());
  Serial.print("the value for maze solving button = ");
  Serial.println(buttons.isBeginMazeSolvingPressed());
  Serial.print("the value for Maze training button = ");
  Serial.println(buttons.isBeginMazeTrainingPressed());
  Serial.println("------------------------------------");
  delay(500);
}