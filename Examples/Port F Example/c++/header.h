// Header file for declarations common to both C and C++ implementations.

// Only allow this file to be included ONCE by other files.
#ifndef HEADER_HEADER_INCLUDED
#define HEADER_HEADER_INCLUDED

// tm4c_cpp.h declares a class, Port, with the following functions:
// C:   portInit()
// C++: [constructor] -   Initialize a port for digital I/O
//      read()        -   Read the value of a single pin
//      set()         -   Set the value of a single pin to 1
//      clear()       -   Set the value of a single pin to 0
//      toggle()      -   Toggles the value of a pin from 0 to 1, or 1 to 0.
//      isSet()       -   Reports whether the value of a pin is set or not.
//      readPins()    -   Read the value of all pins on a port
//      setPins()     -   Set the value of all pins on a port
#include "tm4c.h"


#ifdef __cplusplus
    // C++ only...
    
#else // __cplusplus
    // C only...

#endif // __cplusplus

//#define P0 0x01			// 0000 0001
//#define P1 0x02			// 0000 0010
//#define P2 0x04			// 0000 0100
//#define P3 0x08			// 0000 1000
//#define P4 0x10			// 0001 0000
//#define P5 0x20			// 0010 0000
//#define P6 0x40			// 0100 0000
//#define P7 0x80			// 1000 0000

#endif // HEADER_HEADER_INCLUDED
