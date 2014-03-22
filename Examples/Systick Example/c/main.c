#include "header.h"
#include "systick.h"

#define CLOCK_SPEED 16E6
#define SLOW        (CLOCK_SPEED / 1)
#define MEDIUM      (CLOCK_SPEED / 4)
#define FAST        (CLOCK_SPEED / 8)

Port portF;

int main(void) {       
    
    int enabled = TRUE;
    int speed = SLOW;
    
    // The input buttons' current state and previous state.
	int button1;
	int button1_prev;
    int button2;
	int button2_prev;
		
    // Pins 0 and 4 are inputs, 1, 2, and 3 are outputs
	portF = portInit('F', 0x11, 0x0E);
    
	systickInit(speed);
	systickSetEnabled(enabled);
    
	while(TRUE) {	
        // Remember port F uses negative logic on its buttons!
        
        //
        // Button 1: enable / disable Systick
        //
        button1_prev = button1;
        button1 = read(portF.p4);
        
        // If the button is pressed, but wasn't pressed
        // during the previous loop iteration, toggle interrupts.
        if (button1 != button1_prev && button1 == 0) {
            if (enabled) {			
                setPins(portF, 0);
                enabled = FALSE;
            } else {
                enabled = TRUE;
            }
            systickSetEnabled(enabled);
        }
                
        //
        // Button 2: change Systick period (interrupt frequency)
        //
        button2_prev = button2;
        button2 = read(portF.p0);        
        if (button2 != button2_prev && button2 == 0) {
            if      (speed == SLOW)     speed = MEDIUM;
            else if (speed == MEDIUM)   speed = FAST;
            else                        speed = SLOW;
            systickSetPeriod(speed);
        }
        
        // Pause execution until an interrupt has occurred
        if (enabled) WaitForInterrupt();		
    }
}


/**
 * Interrupt handler: Called when an interrupt occurs
 */
void systickInterruptHandler() {
	
    // static here means index is only initialized once.
    static int index = 0;
    
    // A 3-bit digital wave.
    // Numbers are out of order to show the wave according to the physical locations
    // of the port F LEDs instead of the order used for pin addresses.
    // (Location: Red, Green, Blue. Pins: Red=1, Blue=2, Green=4.)
    static const int wave[] = {0,2,4,6,1,3,5,7,5,3,1,6,4,2};
	
	// Shift bits left by one because we're writing
	// to pins 1-3 instead of 0-2.
	setPins(portF, wave[index] << 1);
	
    // increment the index and start over at 0 if
    // it reaches the length of the wave[] array.
    index = (index + 1) % 14;
}
