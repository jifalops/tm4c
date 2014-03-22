#include "header.h"
#include "pll.h"

#define PLL_DIV_MIN 4
#define PLL_DIV_MAX 127

Port portF;

void delay(int count) {
    while (count > 0) --count;
}

void incDivider(int *divider) {
    (*divider) += 3;
    if (*divider > PLL_DIV_MAX) *divider = PLL_DIV_MIN;
}

void decDivider(int *divider) {
    (*divider) -= 3;
    if (*divider < PLL_DIV_MIN) *divider = PLL_DIV_MAX; 
}

int main(void) {    
    
    // The input buttons' current state and previous state.
	int button1;
	int button1_prev;
    int button2;
	int button2_prev;
    
    // Which LED to toggle
    int led = 1;    
    
    // 24 = 16MHz bus speed (same as without using PLL)
    // See pll.h for alternatives.
    int divider = 24;
    
    pllInit(divider);
    
    // Pins 0 and 4 are inputs, 1, 2, and 3 are outputs
	portF = portInit('F', 0x11, 0x0E);
    
	while(TRUE) {
        // Remember port F uses negative logic on its buttons!        
          
        // Toggle the current LED and move to the next
        switch (led) {
            case 1:
                toggle(portF.p1);
                if (read(portF.p1) == 0) led = 2;
                break;
            case 2:
                toggle(portF.p2);
                if (read(portF.p2) == 0) led = 3;
                break;
            case 3:
                toggle(portF.p3);
                if (read(portF.p3) == 0) led = 1;
                break;
        }
   
        
        // save the previous state of the buttons (previous loop iteration)
        button1_prev = button1;
        button2_prev = button2;
        
        // read the current state of the buttons
        button1 = read(portF.p4);        
        button2 = read(portF.p0);
        
        // If the button is pressed, but wasn't pressed
        // during the previous loop iteration, change speed.
        if (button1 != button1_prev && button1 == 0) {        
            incDivider(&divider);
            pllSetDiv(divider);
        }
        
        if (button2 != button2_prev && button2 == 0) {
            decDivider(&divider);
            pllSetDiv(divider);
        }
        
        // 1E6 = 1x10^6 = 1,000,000        
        delay(1E6);
    }
}
