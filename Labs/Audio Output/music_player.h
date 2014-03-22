// Only allow this file to be included ONCE by other files.
#ifndef HEADER_MUSIC_PLAYER_INCLUDED
#define HEADER_MUSIC_PLAYER_INCLUDED

#include "tm4c.h"
#include "pll.h"
#include "systick.h"
#include "song.h"

static const int 
    PLL_DIVIDER = 4,
    CLOCK_SPEED = 80000000,
    DAC_BITS = 4,    
    SAMPLES_PER_SINE_WAVE = 2 * (2 ^ DAC_BITS),
    MAX_FREQ = CLOCK_SPEED / SAMPLES_PER_SINE_WAVE;


typedef struct {
	int time;				// current position time in micro seconds
	int noteIndex;			// which note of the song we are at
	int waveIndex;			// which point of the sin-wave we are at.
	int noteInterrupts; 	// number of wave-points traveled (duration held so far)
    int noteInterruptLimit; // the number of interrupts before moving on to the next note
    int noteInterruptsPerBeat; // number of interrupts for a beat on this note
    int beatInterrupts;     // number of interrupts passed for the current beat
} SongPosition;

static const SongPosition initialPosition = {0,0,0,0,0,0,0};


typedef struct {
	Song song;
	int *wave; 		        // digital wave for an instrument.
	int waveSize;
	SongPosition position;	
} PlayingSong;


Port musicPort;
PlayingSong playingSong;



void onPlayNote(Note note);
void onBeat(void);


void musicInit(char port, int inputs, int outputs) {
    musicPort = portInit(port, inputs, outputs);
    pllInit(PLL_DIVIDER);
    systickInit(0);
}

Note getCurrentNote() {
    return playingSong.song.notes[playingSong.position.noteIndex];
}

/** Calculate the systick period for interrupts at the note's frequency. */
int noteToSystickPeriod(Note note) {   
    return MAX_FREQ / (note.freq / FREQ_MULTIPLIER);    
}

void playNote(Note note) {
    systickSetPeriod(noteToSystickPeriod(note));
    onPlayNote(note);
}

void musicPlay() {        
    systickSetEnabled(1);
    playNote(getCurrentNote());
}

void musicPause() {
    systickSetEnabled(0);
}

void musicStop() {
    systickSetEnabled(0);
    setPins(musicPort, 0);
    playingSong.position = initialPosition;
}

/** Calculate the duration of a note in milliseconds. */
int noteDurationInMilliseconds(Note note) {
    // beats per minute
    int tempo = playingSong.song.tempo;
    // the duration of a beat (relative to a WHOLE_NOTE)
    int beatDuration = playingSong.song.timeSignature.beatDuration;    
    
    return note.duration / beatDuration * tempo * 1000 / 60;
}

/** The number of interrupts per second for a note. */
int interruptsPerSec(Note note) {
    return SAMPLES_PER_SINE_WAVE * note.freq / FREQ_MULTIPLIER;
}

/** Calculate the number of systick interrupts for the note to be held its entire duration */
int noteToInterruptCount(Note note) {
    return interruptsPerSec(note) * noteDurationInMilliseconds(note) / 1000;
}



/** Calculates the number of interrupts in a beat for a given note. */
int noteInterruptsPerBeat(Note note) {
    return interruptsPerSec(note) / (playingSong.song.tempo / 60);
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
        interruptsPerBeat = playingSong.position.noteInterruptsPerBeat;
        playingSong.position.noteInterruptsPerBeat = noteInterruptsPerBeat(getCurrentNote());
        playingSong.position.beatInterrupts = playingSong.position.beatInterrupts *  \
                (playingSong.position.noteInterruptsPerBeat / interruptsPerBeat);
        
        //musicPlay();
    }
}

void incNoteInterrupts() {
    ++playingSong.position.noteInterrupts;    
    if (playingSong.position.noteInterrupts >= playingSong.position.noteInterruptLimit) {
        incCurrentNote();
    }    
}

void incBeatInterrupts() {
    ++playingSong.position.beatInterrupts;
    if (playingSong.position.beatInterrupts >= playingSong.position.noteInterruptsPerBeat) {
        playingSong.position.beatInterrupts = 0;
        onBeat();        
    }
}

void incCurrentWavePoint() {
    playingSong.position.waveIndex = (playingSong.position.waveIndex + 1) % playingSong.waveSize;
    incBeatInterrupts();
    // Increment the number of points played for the current note.
	incNoteInterrupts();    
}




void musicLoad(Song song, int wave[], int waveSize) {
    musicStop();
    playingSong.song = song;
    playingSong.wave = wave;
    playingSong.waveSize = waveSize;	
//	music->song = song;
//	music->wave = wave;
//	music->wave_size = wave_size;
//	music->pos = malloc(sizeof(position_t));
//	music->pos->note_index = 0;
//	music->pos->note_interrupts = 0;
//	music->pos->time = 0;
//	music->pos->wave_index = 0;
//	music->total_time = calc_time(song);    
}


void systickInterruptHandler() {	
    // Play next point in wave
    // TODO: this only works if outputs are pins 0-3!
    setPins(musicPort, getCurrentWavePoint());
    incCurrentWavePoint();

	
}

#endif // HEADER_MUSIC_PLAYER_INCLUDED
