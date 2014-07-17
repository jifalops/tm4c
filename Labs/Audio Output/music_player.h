// Only allow this file to be included ONCE by other files.
#ifndef HEADER_MUSIC_PLAYER_INCLUDED
#define HEADER_MUSIC_PLAYER_INCLUDED

#include "tm4c.h"
#include "pll.h"
#include "systick.h"
#include "song.h"

// Functions called when certain events happen
// (to be implemented in another file)
void onTwelfthBeat(void);   // called 12 times per beat (for greater flexability in handling events)
void onTwelfthNote(void);   // called 12 times per note
void onMusicStop(void);     // when the end of the song is reached

// public functions intended to be called by another file
void musicInit(Port port);
void musicLoad(Song song, int wave[], int waveSize);
void musicPlay(void);
void musicPause(void);
void musicStop(void);
void setMuted(int muted);

// public functions intended for internal use within this file
Note getCurrentNote(void);
int noteToSystickPeriod(Note note);
void incCurrentNote(void);
void playNote(Note note);
int noteDurationInMilliseconds(Note note);
int interruptsPerSec(Note note);
int noteToInterruptCount(Note note);
int noteInterruptsPerBeat(Note note);
int getCurrentWavePoint(void);
void incCurrentNote(void);
void incNoteInterrupts(void);
void incBeatInterrupts(void);
void incCurrentWavePoint(void);

// Constants used for configuring music player
static const int 
    PLL_DIVIDER = 4,
    CLOCK_SPEED = 80000000,
    DAC_BITS = 4,    
    SAMPLES_PER_SINE_WAVE = 32,                     // 2 * (2 ^ DAC_BITS), // the ^ is an XOR, not exponent
    MAX_FREQ = CLOCK_SPEED / SAMPLES_PER_SINE_WAVE;

// a structure to hold information about the song's current position/status
typedef struct {
	int noteIndex;			// which note of the song we are at
	int waveIndex;			// which point of the sin-wave we are at.
	int noteInterrupts; 	// number of wave-points traveled (duration held so far)
    int noteInterruptLimit; // the number of interrupts before moving on to the next note
    int noteInterruptsPerBeat; // number of interrupts for a beat on this note
    int beatInterrupts;     // number of interrupts passed for the current beat  
    int isMuted;            // whether to send the output to the external DAC or continue silently
} SongPosition;

// the default song position
static const SongPosition initialPosition = {-1,0,0,0,0,0,0};

// structure which holds the actual song and the sample wave to use as the instrument playing it.
typedef struct {
	Song song;
	int *wave; 		        // pointer to the beginning of the array; a digital wave for an instrument.
	int waveSize;
	SongPosition position;
} PlayingSong;





// Variables accessible anywhere in this file
Port musicPort;
PlayingSong playingSong;

// setup the PLL and Systick
void musicInit(Port port) {
    musicPort = port;
    pllInit(PLL_DIVIDER);
    systickInit(0);
}

// Load a song and a digital sine wave to use (the "instrument")
void musicLoad(Song song, int wave[], int waveSize) {
    musicStop();
    playingSong.song = song;
    playingSong.wave = wave;
    playingSong.waveSize = waveSize;	
}


void musicPlay() { 
    if (playingSong.position.noteIndex == -1) {
        incCurrentNote();
        onTwelfthBeat();
        onTwelfthNote();
    }
    playNote(getCurrentNote());
    systickSetEnabled(1);    
}

void musicPause() {
    systickSetEnabled(0);
}

void musicStop() {
    systickSetEnabled(0);
    setPins(musicPort, 0);
    playingSong.position = initialPosition;
    onMusicStop();
}




// shorthand to get the current note from the notes array
Note getCurrentNote() {
    return playingSong.song.notes[playingSong.position.noteIndex];
}

/** Calculate the systick period for interrupts at the note's frequency. */
int noteToSystickPeriod(Note note) {   
    return ((unsigned int)MAX_FREQ * (unsigned int)FREQ_MULTIPLIER) / note.freq;
}

