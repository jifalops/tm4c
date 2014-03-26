// Only allow this file to be included ONCE by other files.
#ifndef HEADER_SONGS_INCLUDED
#define HEADER_SONGS_INCLUDED

#include "song.h"

static Note MaryHadALittleLamb_notes[] = {
    {E5,    DOTTED_QUARTER_NOTE}, 
    {D5,    EIGHTH_NOTE}, 
    {C5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {E5,    HALF_NOTE},

    {D5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {D5,    HALF_NOTE},
    
    {E5,    QUARTER_NOTE},
    {G5,    QUARTER_NOTE},
    {G5,    HALF_NOTE},

    {E5,    DOTTED_QUARTER_NOTE}, 
    {D5,    EIGHTH_NOTE}, 
    {C5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {E5,    QUARTER_NOTE},
    {D5,    QUARTER_NOTE},
    {C5,    HALF_NOTE},
    
    {REST,  EIGHTH_NOTE},
    {C5,    EIGHTH_TRIPLET},
    {E5,    EIGHTH_TRIPLET},
    {G5,    EIGHTH_TRIPLET},
    {C6,    EIGHTH_NOTE},
    {REST,  EIGHTH_NOTE},
};

static Song MaryHadALittleLamb = {
	/*.tempo =*/            120,
	/*.time_signature =*/   {4, QUARTER_NOTE},
	/*.notes =*/            MaryHadALittleLamb_notes,
	/*.note_count =*/       sizeof(MaryHadALittleLamb_notes) / sizeof(MaryHadALittleLamb_notes[0])
};




#endif //HEADER_SONGS_INCLUDED
