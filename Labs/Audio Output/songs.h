// Only allow this file to be included ONCE by other files.
#ifndef HEADER_SONGS_INCLUDED
#define HEADER_SONGS_INCLUDED

#include "song.h"

static Note MaryHadALittleLamb_notes[] = {
    {E4,    DOTTED_QUARTER_NOTE}, 
    {D4,    EIGHTH_NOTE}, 
    {C4,    QUARTER_NOTE},
    {D4,    QUARTER_NOTE},
	{E4,    QUARTER_NOTE},
    {E4,    QUARTER_NOTE},
    {E4,    QUARTER_NOTE}
};

static Song MaryHadALittleLamb = {
	/*.tempo =*/ 120,
	/*.time_signature =*/ {4, QUARTER_NOTE},
	/*.notes =*/ MaryHadALittleLamb_notes,
	/*.note_count =*/ sizeof(MaryHadALittleLamb_notes) / sizeof(MaryHadALittleLamb_notes[0])
};




#endif //HEADER_SONGS_INCLUDED
