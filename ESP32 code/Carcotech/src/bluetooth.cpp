#include "bluetooth.h"

Bluetooth::Bluetooth()
{


}

String Bluetooth::getData(int dataLen, uint32_t timeOutMS)
{
    // this is a counter for how many data we received
    int receivedCounter = 0;

    // this is the buffer to receive in
    String data;

    // setting the timeOut
    SerialBT.setTimeout(timeOutMS);    

    // check if we should wait indefinitely or not
    if(timeOutMS == 0)
    {   

        while(SerialBT.available())
        {
            if(receivedCounter == dataLen)
                break;
            
            data += (char)SerialBT.read();
            receivedCounter++;
            taskYIELD();
            //vTaskDelay(pdMS_TO_TICKS(1));
        }
        
    }
    else
    {

        while(SerialBT.available())
        {
            int temp = SerialBT.read();

            if(receivedCounter == dataLen || temp == -1 )
                break;
            
            data += (char)SerialBT.read();
            receivedCounter++;
            taskYIELD();
            //vTaskDelay(pdMS_TO_TICKS(1));
        }

    }

    return data + "\0";
}

void Bluetooth::sendData(String data)
{
    SerialBT.write((const uint8_t *)data.c_str(), strlen(data.c_str()));
}

void Bluetooth::init()
{
    
    // initialization of the bluetooth
    SerialBT.begin(device_name);

    // telling the bluetooth to use that pin for connection
    SerialBT.setPin(pin);
}


Bluetooth::~Bluetooth()
{
}