// Only allow this file to be included ONCE by other files.
#ifndef HEADER_TM4C_INCLUDED
#define HEADER_TM4C_INCLUDED


// TM4C address divisions 
#define FLASH_ROM_START		0x00000000 
#define FLASH_ROM_SIZE		0x00040000
#define RAM_START			0x20000000
#define RAM_SIZE			0x00010000
#define IO_START			0x40000000
#define IO_SIZE				0x02000000
#define INTERNAL_IO_START	0xE0000000
#define INTERNAL_IO_SIZE	0x00041000

// IO port offsets (address file: GPIO_PORTx_DATA_BITS_R)
#define PORTA_OFFSET		0x04000
#define PORTB_OFFSET		0x05000
#define PORTC_OFFSET		0x06000
#define PORTD_OFFSET		0x07000
#define PORTE_OFFSET		0x24000
#define PORTF_OFFSET		0x25000

// IO port clock gating control offsets (write to SYSCTL_RCGC2 or SYSCTL_SCGC2 to enable port clock)
#define PORTA_CLOCK_CONTROL	0x01
#define PORTB_CLOCK_CONTROL	0x02
#define PORTC_CLOCK_CONTROL	0x04
#define PORTD_CLOCK_CONTROL	0x08
#define PORTE_CLOCK_CONTROL	0x10
#define PORTF_CLOCK_CONTROL	0x20

// Pin offests (not in address file)
#define PIN0_OFFSET			0x004
#define PIN1_OFFSET			0x008
#define PIN2_OFFSET			0x010
#define PIN3_OFFSET			0x020
#define PIN4_OFFSET			0x040
#define PIN5_OFFSET			0x080
#define PIN6_OFFSET			0x100
#define PIN7_OFFSET			0x200

// Other port-specific offsets (address file: GPIO_PORTx_)
#define DATA_R       		0x3FC
#define DIR_R        		0x400
#define IS_R         		0x404
#define IBE_R        		0x408
#define IEV_R        		0x40C
#define IM_R         		0x410
#define RIS_R        		0x414
#define MIS_R        		0x418
#define ICR_R        		0x41C
#define AFSEL_R      		0x420
#define DR2R_R       		0x500
#define DR4R_R       		0x504
#define DR8R_R       		0x508
#define ODR_R        		0x50C
#define PUR_R        		0x510
#define PDR_R        		0x514
#define SLR_R        		0x518
#define DEN_R        		0x51C
#define LOCK_R       		0x520
#define CR_R         		0x524
#define AMSEL_R      		0x528
#define PCTL_R       		0x52C
#define ADCCTL_R     		0x530
#define DMACTL_R     		0x534

// Copied from address file
#define SYSCTL_RCGC2_R		(*((volatile unsigned long *)0x400FE108))
#define GPIO_LOCK_KEY       0x4C4F434B  // Unlocks the GPIO_CR register


#ifdef __cplusplus

    class Port {
        char letter;        	
        char inputs;
        char outputs;  
        int *baseAddress;
        char clockControlBit;    

        int *getAddress(int offset) {
            return (int*)(((char*)baseAddress) + offset);
        }
        
        int *getPinAddress(int pin) {
            return getAddress(PIN0_OFFSET << pin);
        }
        
    public:
        Port(char letter, int inputs, int outputs) {
            if (letter >= 'a' && letter <= 'f') {
                // Use ASCII value of its capital
                letter -= 32;
            }
            
            this->letter = letter;
            this->inputs = (char) inputs;
            this->outputs = (char) outputs;        
            
            baseAddress = (int*) IO_START;
                
            switch (letter) {		
                case 'A':			
                    baseAddress     += (PORTA_OFFSET / 4);
                    clockControlBit =  PORTA_CLOCK_CONTROL;
                    break;
                case 'B':
                    baseAddress     += (PORTB_OFFSET / 4);
                    clockControlBit =  PORTB_CLOCK_CONTROL;
                    break;
                case 'C':
                    baseAddress     += (PORTC_OFFSET / 4);
                    clockControlBit =  PORTC_CLOCK_CONTROL;
                    break;
                case 'D':			
                    baseAddress     += (PORTD_OFFSET / 4);
                    clockControlBit =  PORTD_CLOCK_CONTROL;
                    break;
                case 'E':
                    baseAddress     += (PORTE_OFFSET / 4);
                    clockControlBit =  PORTE_CLOCK_CONTROL; 		
                    break;
                case 'F':			
                    baseAddress     += (PORTF_OFFSET / 4);
                    clockControlBit =  PORTF_CLOCK_CONTROL;	
                    break;
            }
            
            // Enable clock
            SYSCTL_RCGC2_R |= clockControlBit; 							
            
            // Unlock the port if necessary
            if (letter == 'F') {
                *getAddress(LOCK_R) = GPIO_LOCK_KEY;							
                *getAddress(CR_R) 	= 0xFF;		
                *getAddress(PUR_R) 	= 0x11;				// built-in switches need a pull-up resistor
            }
            *getAddress(AMSEL_R)    = 0x00;    						
            *getAddress(PCTL_R) 	= 0x00;   			// use GPIO
            *getAddress(DIR_R) 		= outputs;			// 0 = input; 1 = output. Only output pins are writable
            *getAddress(AFSEL_R) 	= 0x00;     		// don't use alternate function
            *getAddress(DEN_R) 		= 0xFF;
        }

        int read(int pin) {
            return *(getPinAddress(pin));        
        }

        void set(int pin) {
            int *address = getPinAddress(pin);
            *address = (((int) address) & DATA_R) >> 2;        
        }

        void clear(int pin) {
            *(getPinAddress(pin)) = 0;
        }

        void toggle(int pin) {
            int *address = getPinAddress(pin);
            *address ^= (((int) address) & DATA_R) >> 2;
        }

        bool isSet(int pin) {
            return read(pin) != 0;
        }

        int readPins() {
            return *(getAddress(DATA_R));
        } 

        void setPins(int values) {
            *(getAddress(DATA_R)) = values;
        }
    };

