#include "mbed.h"

//These are declaring and naming the output pins for red LEDS
DigitalOut red1(p13);
DigitalOut red2(p12);
DigitalOut red3(p11);

//These are declaring and naming the output pins for green LEDS
//which are active high
DigitalOut green1(p28,1);
DigitalOut green2(p29,1);
DigitalOut green3(p30,1);

//These are declaring the switches
DigitalIn red(p19);
DigitalIn green(p20);
DigitalIn killSwitch(p21);

//These methods refer to the inputs of the button presses
int Pressed1(void){
    return red == 0;
}
int Pressed2(void){
    return green == 0;
}
int Pressed3(void){
    return killSwitch == 1;
}
    
//The following are the set methods and the turn off method
//
//checks for off red LEDS and turns them on in order
void redOn(){
    if (red1 == 0){
        red1 = 1;
        }
    else{
        if (red2 == 0){
            red2 = 1;
            }
        else{
            if (red3 == 0){
                red3 = 1;
            }
        }
    }
}
//checks for off green LEDS and turns them on in order
void greenOn(){
    if (green1 == 1){
        green1 = 0;
        }
    else{
        if (green2 == 1){
            green2 = 0;
            }
        else{
            if (green3 == 1){
                green3 = 0;
            }
        }
    }
}
//checks for on LEDS and turns them off as found
void turnOff(){
    if (green1 == 0 || red1 == 1){
        green1 = 1;
        red1 = 0;
        }
    else{
        if (green2 == 0 || red2 == 1){
            green2 = 1;
            red2 = 0;
            }
        else{
            if (green3 == 0 || red3 == 1){
                green3 = 1;
                red3 = 0;
            }
        }
    }
}

//Main method executes loop waiting for switch activation with cycles of 350ms                 
int main() {
    while(1) {
        if (Pressed1()){
            wait_ms(250);
            redOn();
            wait_ms(350);
        }
        if (Pressed2()){
            wait_ms(250);
            greenOn();
            wait_ms(350);
            }
        if (Pressed3()){
            wait_ms(250);
            turnOff();
            wait_ms(350);
    }
}
}
