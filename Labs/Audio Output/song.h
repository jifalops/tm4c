// Only allow this file to be included ONCE by other files.
#ifndef HEADER_SONG_INCLUDED
#define HEADER_SONG_INCLUDED

#include "note.h"

typedef struct {
	int beatsPerMeasure;
	int beatDuration;    // defined by note.h
} TimeSignature;	

static const TimeSignature
                  // beats per measure, note to get beat
	TWO_TWO     = {2, HALF_NOTE},
	TWO_FOUR    = {2, QUARTER_NOTE},
	THREE_FOUR  = {3, QUARTER_NOTE},
	FOUR_FOUR   = {4, QUARTER_NOTE},
	THREE_EIGHT = {3, EIGHTH_NOTE},
	FOUR_EIGHT  = {4, EIGHTH_NOTE};

typedef struct {
//	char *title;
//	char *artist;
//	const unsigned int year;	
	int tempo;          // beats per minute.
	TimeSignature timeSignature; 
	Note *notes;        // pointer to base of notes array
	int notesSize;      // size of the notes array
    //int total_time;     // playing time in milliseconds	
} Song;



#endif //HEADER_SONG_INCLUDED
