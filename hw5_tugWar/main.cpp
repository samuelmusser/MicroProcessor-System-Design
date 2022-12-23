#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)
#define FIO1DIR (*(volatile unsigned int *)0x2009c020)
#define FIO1PIN (*(volatile unsigned int *)0x2009c034)
#define FIO2DIR (*(volatile unsigned int *)0x2009c040)
#define FIO2PIN (*(volatile unsigned int *)0x2009c054)
#define PINMODE0 (*(volatile unsigned int *)0x4002c040)
#define PINMODE1 (*(volatile unsigned int *)0x4002c044)

//wait function to avoid immediate victories
void wait(float s)
{
    volatile int countDown = s*21.33e6;
    while (countDown >0) {
        countDown--;
    }
}

// function for metastability
volatile int meta;
//shifting method that determines which LEDS should be on
void shiftMe(int k)
{
    switch(k) {
        //red red (victory
        case 0:
            FIO0PIN |= (1<<16) | (1<<25);
            meta;
            FIO1PIN &= ~(1<<18);
            break;
        //blue and red
        case 1:
            FIO0PIN &= ~(1<<25);
            meta;
            FIO0PIN |= (1<<16);
            meta;
            FIO1PIN |= (1<<18);
            meta;
            FIO1PIN &= ~(1<<20);
            break;
        //left blues
        case 2:
            FIO0PIN &= ~(1<<16);
            meta;
            FIO1PIN |= (1<<18) | (1<<20);
            meta;
            FIO1PIN &= ~(1<<21);
            break;
        //middle blues
        case 3:
            FIO1PIN |= (1<<20) | (1<<21);
            meta;
            FIO1PIN &= ~(1<<18) & ~(1<<23);
            break;
        //right blues
        case 4:
            FIO1PIN &= ~(1<<20);
            meta;
            FIO1PIN |= (1<<21) | (1<<23);
            meta;
            FIO2PIN &= ~(1<<2);
            break;
        //green and blue
        case 5:
            FIO1PIN &= ~(1<<21);
            meta;
            FIO1PIN |= (1<<23);
            meta;
            FIO2PIN |= (1<<2);
            meta;
            FIO2PIN &= ~(1<<5);
            break;
        //green green (victory)
        case 6:
            FIO1PIN &= ~(1<<23);
            meta;
            FIO2PIN |= (1<<2) | (1<<5);
            break;
    }
}


int main()
{
    //placement indicator
    int hereIam = 3; //starts in the middle

    //Output declarations
    FIO0DIR |= (1<<25);  //output to red1
    FIO0DIR |= (1<<16);  //output to red2
    FIO1DIR |= (1<<18);  //output to blue1
    FIO1DIR |= (1<<20);  //output to blue2
    FIO1DIR |= (1<<21);  //output to blue3
    FIO1DIR |= (1<<23);  //output to blue4
    FIO2DIR |= (1<<2);   //ouput to green1
    FIO2DIR |= (1<<5);   //output to green2

    //inputs
    FIO0DIR &= ~(1u<<18);  //input at pin11 (red)
    FIO0DIR &= ~(1u<<10); //input at pin28   (green)

    //Pulldowns
    PINMODE0 |= (1u<<20) | (1 <<21);  //Pull down for greenSwitch
    PINMODE1 |= (1u<<5) | (1 <<4); //pull down for redSwitch
    
    //initial state lights
    shiftMe(hereIam);
    wait(0.01);

    //game loop
    while(1) {
        
        //checks for red switch then shifts left
        if(((FIO0PIN>>18) & 1) == 1) {       //rise edge
            wait(0.01);
            if (((FIO0PIN>>18) & 1) == 0) {  //fall edge
                hereIam = hereIam - 1;
                shiftMe(hereIam);
            }
        }
        
        //checks for green switch then shifts right
        if(((FIO0PIN>>10) & 1) == 1) {      //rise edge
            wait(0.01);
            if(((FIO0PIN>>10) & 1) == 0) {  //fall edge
                hereIam = hereIam + 1;
                shiftMe(hereIam);
            }
        }
        //checks for a win then triggers endless loop until restart
        if ((hereIam == 0)||(hereIam == 6)) {
            while(1) {}
        }
    }
}
