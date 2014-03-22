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

    
    ; Import labels (function calls) from pll.s
    IMPORT pllInit      ; initialize the PLL            in:  r0=sysdiv2_divider
                        ;                               out: none
                        ;                               mod: r0-r3
    IMPORT pllSetDiv    ; set speed of the PLL          in:  r0=sysdiv2_divider
                        ;                               out: none
                        ;                               mod: r0-r3
    
; PLL divider limits (see pll.s for possible values)
PLL_DIV_MIN     EQU 4
PLL_DIV_MAX     EQU 127



    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB


start
        ; Initialize port F
        LDR r0, ='F'                            ; load the ASCII value of the port's letter
        LDR r1, =0x0E                           ; pins 1, 2, and 3 are outputs
        BL  portInit                            ; call initialization routine
        MOV r4, r0                              ; copy port base address to higher register (>=r4)
        
        
        ; Which LED to toggle
        LDR r5, =1                              ; initial LED to toggle (1=red)
        
        ; 24 = 16MHz bus speed (same as without using PLL)
        ; See pll.s for alternatives. 
        LDR r6, =24                             ; initial divider value
        
        MOV r0, r6                              ; copy div value for passing
        BL  pllInit                             ; initialize the PLL
        
        ; Initial button states
        LDR r7, =0                              ; button 1 previous
        LDR r8, =0                              ; button 2 previous
        LDR r9, =0                              ; button 1 current
        LDR r10, =0                             ; button 2 current
        
loop
        ; Toggle the current LED and
        ; move to the next if necessary.
        MOV r0, r4                              ; copy port base address for passing
        MOV r1, r5                              ; the pin to toggle (LED)
        BL  togglePin                           ; call routine
        CMP r2, #0                              ; if the LED was turned off
        ADDEQ r5, r5, #1                        ; then move to the next LED
        CMP r5, #4                              ; if the LED is now 4
        LDREQ r5, =1                            ; then restart at 1 (only 3 LEDs)
        
        ; save the previous state of the buttons (previous loop iteration)
        MOV r7, r9                              ; button 1 prev <-- button 1
        MOV r8, r10                             ; button 2 prev <-- button 2
        
        ; Read buttons' state
        MOV r0, r4                              ; copy port base address for passing
        LDR r1, =4                              ; reading button 1 (PF4)
        BL  readPin                             ; call routine
        MOV r9, r2                              ; copy the value so it's not over-written
        LDR r1, =0                              ; reading button 2 (PF0)
        BL  readPin                             ; call routine
        MOV r10, r2                             ; copy the value so it's not over-written

        ; If the button is pressed, but wasn't pressed
        ; during the previous loop iteration, change speed.
        CMP r9, r7                              ; check if button 1 changed
        BLNE button1_changed                    ; if so, call routine
        CMP r10, r8                             ; check if button 2 changed
        BLNE button2_changed                    ; if so, call routine
        
        B   delay                               ; call delay routine
        

button1_changed      
        CMP r9, #0                              ; is the button pressed?
        BXNE lr                                 ; if not, return to caller
        PUSH {lr}                               ; save caller's address to the stack
        BL  inc_divider                         ; increment divider
        MOV r0, r6                              ; copy divider value for passing
        BL  pllSetDiv                           ; set the new divider (changes processing speed)
        POP {lr}                                ; load the caller's address from the stack
        BX lr                                   ; return to caller
      
button2_changed      
        CMP r10, #0                             ; is the button pressed?
        BXNE lr                                 ; if not, return to caller
        PUSH {lr}                               ; save caller's address to the stack
        BL  dec_divider                         ; decrement divider
        MOV r0, r6                              ; copy divider value for passing
        BL  pllSetDiv                           ; set the new divider (changes processing speed)
        POP {lr}                                ; load the caller's address from the stack
        BX lr                                   ; return to caller


inc_divider
        ADD r6, r6, #4                          ; add 3 to divider
        CMP r6, #PLL_DIV_MAX                    ; compare to max
        LDRGT r6, =PLL_DIV_MIN                  ; if exceeded, start at min
        BX lr
   

dec_divider
        SUB r6, r6, #4                          ; subtract 3 from divider
        CMP r6, #PLL_DIV_MIN                    ; compare to min
        LDRLT r6, =PLL_DIV_MAX                  ; if exceeded, start at max
        BX lr


; Delay routine.
; inputs:   none
; outputs:  none
; modifies: r0
delay
        LDR r0, =1500000                        ; load delay value        
delay_loop    
        SUBS r0, r0, #1                         ; subtract 1 and set program status flags
        BNE delay_loop                          ; if zero flag not set, restart delay loop
        B   loop                                ; return to main program loop


    ALIGN                                       ; make sure the end of this section is aligned (even memory address)
    END                                         ; end of file