// set the Systick period to play a note (interrupt freq*32 times per second)
void playNote(Note note) {
    int period = noteToSystickPeriod(note);
    systickSetPeriod(period);    
}


/** Calculate the duration of a note in milliseconds. */
int noteDurationInMilliseconds(Note note) {    
    // beats per minute
    int tempo = playingSong.song.tempo;    
    // the duration of a beat (relative to a WHOLE_NOTE)
    int beatDuration = playingSong.song.timeSignature.beatDuration;
    
    return 1000 * note.duration * 60 / beatDuration / tempo ;
}

/** The number of interrupts per second for a note. */
int interruptsPerSec(Note note) {
    return SAMPLES_PER_SINE_WAVE * note.freq / FREQ_MULTIPLIER;
}

/** Calculate the number of systick interrupts for the note to be held its entire duration */
int noteToInterruptCount(Note note) {
    int i = interruptsPerSec(note);
    int d = noteDurationInMilliseconds(note);
    return interruptsPerSec(note) * noteDurationInMilliseconds(note) / 1000;
}



/** Calculates the number of interrupts in a beat for a given note. */
int noteInterruptsPerBeat(Note note) {
    return interruptsPerSec(note) * 60 / playingSong.song.tempo;
}


int getCurrentWavePoint() {
    return playingSong.wave[playingSong.position.waveIndex];
}

void incCurrentNote() {
    ++playingSong.position.noteIndex;
    if (playingSong.position.noteIndex >= playingSong.song.notesSize) {
        musicStop();       
    } else {        
        int interruptsPerBeat;
        playNote(getCurrentNote());
        //musicPause();        
        
        playingSong.position.noteInterruptLimit = noteToInterruptCount(getCurrentNote());
        playingSong.position.noteInterrupts = 0;
        
        // convert the number of current beat interrupts to the new interrupts per beat
        
        if (playingSong.position.noteInterruptsPerBeat > 0) {
            interruptsPerBeat = playingSong.position.noteInterruptsPerBeat;
            playingSong.position.noteInterruptsPerBeat = noteInterruptsPerBeat(getCurrentNote());
            playingSong.position.beatInterrupts = playingSong.position.beatInterrupts *  \
                    playingSong.position.noteInterruptsPerBeat / interruptsPerBeat;
        } else {
            playingSong.position.noteInterruptsPerBeat = noteInterruptsPerBeat(getCurrentNote());
        }
        //musicPlay();
    }
}

void incNoteInterrupts() {
    ++playingSong.position.noteInterrupts;    
    if (playingSong.position.noteInterrupts % (playingSong.position.noteInterruptLimit / 12) == 0) {
        onTwelfthNote();
    }
    if (playingSong.position.noteInterrupts >= playingSong.position.noteInterruptLimit) {
        incCurrentNote();
    }    
}

void incBeatInterrupts() {
    ++playingSong.position.beatInterrupts;
    if (playingSong.position.beatInterrupts % (playingSong.position.noteInterruptsPerBeat / 12) == 0) {
        onTwelfthBeat();
    }
    if (playingSong.position.beatInterrupts >= playingSong.position.noteInterruptsPerBeat) {
        playingSong.position.beatInterrupts = 0;                
    }    
}

void incCurrentWavePoint() {
    playingSong.position.waveIndex = (playingSong.position.waveIndex + 1) % playingSong.waveSize;
    incBeatInterrupts();
    // Increment the number of points played for the current note.
	incNoteInterrupts();    
}






void setMuted(int muted) {
    playingSong.position.isMuted = muted;
}

void systickInterruptHandler() {	
    // Play next point in wave
    // this only works if outputs are pins 0-3!
    
    if (playingSong.position.isMuted == 0) {
        setPins(musicPort, getCurrentWavePoint());
    }
    incCurrentWavePoint();

	
}

#endif // HEADER_MUSIC_PLAYER_INCLUDED
