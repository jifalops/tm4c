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

    


    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB


start   ; Initialization
        
        
loop    ; Main program loop





    ALIGN                                       ; make sure the end of this section is aligned (even memory address)
    END                                         ; end of file
