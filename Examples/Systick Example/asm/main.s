    ; Allow other files to call these labels
    EXPORT start
    EXPORT SysTick_Handler

    ; Import labels (function calls) from tm4c.s
    IMPORT portInit     ; initialize a port             in:  r0=port_letter(upper-case) r1=output_pins(pin-directions)             
                        ;                               out: r0=port_base_address
                        ;                               mod: r0-r3
    IMPORT readPin      ; read a port's pin             in:  r0=port_base_address       r1=pin_number(0-7)
                        ;                               out: r2=pin_value
                        ;                               mod: r2,r3
    IMPORT setPin       ; turn on a port's pin          in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: none
                        ;                               mod: r2,r3
    IMPORT clearPin     ; turn off a port's pin         in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: none
                        ;                               mod: r2,r3
    IMPORT togglePin    ; toggle a port's pin           in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: r2=new_pin_value
                        ;                               mod: r2,r3
    IMPORT readPins     ; read all pins on a port       in:  r0=port_base_address 
                        ;                               out: r1=pin_values
                        ;                               mod: r1
    IMPORT setPins      ; set values for port's pins    in:  r0=port_base_address       r1=pin_values
                        ;                               out: none
                        ;                               mod: r1,r2	

    
    ; Import labels (function calls) from systick.s
    ;
    ; IMPORTANT: SysTick_Handler (defined in the startup file) is called by the system 
    ;            whenever an interrupt from Systick occurs. To implement it, simply 
    ;            create a label with the name "SysTick_Handler" (case-sensitive), and
    ;            add the line "EXPORT SysTick_Handler" near the top of the file.   
    IMPORT systickInit          ; initialize systick            in:  r0=period
                                ;                               out: none
                                ;                               mod: r0-r3                                    
    IMPORT systickSetEnabled    ; enable/disable systick        in:  r0=zero-or-nonzero
                                ;                               out: none
                                ;                               mod: r1-r3    
    IMPORT systickSetPeriod     ; set systick reload value      in:  r0=period
                                ;                               out: none
                                ;                               mod: r1  


CLOCK_SPEED     EQU 16000000
SLOW            EQU CLOCK_SPEED / 1
MEDIUM          EQU CLOCK_SPEED / 4
FAST            EQU CLOCK_SPEED / 8
WAVE_SIZE       EQU 14


    AREA    wave, DATA, READONLY, ALIGN=2
; A 3-bit digital wave.
; Numbers are out of order to show the wave according to the physical locations
; of the port F LEDs instead of the order used for pin addresses.
; (Location: Red, Green, Blue. Pins: Red=1, Blue=2, Green=4.)
WAVE            DCB 0,2,4,6,1,3,5,7,5,3,1,6,4,2





    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB


start
        ; Set initial state
        LDR r4, =~0                             ; r4 = enabled / disabled state (for systick)
        LDR r5, =SLOW                           ; r5 = speed of interrupts
        LDR r11, =0                             ; register for the WAVE index

        ; Initialize port F
        LDR r0, ='F'                            ; load the ASCII value of the port's letter
        LDR r1, =0x0E                           ; pins 1, 2, and 3 are outputs
        BL  portInit                            ; call initialization routine
        MOV r6, r0                              ; copy port base address to higher register (>=r4)
        
        MOV r0, r5                              ; load period for passing
        BL  systickInit                         ; call initialization for systick
        MOV r0, r4                              ; load enabled/disabled for passing
        BL  systickSetEnabled                   ; call enable/disable function
          
        ; Initial button states (port F built-in)
        LDR r7, =0                              ; button 1 previous
        LDR r8, =0                              ; button 2 previous
        LDR r9, =0                              ; button 1 current
        LDR r10, =0                             ; button 2 current
loop
        ; save the previous state of the buttons (previous loop iteration)
        MOV r7, r9                              ; button 1 prev <-- button 1
        MOV r8, r10                             ; button 2 prev <-- button 2

        ; Read buttons' state
        MOV r0, r6                              ; copy port base address for passing
        LDR r1, =4                              ; reading button 1 (PF4)
        BL  readPin                             ; call routine
        MOV r9, r2                              ; copy the value so it's not over-written
        LDR r1, =0                              ; reading button 2 (PF0)
        BL  readPin                             ; call routine
        MOV r10, r2                             ; copy the value so it's not over-written

        ; If the button is pressed, but wasn't pressed
        ; during the previous loop iteration, toggle systick.
        CMP r9, r7                              ; check if button 1 changed
        BLNE button1_changed                    ; if so, call routine
        CMP r10, r8                             ; check if button 2 changed
        BLNE button2_changed                    ; if so, call routine

        CMP r4, #0                              ; disabled?
        WFINE                                   ; if not, wait for interrupt
        B   loop                                ; restart loop

button1_changed      
        CMP r9, #0                              ; is the button pressed?
        BXNE lr                                 ; if not, return to caller
        ; button 1 has just been pressed,
        ; toggle systick being enabled/disabled.
        PUSH {lr}                               ; save caller's address to the stack
        MVN r4, r4                              ; flip all bits in the enabled state 
        MOV r0, r4                              ; copy enabled value for passing
        BL  systickSetEnabled                   ; call enable/disable function
        ; turn off all LEDs if disabled
        CMP r4, #0                              ; is systick disabled?
        MOVEQ r0, r6                            ; if so, copy base address for passing
        LDREQ r1, =0                            ; also, all pins will be 0        
        BLEQ  setPins                           ; also, call function to set pins
        POP {lr}                                ; load the caller's address from the stack
        BX lr                                   ; return to caller
      
button2_changed      
        CMP r10, #0                             ; is the button pressed?
        BXNE lr                                 ; if not, return to caller
        ; button 2 has just been pressed,
        ; change the speed setting.
        PUSH {lr}                               ; save caller's address to the stack
        LDR r0, =0                              ; mark no changes have been made
        LDR r1, =SLOW                           ; load speed value
        LDR r2, =MEDIUM                         ; load speed value
        CMP r5, r2                              ; if speed is medium
        LDREQ r5, =FAST                         ; then speed will be fast
        LDREQ r0, =1                            ; mark that a change was made
        CMP r5, r1                              ; if speed is slow
        LDREQ r5, =MEDIUM                       ; then speed will be medium
        LDREQ r0, =1                            ; mark that a change was made
        CMP r0, #0                              ; if no change has been made
        LDREQ r5, =SLOW                         ; then speed will be slow        
        MOV r0, r5                              ; copy speed for passing
        BL systickSetPeriod                     ; call function        
        POP {lr}                                ; load the caller's address from the stack
        BX lr                                   ; return to caller





; Interrupt handler (defined in startup file): Called when an interrupt occurs
SysTick_Handler
        PUSH {lr}                               ; save caller's address to the stack
        LDR r2, =WAVE                           ; load the base address of the WAVE array
        LDRB r1, [r2, r11]                      ; load a byte from the array (r11 is index)
        LSL r1, r1, #1                          ; shift value left because outpus are pins 1-3 instead of 0-2
        MOV r0, r6                              ; load port base address
        BL  setPins                             ; set port's pins for this wave element
        ADD r11, r11, #1                        ; increment the array index
        CMP r11, #WAVE_SIZE                     ; if end of array is reached
        LDREQ r11, =0                           ; then start over at 0
        POP {lr}                                ; load the caller's address from the stack
        BX lr                                   ; return from interrupt
        

    ALIGN                                       ; make sure the end of this section is aligned (even memory address)
    END                                         ; end of file
