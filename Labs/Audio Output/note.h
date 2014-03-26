// Only allow this file to be included ONCE by other files.
#ifndef HEADER_NOTE_INCLUDED
#define HEADER_NOTE_INCLUDED

#include "tone.h"

typedef int NoteDuration;

typedef struct {
	Tone freq;
    NoteDuration duration;	
} Note;

// Note durations.
static const NoteDuration 
    WHOLE_NOTE     = 32 * 6,            // allows all of the following to be integers
    HALF_NOTE      = WHOLE_NOTE / 2,	
    QUARTER_NOTE   = WHOLE_NOTE / 4,
    EIGHTH_NOTE    = WHOLE_NOTE / 8,
    SIXTEENTH_NOTE = WHOLE_NOTE / 16,
    THIRTYSECOND_NOTE = WHOLE_NOTE / 32,

	DOTTED_WHOLE_NOTE     = 1.5 * WHOLE_NOTE,
	DOTTED_HALF_NOTE      = 1.5 * HALF_NOTE,
	DOTTED_QUARTER_NOTE   = 1.5 * QUARTER_NOTE,
	DOTTED_EIGHTH_NOTE    = 1.5 * EIGHTH_NOTE,
	DOTTED_SIXTEENTH_NOTE = 1.5 * SIXTEENTH_NOTE,
    DOTTED_THIRTYSECOND_NOTE = 1.5 * THIRTYSECOND_NOTE,

	WHOLE_TRIPLET	  = WHOLE_NOTE     / 3,
	HALF_TRIPLET      = HALF_NOTE      / 3,
	QUARTER_TRIPLET   = QUARTER_NOTE   / 3,
	EIGHTH_TRIPLET    = EIGHTH_NOTE    / 3,
	SIXTEENTH_TRIPLET = SIXTEENTH_NOTE / 3,
    THIRTYSECOND_TRIPLET = THIRTYSECOND_NOTE / 3;

#endif //HEADER_NOTE_INCLUDED
