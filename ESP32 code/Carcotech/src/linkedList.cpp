#include "linkedList.h"

void LinkedList::addToList(uint8_t direction)
{
    traverseList.push_back(direction);
}

uint8_t* LinkedList::convertToArr()
{

    /*this is the array that holds the traverse values*/
    uint8_t* traverseArr = nullptr;
    
    /*get the size of the list*/
    uint32_t size = traverseList.size();

    /*allocate memory*/
    traverseArr = new uint8_t[size+1];

    /*move the values to the array*/
    uint32_t i = 0;
    for(uint8_t data : traverseList)
    {
        traverseArr[i] = data;
        i++;
    }

    /*add a terminating character*/
    traverseArr[i] = '\0';

    /*free space of the linked list*/
    traverseList.clear();

    /*return the new array*/
    return traverseArr;
}
