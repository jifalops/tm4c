#include "header.h"
#include "systick.h"

/*
    Using port D
    pins 0-2: RYG LEDs for north-south direction
    pins 3-5: RYG LEDs for east-west direction
    pin 6: NS walk button (not implemented)
    pin 7: EW walk button (not implemented)
    
    Using port E instead of D for pins 4 and 5 (yellow and green for east-west direction)
		(Port D pins 4 and 5 are hardwired to the USB).
*/

// functions defined in this file
void setState1(void);		// NS = G, EW = R		(north-south is green, east-west is red)
void setState2(void);		// NS = Y, EW = R
void setState3(void);		// NS = R, EW = G
void setState4(void);		// NS = R, EW = Y

Port portD, portE;
int NStime, EWtime;
const int DEFAULT_GREEN_TIME = 6;


int main(void) {
    
    portD = portInit('D', 0xC0, 0x0F);
    portE = portInit('E', 0x00, 0x30);
    
    // Clock is 16MHz, so 1 second is 16 million cycles.
		// This will allow control using 1 second granularity.
    systickInit(16E6);
    systickSetEnabled(TRUE);
	
		setState1();
    
		// Main loop
    while(TRUE) {
        if(read(portX.p0)) {
           NStime = DEFAULT_GREEN_TIME / 2; 
        }
        if(read(portX.p1)) {
           EWtime = DEFAULT_GREEN_TIME / 2; 
        }
        //WaitForInterrupt();
    }
}

void setState1() {
    // turn off all outputs
    setPins(portD, 0);
    setPins(portE, 0);
    
    // turn on red LED for EW
    set(portD.p3);
    
    // turn on NS green only if EW red is on
    if (isSet(portD.p3)) {
        set(portD.p2);
    }
}

void setState2() {
    // We are in the initial state
    // change to yellow for north-south
    clear(portD.p2);
    set(portD.p1);
}

void setState3() {
    // We are in the 2nd state (NS = yellow)
    clear(portD.p1);
    set(portD.p0);
    
    // Before turning on a green light,
    // check that the other direction's 
    // red light is on.
    if (isSet(portD.p0)) {
        clear(portD.p3);
        set(portE.p5);
    }
}

void setState4() {
    // We are in the 3rd state (EW = Green)
    // Go to 4th state
    clear(portE.p5);
    set(portE.p4);
}

// Will be called every second
void systickInterruptHandler() {
		// Keep a counter for the number of interrupts.
		// 'static' means it will be initialized only once, during compilation.
		static int count = 0;
    
        
        
		// Increment the count every time there is an interrupt.
		++count; 
    
    if (isSet(portD.p2)) {
        // We are in 1st state
        if (count >= NStime) {
			// NS has been green for 6 (or more) seconds
            setState2();   
			count = 0;
            NStime = DEFAULT_GREEN_TIME;
        }          
    } else if (isSet(portD.p1)) {
        // We are in 2nd state
        if (count >= 2) {
			// NS has been yellow for 2 (or more) seconds
            setState3();
			count = 0;
        }
    } else if (isSet(portE.p5)) {
        // We are in 3rd state
        if (count >= EWtime) {
            // EW has been green for 6 (or more) seconds
            setState4();
			count = 0;
            EWtime = DEFAULT_GREEN_TIME;
        }
    } else {        
        // Assume we are in 4th state, restart at initial state.
        if (count >= 2) {
			// EW has been yellow for 2 (or more) seconds
            setState1();
			count = 0;
        }
    }
}
