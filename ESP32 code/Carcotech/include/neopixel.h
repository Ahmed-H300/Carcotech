#ifndef NEO_PIXEL_H_
#define NEO_PIXEL_H_

/*library link: https://github.com/adafruit/Adafruit_NeoPixel */
#include <Adafruit_NeoPixel.h>

// Which pin on the esp32 is connected to the NeoPixels?
#define LED_PIN     26

// How many NeoPixels are attached to the esp32?
#define LED_COUNT   8

class NeoPixel
{
private:
    /*object of neopixel led strip*/
    Adafruit_NeoPixel strip{LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800};

    /*utility function*/
    uint32_t Wheel(byte WheelPos);

public:

    /**
     * @brief this function is used to initialize the neopixel strip
    */
    void init();

    /**
     * @brief Fill strip pixels one after another with a color. Strip is NOT cleared
     *          first; anything there will be covered pixel by pixel. Pass in color
     *          (as a single 'packed' 32-bit value, which you can get by calling
     *          strip.Color(red, green, blue) as shown in the loop() function above),
     *          and a delay time (in milliseconds) between pixels.
     * @param color this is the colour ued in this effect
     * @param wait this is wait time in millisecond till turn off the led
    */
    void colorWipe(uint32_t color, int wait);

    /** 
     * @brief Theater-marquee-style chasing lights. Pass in a color (32-bit value,
     *        a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
     *        between frames.'
     * @param color this is the colour used in this effect
     * @param wait this is wait time in millisecond till turn off the led
    */
    void theaterChase(uint32_t color, int wait);


    /**
     * @brief Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
     * @param wait this is wait time in millisecond till turn off the led
    */
    void rainbow(int wait);


    /**
     * @brief Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
     * @param wait this is wait time in millisecond till turn off the led
    */
    void theaterChaseRainbow(int wait);

    /**
     * @brief utility function to convect rgb color to a single varaible that contains all information about rgb
     * @param r: red component of the color
     * @param g: green component of the color
     * @param b: blue component of the color
    */
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief this fucntion is used to clear the strip
    */
    void clearStrip();


    /**
     * @brief give like a snake colored effect
     * @param whiteSpeed: speed of snake
     * @param whiteLength: length of the snake
    */
    void whiteOverRainbow(int whiteSpeed, int whiteLength);

     /**
     * @brief  create white pulses 
     * @note this function depends on giving leds values so you should have called any other function before this function
     * @param wait this is wait time in millisecond till turn off the led
    */   
    void pulseWhite(uint8_t wait);


    /**
     * @brief create a rainbow effect and fade
     * @param wait this is wait time in millisecond till turn off the led
    */
    void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);

    /**
     * @brief create a simple effect on the strip
     * @param wait this is wait time in millisecond till turn off the led
    */
    void simpleEffect(int wait);

    /**
     * @brief Slightly different, this makes the rainbow equally distributed throughout
     * @param wait this is wait time in millisecond till turn off the led
    */
    void rainbowCycle(uint8_t wait);

    /**
     * @brief this function is used to display battery-charge like effect
     * @param chrageRatio: that repesent how much is battery charged
    */
    void batteryChargeEffect(float_t chrageRatio);


    /**
     * @brief this function is used to turn on all leds with colors that look like battery charge colors
    */
    void turnOnAllLedsBatteryEffect();
    
};


#endif 


