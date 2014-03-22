    ; Allow other files to call these labels
    EXPORT start


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

    

; Delay used between input read cycles.
; 16 MHz clock / 3 clock-cycles for the delay function = about 5333333 loops per second
; (2 pipelined instructions + 1 clock-cycle pipeline stall for branching = 3 clock-cycles for the delay function)
CLOCK_SPEED         EQU 16000000                                  
CYCLES_PER_DELAY    EQU 3
ONE_SEC             EQU CLOCK_SPEED / CYCLES_PER_DELAY

; Actual delay used in program
DELAY               EQU ONE_SEC / 5



    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

start
        ; Initialize port F
        LDR r0, ='F'                            ; load the ASCII value of the port's letter
        LDR r1, =0x0E                           ; pins 1, 2, and 3 are outputs
        BL  portInit                            ; call initialization routine
        
loop
        ; Remember port F uses negative logic on its buttons!

        ; Read state of port F's built-in buttons
        BL  readPins                            ; read state of port F's pins        
        AND r4, r1, #0x11                       ; clear values for all pins except pins 0 and 4
        
        
        ;
        ; Take action based on the buttons' state
        ;
        LDR lr, =delay                          ; set the delay function as the return address
        
        ; If only button 1 is pressed, turn on red LED
        CMP r4, #0x01                           ; compare state to only button 1 pressed
        LDREQ r1, =1                            ; load pin number if compare was equal        
        BEQ setPin                              ; set the pin if compare was equal
        
        ; If only button 2 is pressed, turn on blue LED
        CMP r4, #0x10                           ; compare state to only button 2 pressed
        LDREQ r1, =2                            ; load pin number if compare was equal
        BEQ setPin                              ; set the pin if compare was equal
        
        ; If both buttons are pressed, turn on green LED
        CMP r4, #0x00                           ; compare state to both buttons pressed
        LDREQ r1, =3                            ; load pin number if compare was equal
        BEQ setPin                              ; set the pin if compare was equal
        
        ; If no buttons are pressed, turn off all LEDs
        CMP r4, #0x11                           ; compare state to no buttons pressed
        LDREQ r1, =0                            ; load new pin values if compare was equal
        BEQ setPins                             ; set new pin values if compare was equal
        
        
	
; Delay function for debouncing and accounting for human reaction times. Delays about 3*count cycles.
; inputs:   none
; outputs:  none
; modifies: r1
delay
        LDR r1, =DELAY                          ; load delay value (see top of file)
delay_loop    
        SUBS r1, r1, #1                         ; subtract 1 and set program status flags
        BNE delay_loop                          ; if zero flag not set, restart delay loop
        B   loop                                ; return to main program loop




    ALIGN                                       ; make sure the end of this section is aligned (even memory address)
    END                                         ; end of file
