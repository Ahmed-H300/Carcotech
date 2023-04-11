#include "mp3.h"

void MP3::init(int songName, bool enablePlayback)
{
    /*initialize the serial*/
    Serial2.begin(9600);

    /*intialize the device*/
    myDFPlayer.begin(Serial2);

    /*set default volume to 30*/
    myDFPlayer.volume(30);

    /*play the song*/
    changeSong(songName, enablePlayback);
}


void MP3::changeSong(int songName, bool enablePlayback)
{
    /*check if to play song with loop or not*/
    if(enablePlayback)
    {
        myDFPlayer.loop(songName);
    }
    else
    {
        myDFPlayer.play(songName);
    }
}

void MP3::playIntermediateSound(int trackName)
{
    /*play the track*/
    myDFPlayer.advertise(trackName);
}