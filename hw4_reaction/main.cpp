#include "mbed.h"
//SPI Declarations
DigitalOut ss1(p14), ss2(p15);
SPI spi(p11,p12,p13);

//Variable for the Current Letter Displayed 
int currentNum;
Timer timer;

//Display method that can display A,B,C,D, or L
void Display(int num)
{
    spi.format(8,0);
    switch(num) {
        case 0:
            ss1 = 0;
            spi.write(0b00011100); //Displays 'L'
            ss1 = 1;
            wait(4);
            break;
        case 1:
            ss1 = 0;
            spi.write(0b11101110); //Displays 'A'
            ss1 = 1;
            currentNum = 1;
            break;
        case 2:
            ss1 = 0;
            spi.write(0b11111110); //Displays 'B'
            ss1 = 1;
            currentNum = 2;
            break;
        case 3:
            ss1 = 0;
            spi.write(0b10011100); //Displays 'C'
            ss1 = 1;
            currentNum = 4;
            break;
        case 4:
            ss1 = 0;
            spi.write(0b11111100); //Displays 'D'
            ss1 = 1;
            currentNum = 8;
            break;
    }
}
//Main method which checks for UserIn and calls Display
int main()
{
    //User Input and Starting time variables
    float userIn;
    float startTime = 1000;
    timer.start();
    
    while(1) {
        
        srand(timer.read_us());
        Display(rand()%4+1);  //Displays a random number between 1 and 4
        wait_ms(startTime);   //waits the intial start time of 1s 
                              //or the consequential decrement
        spi.format(8,2);
        ss2 = 1;
        userIn = spi.write(0b00000000); //Checks to see which button is pressed
        ss2 = 0;
        
        //If else statement with consequences for which button was pushed
        if(userIn == currentNum){ 
        startTime = startTime*.8;
        }
        else{   //resets the game
            Display(0);
            startTime = 1000;
        }
        
    }
}
