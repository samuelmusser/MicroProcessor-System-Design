#include "mbed.h"

//In methods for MBED
AnalogIn vIn(p20);
InterruptIn cSwitch(p21, PullDown), fSwitch(p23, PullUp);

//Actively changeable mode for temperature conversions
volatile int mode;

//The two titles of each respective temperature data
float celsius, fahrenheit;

//Conversion from volts to degrees Celsius
float tempConversion(float volts)
{
    celsius = (((3.3*volts)-0.500)/0.010);
    return celsius;
}

//Averages the voltages to avoid minor fluctuation or noise
float averageVolts(void)
{
    float num = 0;
    for (int i = 0; i <1000; ++i) {
        num = num + vIn.read();
    }
    return num/1000;
}

//7 seg display bus
BusOut tempDisp(p5,p6,p7,p8,p9,p10,p11,p12);  //segments from a to dp

//cases for number to display with input of respective digit
void displayTemperature(int x)
{
    switch(x) {
        case 0:
            tempDisp = 0x3F;  //displays 0
            break;
        case 1:
            tempDisp = 0x06;  //displays 1
            break;
        case 2:
            tempDisp = 0x5B;  //displays 2
            break;
        case 3:
            tempDisp = 0x4F;  //displays 3
            break;
        case 4:
            tempDisp = 0x66;  //displays 4
            break;
        case 5:
            tempDisp = 0x6D;  //displays 5
            break;
        case 6:
            tempDisp = 0x7D;  //displays 6
            break;
        case 7:
            tempDisp = 0x07;  //displays 7
            break;
        case 8:
            tempDisp = 0x7F;  //displays 8
            break;
        case 9:
            tempDisp = 0x6F;  //displays 9
            break;
    }
    wait_ms(750);
    tempDisp = 0x00;
    wait(0.1);
}

//Mode select which is directly effected by the button presses in MM
void setMode0(void)
{
    mode = 0;
}
void setMode1(void)
{
    mode = 1;
}

//Main Method which checks for mode select then calls conversion and display
//methods for the temp in either C or F
int main()
{
    int digit;
    int cel, fah;


    //mode select
    cSwitch.rise(setMode0);
    fSwitch.fall(setMode1);

    while(1) {
        //refresh after every loop for temp changes
        cel = (int)tempConversion(averageVolts());
        fah = (int)(tempConversion(averageVolts())*9/5)+32;

        //Celsius Mode
        if (mode == 0) {
            if (celsius < 0) {
                tempDisp = 0x40;
                wait_ms(750);
                cel = cel*-1;
            }
            digit = (cel/10)%10;
            displayTemperature(digit);

            digit = (cel)%10;
            displayTemperature(digit);

            tempDisp = 0x39;
            wait_ms(750);
            tempDisp = 0x00;
            wait_ms(100);
        }

        //Fahrenheit Mode
        if (mode == 1) {
            if (fahrenheit <= 0) {
                tempDisp = 0x40;
                wait_ms(750);
                fah = fah *-1;
            }
            digit = (fah/10)%10;
            displayTemperature(digit);
            digit = (fah)%10;
            displayTemperature(digit);
            tempDisp = 0x71;
            wait_ms(750);
            tempDisp = 0x00;
            wait_ms(100);
        }
    }
}


