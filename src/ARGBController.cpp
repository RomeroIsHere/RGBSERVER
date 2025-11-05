#include <Arduino.h>
#include <Adafruit_Neopixel.h>
#include "ARGBController.h"
#define thirdOfHue (0x5555)
uint8_t rot(uint32_t c) {
    return (c >> 16);
}
uint8_t grun(uint32_t c) {
    return (c >> 8);
}
uint8_t blau(uint32_t c) {
    return (c >> 0);
}

int ARGBController::customMap(uint16_t degree,int init, int final){
    int end=0xffff;
    return map(abs(degree-end/2),0,end,final,init);
}

uint8_t ARGBController::calculateHue(uint32_t colo){
    int red=rot(colo), green=grun(colo), blue=blau(colo);
    uint8_t DasMax=max(red,max(green,blue));
    uint8_t DasMin=min(red,min(green,blue));
    if (DasMin == DasMax) {
        return 0;
    }
    int hue = 0;
    int Ranged=DasMax-DasMin;
    int Skew=0;
    //0xffff is the full hue Map
    //0xffff/3=0x5555 Each step is an additive
    if (DasMax == red) {
        Skew=(green - blue);
        
    } else if (DasMax == green) {
        Skew=(blue - red);
        hue = thirdOfHue;

    } else {//max is blue
        Skew=(red - green);
        hue = 2*thirdOfHue;
    }
    hue+=map(Skew,-Ranged,Ranged,-thirdOfHue/2,thirdOfHue/2);
    while(hue<0){
        hue+=0x10000;    
    }
    hue=hue%0x10000;

    return hue;
}

ARGBController::ARGBController(int16_t count, int16_t pin){
    ws2812=Adafruit_NeoPixel(count, pin, NEO_GRB + NEO_KHZ800);
    ws2812.begin();
    speed=15;
}
ARGBController::~ARGBController(){
    
}
void ARGBController::rainbow(int strips, int number[]){
    ws2812.rainbow(transitionStep);
    show();
}

void ARGBController::rainbowStrips(int strips, int number[]){
    int offset=0;
    for(int ii=0;ii<strips;ii++){//per strip
        uint16_t degree=0xFFFF/number[ii];//divide the range of full hues
        for(int jj=0;jj<number[ii];jj++){//per led on strip
            ws2812.setPixelColor(jj+offset,calculateColour(transitionStep+degree*jj));
        }
        offset+=number[ii];
    }
    show();
}

void ARGBController::rainbowBlocks(int strips, int number[]){
    
    int offset=0;
    for(int ii=0;ii<strips;ii++){//per strip
        uint16_t degree=0xFFFF/strips;//divide the range of full hues across the strip numbers
        ws2812.fill(calculateColour(transitionStep+degree*ii),offset,number[ii]);
        offset+=number[ii];
    }
    show();
}
void ARGBController::colour(int strips, int number[]){
    ws2812.fill(calculateColour(transitionStep));
    show();
}
void ARGBController::colourBlockTransition(int strips, int number[]){
    int offset=0;
    for(int ii=0;ii<strips;ii++){//per strip
        uint16_t degree=0xFFFF/strips;//divide the range of full hues across the strip numbers
        ws2812.fill(calculateColour(transitionStep+degree*ii),offset,number[ii]);
        offset+=number[ii];
    }
    show();
}
void ARGBController::setSpeed(int sp){
speed=sp;
}
int ARGBController::getSpeed(){
return speed;
}

void ARGBController::setHue(int hue){
    firsthue=hue;
}
int ARGBController::getHue(){
    return firsthue;
}
void ARGBController::setInitColour(uint8_t R, uint8_t G, uint8_t B){
    Ri=R;
    Gi=G;
    Bi=B;   
}
void ARGBController::setFinalColour(uint8_t R, uint8_t G, uint8_t B){
    Rf=R;
    Gf=G;
    Bf=B;
}

int ARGBController::getInitColour(){
    return ws2812.Color(Ri,Gi,Bi);
}
int ARGBController::getFinaltColour(){
    return ws2812.Color(Rf,Gf,Bf);
}
void ARGBController::setMode(int numod){
mode=numod;
}
uint32_t ARGBController::calculateColour(uint16_t degree){
    uint32_t returnal=0;
    degree%=0x10000;//bind to 0-0xffff range
    switch(mode){
        case 0:
        returnal=ws2812.gamma32(ws2812.ColorHSV((firsthue+degree)%0x10000));
        break;
        case 1:
        returnal=ws2812.gamma32(ws2812.Color(customMap(degree,Ri,Rf),customMap(degree,Gi,Gf),customMap(degree,Bi,Bf)));
        break;
        default:
        returnal=degree;
    }
    return returnal;
}
void ARGBController::show(){
    ws2812.show();
    transitionStep+=speed;
}