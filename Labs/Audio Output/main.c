#include "header.h"
#include "songs.h"
#include "waves.h"
#include "music_player.h"

Port io;
int playing = FALSE;
int beatCount = 0;
int noteCount = 0;

int main(void) {    
    int playPauseButton, playPauseButtonPrev, stopButton, stopButtonPrev;
    
    
    // Music output port. The DAC MUST use pins 0-3 on the port!
    Port dac = portInit('D', 0x00, 0x0F);
    
    // port used for the play/pause and stop buttons
    // and the LED outputs (one for tempo/beat, one to show a note is playing)
    io = portInit('A', 0x0C, 0xC0);
    
    musicInit(dac);
    musicLoad(MaryHadALittleLamb, horn64, 64);
    
    //musicPlay();
    
	// Main loop
    while(TRUE) {
        playPauseButtonPrev = playPauseButton;
        stopButtonPrev = stopButton;
        playPauseButton = read(io.p2);
        stopButton = read(io.p3);        
        if (playPauseButton != playPauseButtonPrev && playPauseButton != 0) {
            // play/pause has just changed state to pressed
            if (playing == FALSE) {                
                musicPlay();
                playing = TRUE;
            } else {
                musicPause();
                playing = FALSE;
            }
        }
        
        if (stopButton != stopButtonPrev && stopButton != 0) {
            // stop button has just been pressed
            musicStop();
            playing = FALSE;
        }
        
        if (playing == TRUE) 
            WaitForInterrupt();
    }
}


void onTwelfthNote(void) { 
    if (noteCount == 0) {
        setMuted(0);
        set(io.p7);
    } else if (noteCount == 11) {
        setMuted(1);
        clear(io.p7);
    }    
    noteCount = (noteCount + 1) % 12;
}

void onTwelfthBeat(void) {  
    if (beatCount == 0) {
        set(io.p6);        
    } else if (beatCount == 2) {
        clear(io.p6);
    }
    beatCount = (beatCount + 1) % 12;
}

void onMusicStop(void) {
    setPins(io, 0);
    playing = FALSE;
    noteCount = 0;
    beatCount = 0;
}
