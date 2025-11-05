#ifndef ARGBController_h
#define ARGBController_h    
#include <Adafruit_Neopixel.h>
#include <vector>

class ARGBController{
    public:
        ARGBController();
        ARGBController(int16_t count, int16_t pin);
        virtual ~ARGBController();
        int customMap(uint16_t degree,int init, int final);
        void rainbow(int strips, int number[]);
        void rainbowStrips(int strips, int number[]);
        void rainbowBlocks(int strips, int number[]);
        void colour(int strips, int number[]);
        void colourBlockTransition(int strips, int number[]);
        void setSpeed(int sp);
        int getSpeed();
        void setHue(int hue);
        int getHue();
        void setInitColour(uint8_t R, uint8_t G, uint8_t B);
        void setFinalColour(uint8_t R, uint8_t G, uint8_t B);
        int getInitColour();
        int getFinaltColour();
        void setMode(int numod);
        uint32_t calculateColour(uint16_t degree);
        static uint8_t calculateHue(uint32_t colo);
    protected:
        void show();
    private:
    uint8_t Ri; uint8_t Gi; uint8_t Bi;
    uint8_t Rf; uint8_t Gf; uint8_t Bf;
    int mode;
    uint16_t firsthue;
    int transitionStep;
    int pin;
    int speed;
    Adafruit_NeoPixel ws2812;
};

#endif