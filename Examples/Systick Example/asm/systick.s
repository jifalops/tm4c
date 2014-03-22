; Use the SysTick timer to request interrupts at a particular period.

    ; Allow other files to call these labels
    ;
    ; IMPORTANT: SysTick_Handler (defined in the startup file) is called by the system 
    ;            whenever an interrupt from Systick occurs. To implement it, simply 
    ;            create a label with the name "SysTick_Handler" (case-sensitive), and
    ;            add the line "EXPORT SysTick_Handler" near the top of the file. 
    EXPORT systickInit          ; initialize systick            in:  r0=period
                                ;                               out: none
                                ;                               mod: r0-r3                                    
    EXPORT systickSetEnabled    ; enable/disable systick        in:  r0=zero-or-nonzero
                                ;                               out: none
                                ;                               mod: r1-r3    
    EXPORT systickSetPeriod     ; set systick reload value      in:  r0=period
                                ;                               out: none
                                ;                               mod: r1  

; Copied from address file
NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018
NVIC_ST_CTRL_COUNT    EQU 0x00010000  ; Count flag
NVIC_ST_CTRL_CLK_SRC  EQU 0x00000004  ; Clock Source
NVIC_ST_CTRL_INTEN    EQU 0x00000002  ; Interrupt enable
NVIC_ST_CTRL_ENABLE   EQU 0x00000001  ; Counter mode
NVIC_ST_RELOAD_M      EQU 0x00FFFFFF  ; Counter load value
NVIC_SYS_PRI3_R       EQU 0xE000ED20  ; Sys. Handlers 12 to 15 Priority



    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

; **************SysTick_Init*********************
; Initialize SysTick periodic interrupts, priority 2
; Input: R0  interrupt period
;        Units of period are 1/clockfreq
;        Maximum is 2^24-1
;        Minimum is determined by length of ISR
; Output: none
; Modifies: R0, R1, R2, R3
systickInit
        ; start critical section
        MRS    R3, PRIMASK              ; save old status
        CPSID  I                        ; mask all (except faults)
        ; disable SysTick during setup
        LDR R1, =NVIC_ST_CTRL_R         ; R1 = &NVIC_ST_CTRL_R (pointer)
        MOV R2, #0
        STR R2, [R1]                    ; disable SysTick
        ; maximum reload value
        LDR R1, =NVIC_ST_RELOAD_R       ; R1 = &NVIC_ST_RELOAD_R (pointer)
        ;SUB R0, R0, #1                  ; counts down from RELOAD to 0
        STR R0, [R1]                    ; establish interrupt period
        ; any write to current clears it
        LDR R1, =NVIC_ST_CURRENT_R      ; R1 = &NVIC_ST_CURRENT_R (pointer)
        STR R2, [R1]                    ; writing to counter clears it
        ; set NVIC system interrupt 15 to priority 2
        LDR R1, =NVIC_SYS_PRI3_R        ; R1 = &NVIC_SYS_PRI3_R (pointer)
        LDR R2, [R1]                    ; friendly access
        AND R2, R2, #0x00FFFFFF         ; R2 = R2&0x00FFFFFF (clear interrupt 15 priority)
        ORR R2, R2, #0x40000000         ; R2 = R2|0x40000000 (interrupt 15 priority is in bits 31-29)
        STR R2, [R1]                    ; set SysTick to priority 2
        ; enable SysTick with core clock
        LDR R1, =NVIC_ST_CTRL_R         ; R1 = &NVIC_ST_CTRL_R
        ; ENABLE SysTick (bit 0), INTEN enable interrupts (bit 1), and CLK_SRC (bit 2) is internal
        MOV R2, #(NVIC_ST_CTRL_INTEN+NVIC_ST_CTRL_CLK_SRC)
        STR R2, [R1]                    ; store a 7 to NVIC_ST_CTRL_R
        ; end critical section
        MSR    PRIMASK, R3              ; restore old status
        
        CPSIE  I                        ; enable interrupts
        
        BX  LR                          ; return


; Enable or disable systick.
; inputs:   r0 - 0 to disable, any other value to enable
; outputs:  none
; modifies: r1-r3
systickSetEnabled
        LDR r1, =NVIC_ST_CTRL_R                 ; load the systick control register's address
        LDR r2, [r1]                            ; load its value
        LDR r3, =NVIC_ST_CTRL_ENABLE            ; load the value to enable systick
        CMP r0, #0                              ; if disabling...
        BICEQ r2, r2, r3                        ; true:  r2 &= ~r3 (clear enable bit)
        ORRNE r2, r2, r3                        ; false: r2 |= r3  (set enable bit)
        STR r2, [r1]                            ; save new setting
        BX lr                                   ; return to caller
        

; Set Systick's reload value ("period")
; inputs:   r0 - new reload value
; outputs:  none
; modifies: r1
systickSetPeriod
        LDR r1, =NVIC_ST_RELOAD_R               ; load Systick's reload register's address
        STR r0, [r1]                            ; store the new value
        LDR r1, =NVIC_ST_CURRENT_R              ; load Systick's current-value register's address
        LDR r0, =0                              ; load 0
        STR r0, [r1]                            ; restart current count
        BX  lr                                  ; return to caller


    ALIGN                                       ; make sure the end of this section is aligned
    END                                         ; end of file
