// Use the SysTick timer to request interrupts at a particular period.

// Only allow this file to be included ONCE by other files.
#ifndef HEADER_SYSTICK_INCLUDED 
#define HEADER_SYSTICK_INCLUDED

// Copied from the address file
#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode


// These are defined in the startup file (assembly).
void SysTick_Handler(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void systickInterruptHandler(void);

void systickInit(int period) {
    int sr;
    sr = StartCritical();
    NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
    NVIC_ST_RELOAD_R = period;  // reload value
    NVIC_ST_CURRENT_R = 0;      // any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
    NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
    EndCritical(sr);
    EnableInterrupts();
}

void systickSetEnabled(int enabled) {
	if (enabled) NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
    else NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
}

void systickSetPeriod(int period) {
	NVIC_ST_RELOAD_R = period;
    NVIC_ST_CURRENT_R = 0;
}

void SysTick_Handler(void) {
    systickInterruptHandler();
}

#endif // HEADER_SYSTICK_INCLUDED
