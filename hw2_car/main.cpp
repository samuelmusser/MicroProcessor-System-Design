#include "mbed.h"
//Homework 2 by Sam Musser

//Declarations for the piezo and LEDS
//which can turn on immediately or ascendingly
PwmOut piezo(p22), red(p21), blue(LED1);

//Declarations for the pins that are activated 
//by the two switches
DigitalIn twoTone(p19, PullDown), warble(p20, PullDown); 

//Push methods for the switches
int Pressed1(void){
    return twoTone == 1;
}
int Pressed2(void){
    return warble == 1;
}

int main() {
    while(1) {
       //This if statement is for the twoTone switch and
       //alternates between 960Hz/Red and 770Hz/Blue frequencies and LEDs
       //every 700 ms
       if (twoTone){
           piezo.period(1.0/960);
           piezo = 0.5;
           red = 1;
           blue = 0;
           wait_ms(700);
           piezo.period(1.0/770);
           piezo = 0.5;
           red = 0;
           blue = 1;
           wait_ms(700);
           } 
       // This if statement ascends and descends between 1400 and 1600Hz with 
       //a red LED doing the same from full brightness to comepletely 
       //off every 0.25s
       else if (warble){
           red = 1;
           for (int i = 1400; i < 1600; i=i+1){
              piezo.period(1.0/i);
              piezo = 0.5;
              red = red - 0.005;
              wait_ms(1.25);
           } 
           for (int i = 1600; i > 1400; i=i-1){
              piezo.period(1.0/i);
              piezo = 0.5;
              red = red + 0.005;
              wait_ms(1.25);
           }
        }
        //The default conditions are off for the LEDs and piezo
        else{
           piezo = 0;
           red = 0;
           blue = 0;
           wait_ms(250);
           }
    }
}
