#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

/*library link: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial*/
#include "BluetoothSerial.h"

class Bluetooth
{
private:

    /*this is object used to access bluetooth utils*/
    BluetoothSerial SerialBT;

public:

    /*this is the name of the esp32 when being searched for*/
    const String device_name = "ESP32-Carcotech";

    /*this is the Pin number for connection with ESP for legacy devices*/
    const char *pin = "1234";

    /*constructor*/
    Bluetooth();
  
    /*destructor*/
    ~Bluetooth();
    
    /**
     * @brief used to receive a string from the bluetooth
     * @param dataLen: is the length of the expected data to receive from the bluetooth module
     * @param timeOutMS: is the timeOut to wait for if not all data received : put 0 to wait infinitely if didn't till recevie same dataLen
    */
    String getData(int dataLen, uint32_t timeOutMS);

    /**
     * @brief used to send data over the bluetooth module
     * @param data: this is the data to send
    */
    void sendData(String data);


    /**
     * @brief this is the initialization function, for some reason putting the intialization code in the 
     * constructor results in watchdog timer to reset the esp32 over and over and over
    */
    void init();

};

#endif