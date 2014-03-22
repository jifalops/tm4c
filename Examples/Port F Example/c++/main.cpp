#include "header.h"

void delay(int count) {
    while (count > 0) --count;
}

int main(void) {
    
    // buttons
    bool b1;
    bool b2;
    
    // Pins 0 and 4 are inputs, 1, 2, and 3 are outputs
	Port PF = Port('F', 0x11, 0x0E);
    
	while(true) {	
        // Remember port F uses negative logic on its buttons!    
        b1 = (PF.read(4) == 0);
        b2 = (PF.read(0) == 0);
               
        if      (b1 && !b2) PF.set(1);      // If only b1 is pressed, turn on red LED.
        else if (!b1 && b2) PF.set(2);      // If only b2 is pressed, turn on blue LED.
        else if (b1 && b2) PF.set(3);       // If b1 and b2 are pressed, turn on green LED.
        else    PF.setPins(0);              // Otherwise, turn all LEDs off.
        
        // 1E6 = 1x10^6 = 1,000,000        
        delay(1E6);
    }
}