#else // __cplusplus

    /*
     * Data-type definitions (Pin, AllPins, Port)
     */
     
    typedef struct {	
        int *address;
    } Pin;

    typedef struct {
        int *address;
    } AllPins;

    typedef struct {		
        char letter;
        int baseAddress;
        int clockControlBit;
        int inputs;
        int outputs;			
        Pin p0, p1, p2, p3, p4, p5, p6, p7;
        AllPins pins;	
    } Port;


    /*
     * Convenience methods for reading/writing pin(s) (read, set, clear, readPins, setPins).
     */

    int read(Pin p) {
        return *(p.address);
    }

    void set(Pin p) {
        *(p.address) = (((int)(p.address)) & DATA_R) >> 2;
    }

    void clear(Pin p) {
        *(p.address) = 0;
    }

    void toggle(Pin p) {
        *(p.address) ^= (((int)(p.address)) & DATA_R) >> 2;
    }

    int isSet(Pin p) {
        return *(p.address) != 0;
    }

    int readPins(Port p) {
        return *(p.pins.address);
    } 

    void setPins(Port p, int values) {
        *(p.pins.address) = values;
    }




    /** Helper function used during initialization. */
    int *getAddress(int base, int offset) {
        return (int *)(((char *)base) + offset);
    }


    /**
     * Initialization: Set up the port and its pins for their intended usage.
     * This must be called before any other functions in this file!
     */
    Port portInit(char letter, int inputs, int outputs) {
        Port port;
            
        if (letter >= 'a' && letter <= 'f') {
            // Use ASCII value of its capital
            letter -= 32;
        }
        
        port.letter = letter;
        port.inputs = inputs;
        port.outputs = outputs;
        
        port.baseAddress = IO_START;
            
        switch (letter) {		
            case 'A':			
                port.baseAddress     += PORTA_OFFSET;			
                port.clockControlBit =  PORTA_CLOCK_CONTROL;
                break;
            case 'B':
                port.baseAddress 	 += PORTB_OFFSET;
              port.clockControlBit   =  PORTB_CLOCK_CONTROL;
                break;
            case 'C':
                port.baseAddress 	 += PORTC_OFFSET;
                port.clockControlBit =  PORTC_CLOCK_CONTROL;
                break;
            case 'D':			
                port.baseAddress 	 += PORTD_OFFSET;
                port.clockControlBit =  PORTD_CLOCK_CONTROL;
                break;
            case 'E':
                port.baseAddress 	 += PORTE_OFFSET; 
              port.clockControlBit   =  PORTE_CLOCK_CONTROL; 		
                break;
            case 'F':			
                port.baseAddress 	 += PORTF_OFFSET;
                port.clockControlBit =  PORTF_CLOCK_CONTROL;	
                break;
        }
        
        // Enable clock
        SYSCTL_RCGC2_R |= port.clockControlBit; 							

        // Setup pin addresses while the clock settles
        port.pins.address = getAddress(port.baseAddress, DATA_R);
        port.p0.address = getAddress(port.baseAddress, PIN0_OFFSET);
        port.p1.address = getAddress(port.baseAddress, PIN1_OFFSET);
        port.p2.address = getAddress(port.baseAddress, PIN2_OFFSET);
        port.p3.address = getAddress(port.baseAddress, PIN3_OFFSET);
        port.p4.address = getAddress(port.baseAddress, PIN4_OFFSET);
        port.p5.address = getAddress(port.baseAddress, PIN5_OFFSET);
        port.p6.address = getAddress(port.baseAddress, PIN6_OFFSET);
        port.p7.address = getAddress(port.baseAddress, PIN7_OFFSET);
        
        // Unlock the port if necessary
        if (letter == 'F') {
            *getAddress(port.baseAddress, LOCK_R) 	= GPIO_LOCK_KEY;							
            *getAddress(port.baseAddress, CR_R) 	= 0xFF;		
            *getAddress(port.baseAddress, PUR_R) 	= 0x11;				// built-in switches need a pull-up resistor
        }
        *getAddress(port.baseAddress, AMSEL_R)      = 0x00;    						
        *getAddress(port.baseAddress, PCTL_R) 	    = 0x00;   			// use GPIO
        *getAddress(port.baseAddress, DIR_R) 		= outputs;			// 0 = input; 1 = output. Only output pins are writable
        *getAddress(port.baseAddress, AFSEL_R) 	    = 0x00;     		// don't use alternate function
        *getAddress(port.baseAddress, DEN_R) 		= 0xFF;
        
        return port;
    }

#endif // __cplusplus

#endif // HEADER_TM4C_INCLUDED
