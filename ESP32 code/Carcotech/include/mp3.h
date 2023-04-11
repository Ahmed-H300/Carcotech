#ifndef  _MP3_H_
#define _MP3_H_


/*library link: https://github.com/DFRobot/DFRobotDFPlayerMini*/
#include "DFRobotDFPlayerMini.h"

class MP3
{

private:

    /*this is the name of default song that will be played during solving the maze where nameOfSong = 0.mp3 or 0.wav*/
    int defaultSong = 0;

    /*this is the object through which we will access the mp3 player*/
    DFRobotDFPlayerMini myDFPlayer;

public:

    /**
     * @brief this is the initialization function, it should be called before using any function in the library
     * @param songName: this is the name of default song where fileName = songName.mp3 or songName.wav and songName is int value from 0 to 65553 
     * @param enablePlayback: this is a boolean variable that enables the song to be played back or not
    */
   void init(int songName, bool enablePlayback);

    /**
     * @brief this function is used to change the song
     * @param songName: this is the name of default song where fileName = songName.mp3 or songName.wav and songName is int value from 0 to 65553 
     * @param enablePlayback: this is a boolean variable that enables the song to be played bac
    */
   void changeSong(int songName, bool enablePlayback);


    /**
     * @brief this function is used to play intermediate sound by stopping the current song then play the intermediate sound then go back to continue original sound
     * @param songName: this is the name of the track to be played where the track should be stored in SD:/ADVERT/ so for example if songName = 5 then the path of track 
     *                  that will be played will be SD:/ADVERT/0005.mp3
    */
    void playIntermediateSound(int trackName);

};


#endif