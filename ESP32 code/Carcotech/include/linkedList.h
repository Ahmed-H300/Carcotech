#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/*for the linked list library from stl library*/
#include <list>

class LinkedList
{
private:
    /*this is the actual linked list*/
    std::list <uint8_t> traverseList;

public:

    /**
     * @brief this is to add a character to the end of the list
     * @param direction: this holds the new direction visisted by the car, refer to @MAZE_TRAVERSE_POSSIBLE_DIR in utils.h
    */
    void addToList(uint8_t direction);

    /**
     * @brief this fuction is used to convert the linked list into array and free the linked list space to only use stack not heap and return a pointer to that array
     * @return uint8_t* this is a pointer to the array holding the traverse directions of the car
    */
    uint8_t* convertToArr();
};



#endif