    ; Allow other files to call these labels
    EXPORT portInit     ; initialize a port             in:  r0=port_letter(upper-case) r1=output_pins(pin-directions)
                        ;                               out: r0=port_base_address
                        ;                               mod: r0-r3
    EXPORT readPin      ; read a port's pin             in:  r0=port_base_address       r1=pin_number(0-7)
                        ;                               out: r2=pin_value
                        ;                               mod: r2,r3
    EXPORT setPin       ; turn on a port's pin          in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: none
                        ;                               mod: r2,r3
    EXPORT clearPin     ; turn off a port's pin         in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: none
                        ;                               mod: r2,r3
    EXPORT togglePin    ; toggle a port's pin           in:  r0=port_base_address       r1=pin_number(0-7)    
                        ;                               out: r2=new_pin_value
                        ;                               mod: r2,r3
    EXPORT readPins     ; read all pins on a port       in:  r0=port_base_address 
                        ;                               out: r1=pin_values
                        ;                               mod: r1
    EXPORT setPins      ; set values for port's pins    in:  r0=port_base_address       r1=pin_values
                        ;                               out: none
                        ;                               mod: r1,r2


; TM4C address divisions 
FLASH_ROM_START     EQU 0x00000000 
FLASH_ROM_SIZE      EQU 0x00040000
RAM_START           EQU 0x20000000
RAM_SIZE            EQU 0x00010000
IO_START            EQU 0x40000000
IO_SIZE             EQU 0x02000000
INTERNAL_IO_START   EQU 0xE0000000
INTERNAL_IO_SIZE    EQU 0x00041000

; IO port offsets (address file: GPIO_PORTx_DATA_BITS_R)
PORTA_OFFSET        EQU 0x04000
PORTB_OFFSET        EQU 0x05000
PORTC_OFFSET        EQU 0x06000
PORTD_OFFSET        EQU 0x07000
PORTE_OFFSET        EQU 0x24000
PORTF_OFFSET        EQU 0x25000

; IO port clock gating control offsets (write to SYSCTL_RCGC2 or SYSCTL_SCGC2 to enable port clock)
PORTA_CLOCK_CONTROL EQU 0x01
PORTB_CLOCK_CONTROL EQU 0x02
PORTC_CLOCK_CONTROL EQU 0x04
PORTD_CLOCK_CONTROL EQU 0x08
PORTE_CLOCK_CONTROL EQU 0x10
PORTF_CLOCK_CONTROL EQU 0x20

; Pin offests (not in address file)
PIN0_OFFSET         EQU 0x004
PIN1_OFFSET         EQU 0x008
PIN2_OFFSET         EQU 0x010
PIN3_OFFSET         EQU 0x020
PIN4_OFFSET         EQU 0x040
PIN5_OFFSET         EQU 0x080
PIN6_OFFSET         EQU 0x100
PIN7_OFFSET         EQU 0x200

; Other port-specific offsets (address file: GPIO_PORTx_)
DATA_R              EQU 0x3FC
DIR_R               EQU 0x400
IS_R                EQU 0x404
IBE_R               EQU 0x408
IEV_R               EQU 0x40C
IM_R                EQU 0x410
RIS_R               EQU 0x414
MIS_R               EQU 0x418
ICR_R               EQU 0x41C
AFSEL_R             EQU 0x420
DR2R_R              EQU 0x500
DR4R_R              EQU 0x504
DR8R_R              EQU 0x508
ODR_R               EQU 0x50C
PUR_R               EQU 0x510
PDR_R               EQU 0x514
SLR_R               EQU 0x518
DEN_R               EQU 0x51C
LOCK_R              EQU 0x520
CR_R                EQU 0x524
AMSEL_R             EQU 0x528
PCTL_R              EQU 0x52C
ADCCTL_R            EQU 0x530
DMACTL_R            EQU 0x534

; Copied from address file
SYSCTL_RCGC2_R      EQU 0x400FE108
GPIO_LOCK_KEY       EQU 0x4C4F434B  ; Unlocks the GPIO_CR register




    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB  
    
    
    
    
