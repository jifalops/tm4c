#include "header.h"
#include "music_waves.h"
#include "music_songs.h"
#include "music_player.h"

Port io;
int playing = FALSE;
int beatCount = 0;
int noteCount = 0;

int main(void) {    
    int playPauseButton, playPauseButtonPrev, stopButton, stopButtonPrev;
    
    
    // Music output port. The DAC MUST use pins 0-3 on the port!
    Port dac = portInit('YOUR PORT LETTER HERE', 0x00, 0x0F);
    
    // port used for the play/pause and stop buttons
    // and the LED outputs (one for tempo/beat, one to show a note is playing)
    io = portInit(/* YOUR PORT INFO HERE*/);
    
    musicInit(dac);
    musicLoad(/* YOUR SONG INFO HERE */);
    
	// Main loop
    while(TRUE) {
        // Read inputs and call one of:
        // musicPlay()
        // musicPause()
        // musicStop()
        
        if (playing == TRUE) {
            WaitForInterrupt();
        }
    }
}

/** Light the LED for 11/12ths of the note's duration */
void onTwelfthNote(void) { 
    if (noteCount == 0) {
        setMuted(0);
        set(/* YOUR LED PIN HERE */);
    } else if (noteCount == 11) {
        setMuted(1);
        clear(/* SAME LED PIN */);
    }    
    noteCount = (noteCount + 1) % 12;
}

/** Light the LED for 2/12ths of the beat's duration */
void onTwelfthBeat(void) {  
    if (beatCount == 0) {
        set(/* YOUR LED PIN HERE */);        
    } else if (beatCount == 2) {
        clear(/* SAME LED PIN */);
    }
    beatCount = (beatCount + 1) % 12;
}

void onMusicStop(void) {
    // turn off LEDs
    setPins(io, 0);
    playing = FALSE;
    noteCount = 0;
    beatCount = 0;
}
