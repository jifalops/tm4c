#include "header.h"

Port portF;

void delay(int count) {
    while (count > 0) --count;
}

int main(void) {
    
    // buttons
    int b1;
    int b2;
    
    // Pins 0 and 4 are inputs, 1, 2, and 3 are outputs
	portF = portInit('F', 0x11, 0x0E);
    
	while(TRUE) {	
        // Remember port F uses negative logic on its buttons!
    
        b1 = read(portF.p4);
        b2 = read(portF.p0);
               
        if      (b1 == 0 && b2 != 0) set(portF.p1);     // If only b1 is pressed, turn on red LED.
        else if (b1 != 0 && b2 == 0) set(portF.p2);     // If only b2 is pressed, turn on blue LED.
        else if (b1 == 0 && b2 == 0) set(portF.p3);     // If b1 and b2 are pressed, turn on green LED.
        else    setPins(portF, 0);                      // Otherwise, turn all LEDs off.
        
        // 1E6 = 1x10^6 = 1,000,000        
        delay(1E6);
    }
}