; Read the value of a port's pin.
; inputs:   r0 - port's base address
;           r1 - pin number (0-7)
; outputs:  r2 - value of that pin (0 or 2 ^ pin_number)
;           r3 - pin's offset (from port base address)
; modifies: r2,r3
readPin
        LDR r3, =PIN0_OFFSET                    ; load pin 0's offset (from the port base address)
        LSL r3, r3, r1                          ; left shift to the specified pin's offset        
        LDR r2, [r0, r3]                        ; load the correct pin's value (base address + offset)
        BX  lr




; Set a specified I/O pin (to a logical 'on').
; inputs:   r0 - port's base address
;           r1 - pin number (0-7)
; outputs:  r2 - value to set the pin
;           r3 - pin's offset (from port base address)
; modifies: r2,r3
setPin
        LDR r2, =0x01                           ; load the value to set pin 0
        LSL r2, r2, r1                          ; left shift for the correct pin's set-value        
        LDR r3, =PIN0_OFFSET                    ; load pin 0's offset (from the port base address)        
        LSL r3, r3, r1                          ; left shift to the specified pin's offset        
        STR r2, [r0, r3]                        ; set the specified pin
        BX  lr




; Clear a specified I/O pin (to a logical 'off').
; inputs:   r0 - port's base address
;           r1 - pin number (0-7)
; outputs:  r2 - pin's offset
;           r3 - 0
; modifies: r2,r3
clearPin
        LDR r2, =PIN0_OFFSET                    ; load pin 0's offset (from the port base address)
        LSL r2, r2, r1                          ; left shift to the specified pin's offset
        LDR r3, =0                              ; load the value for 'off'
        STR r3, [r0, r2]                        ; clear the specified pin
        BX  lr
 
 
 
    
; Toggle a specified I/O pin (off to on, or on to off).
; inputs:   r0 - port's base address
;           r1 - pin number (0-7)
; outputs:  r2 - pin's new value
;           r3 - pin's offset (from port base address)
; modifies: r2,r3
togglePin
        LDR r2, =0x01                           ; load the value to set pin 0
        LSL r2, r2, r1                          ; left shift for the correct pin's set-value                
        LDR r3, =PIN0_OFFSET                    ; load pin 0's offset (from the port base address)        
        LSL r3, r3, r1                          ; left shift to the specified pin's offset        
        
        PUSH {r1}                               ; save pin number to stack
        LDR r1, [r0, r3]                        ; load value for this pin
        EOR r2, r1, r2                          ; toggle the pin's value        
        POP {r1}                                ; load pin number from stack
        STR r2, [r0, r3]                        ; store the pin's new value
        BX  lr
        
        
        

; Read all pins (0-7) for a port.
; inputs:   r0 - port's base address
; outputs:  r1 - value of the port's pins
; modifies: r1
readPins
        LDR r1, =DATA_R                         ; load port's data address offset (from the port base address)        
        LDR r1, [r0, r1]                        ; load port's data value (base address + offset)
        BX  lr




; Set the value of all pins (0-7) for a port.
; inputs:   r0 - port's base address
;           r1 - pin values
; outputs:  r2 - data register offset (from port base address)
; modifies: r1,r2
setPins
        LDR r2, =DATA_R                         ; load port's data address offset (from the port base address)        
        STR r1, [r0, r2]                        ; store pin values
        BX  lr
        



; Initialize a port.
; inputs:   r0 - port's letter (upper-case)
;           r1 - port's output pins (pin directions)
; outputs:  r0 - port's base address
; modifies: r0-r3
portInit  
        ; r0,r1 used
        PUSH {lr}                               ; save caller's address to stack
        BL  portInit_prep                       ; call preparation routine
        ; r2,r3 used    
        BL  portInit_activate                   ; call clock activation routine
        ; r3 freed      
        CMP r0, #'F'                            ; if this is port F...        
        MOV r0, r2                              ; copy port's base address
        ; r2 freed
        BLEQ portInit_unlock                    ; unlock its pins (PF0 / button 2)
        BLEQ portInit_pullUpF                   ; use pull-up resistors for the built-in buttons
        
        
        ; Disable analog functionality
        LDR r2, =AMSEL_R                        ; load offset of analog register
        LDR r3, =0x00         	                ; value for analog off (all pins)
        STR r3, [r0, r2]                        ; store analog settings
        
        ; Configure as GPIO
        LDR r2, =PCTL_R                         ; load offset of port control
        LDR r3, =0x00                           ; 0 = GPIO (for all pins)
        STR r3, [r0, r2]                        ; store port control settings
        
        ; Set pin directions (I/O)
        LDR r2, =DIR_R                          ; load offset of direction register        
        STR r1, [r0, r2]                        ; store pin directions (0 = input, 1 = output)
        
        ; r1 freed
        
        ; Disable Alternate functions
        LDR r2, =AFSEL_R                        ; load offset of alternate function register
        LDR r1, =0x00                           ; 0 = disable alt. fun. (for all pins)
        STR r1, [r0, r2]                        ; store alt. fun. settings

        ; Enable digital functionality
        LDR r2, =DEN_R                          ; load offset of digital enable register
        LDR r1, =0xFF         	                ; enable digital I/O (for all pins)
        STR r1, [r0, r2]                        ; store digital settings

        POP {lr}                                ; load return address from stack
        BX  lr                                  ; return to caller
        
        
