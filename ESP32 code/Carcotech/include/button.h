#ifndef BUTTON_H_
#define BUTTON_H_


// related to the buttons
#define LINE_TRACKER_PIN    23
#define MAZE_TRAIN_PIN      4
#define MAZE_SOLVER_PIN     19

class Button
{
private:
    
    /*this is a function that gets the reading of the button and returns wether its pressed or not*/
    bool isPressed(int pinNumber);

public:

    /**
     * @brief this is the initialization function and has to be called before using any other function
    */
   void init();

   /**
    * @brief this function tells whether button called "Line tracking" is pressed or not
    * @return bool where it returns:
    *           -> true  : if the button is pressed
    *           -> false : if the button isn't presses
   */
  bool isBeginLineTrackingPressed();

   /**
    * @brief this function tells whether button called "Maze Train" is pressed or not
    * @return bool where it returns:
    *           -> true  : if the button is pressed
    *           -> false : if the button isn't presses
   */
  bool isBeginMazeTrainingPressed();
  

   /**
    * @brief this function tells whether button called "Maze Solve" is pressed or not
    * @return bool where it returns:
    *           -> true  : if the button is pressed
    *           -> false : if the button isn't presses
   */
  bool isBeginMazeSolvingPressed();
  
};


#endif