; Load the port base address and clock enable value to registers.
; inputs:   r0 - port letter (ASCII value)
; outputs:  r2 - port base address
;           r3 - port clock enable bit
; modifies: r2, r3
portInit_prep                
        CMP r0, #'A'                            
        BEQ portInit_prep_A
        CMP r0, #'B'
        BEQ portInit_prep_B
        CMP r0, #'C'
        BEQ portInit_prep_C
        CMP r0, #'D'
        BEQ portInit_prep_D
        CMP r0, #'E'
        BEQ portInit_prep_E
        CMP r0, #'F'
        BEQ portInit_prep_F
        BX  lr                
portInit_prep_A
        LDR r2, =(IO_START + PORTA_OFFSET)
        LDR r3, =PORTA_CLOCK_CONTROL
        BX  lr
portInit_prep_B
        LDR r2, =(IO_START + PORTB_OFFSET)
        LDR r3, =PORTB_CLOCK_CONTROL
        BX  lr        
portInit_prep_C
        LDR r2, =(IO_START + PORTC_OFFSET)
        LDR r3, =PORTC_CLOCK_CONTROL
        BX  lr      
portInit_prep_D
        LDR r2, =(IO_START + PORTD_OFFSET)
        LDR r3, =PORTD_CLOCK_CONTROL
        BX  lr       
portInit_prep_E
        LDR r2, =(IO_START + PORTE_OFFSET)
        LDR r3, =PORTE_CLOCK_CONTROL
        BX  lr    
portInit_prep_F
        LDR r2, =(IO_START + PORTF_OFFSET)
        LDR r3, =PORTF_CLOCK_CONTROL
        BX  lr        
        

; Enable the clock signal for this port.
; inputs:   r3 - port clock control bit
; outputs:  none
; modifies: none
portInit_activate            
        PUSH {r0,r1}                            ; save registers to stack
        LDR r0, =SYSCTL_RCGC2_R                 ; load the clock control address
        LDR r1, [r0]                            ; load the clock control value
        ORR r1, r1, r3                          ; enable this port's clock bit
        STR r1, [r0]                            ; store the new clock control value (turn on the necessary clock)
        NOP								        ; allow time for clock to finish      
        POP {r0,r1}                             ; load registers from stack
        BX  lr                                  
        
        
; Unlock the lock register (port F pin 0 (built-in button 2))
; inputs:   r0 - port base address
; outputs:  none
; modifies: r2,r3
portInit_unlock        
        LDR r2, =LOCK_R                         ; load offset of lock register
        LDR r3, =GPIO_LOCK_KEY                  ; load the lock key
        STR r3, [r0, r2]                        ; store the key into the register  
        LDR r2, =CR_R                           ; load offset of commit register
        LDR r3, =0xFF          	                ; load value to unlock all pins (only PF0 is locked)
        STR r3, [r0, r2]                        ; store value into commit register
        BX  lr                                 
        
        
; Enable pull-up resistors on PF0 and PF4 (built-in buttons)
; inputs:   r0 - port base address
; outputs:  none
; modifies: r2,r3
portInit_pullUpF
        LDR r2, =PUR_R           	            ; load offset of pull up register
        LDR r3, =0x11                           ; load value for weak pull-up on pins 0 and 4
        STR r3, [r0, r2]                        ; store the value
        BX  lr                               




    ALIGN
    END
    