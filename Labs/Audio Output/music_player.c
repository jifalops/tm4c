#include "header.h"
#include "pll.h"
#include "systick.h"
#include "music_player.h"


static unsigned int calc_time(const song_t *song); 
static unsigned int duration_to_millis(const unsigned int duration, const unsigned int note_to_get_beat, unsigned int tempo);

static music_t *music;

void musicInit() {
	// Initialize Port E to use bit 5 for digital output
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; 			// 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;        							// allow time for clock to start	
																							// 2) no unlocking necessary  
  GPIO_PORTE_AMSEL_R = 0x00;     							// 3) disable analog on PE7-0	
  GPIO_PORTE_PCTL_R = 0;   										// 4) PCTL GPIO on PE7-0	
  GPIO_PORTE_DIR_R = 0x20;         						// 5) PE5 are output
  GPIO_PORTE_AFSEL_R = 0;     								// 6) disable alt funct on PD7-0  
  GPIO_PORTE_DEN_R = 0x20;   									// 7) enable digital I/O on PE5
	
	
	pll_init();
	dac_init();
	systick_init(1);	
}

void music_load(const song_t *song, const unsigned int *wave, unsigned int wave_size) {	
	music_stop();
	music = malloc(sizeof(music_t));
	music->song = song;
	music->wave = wave;
	music->wave_size = wave_size;
	music->pos = malloc(sizeof(position_t));
	music->pos->note_index = 0;
	music->pos->note_interrupts = 0;
	music->pos->time = 0;
	music->pos->wave_index = 0;
	music->total_time = calc_time(song);
}

static unsigned int calc_time(const song_t *song) {
	unsigned int time = 0;
	int i;
	unsigned int size = song->notes_size;
	note_t *note;
	for (i = 0; i < size; i++) {
		note = (note_t*) song->notes + i;
		time += note->duration;		
	}	
	time = duration_to_millis(time, song->time_signature->note_to_get_beat, song->tempo);
	return time;
}

static unsigned int duration_to_millis(const unsigned int duration, const unsigned int note_to_get_beat, unsigned int tempo) {
	unsigned int millis;
	millis = 1000 * duration / note_to_get_beat;
	millis /= (tempo/60);
	return millis;
}


void music_play() {
	systick_reload((music->song->notes + music->pos->note_index)->tone->delay);
}

void music_pause() {
	
}

void music_stop() {
	free(music);
}






#define PE5       			 	(*((volatile unsigned long *) 0x40024080))		// pin 5, the LED output. 
// Interrupt service routine, Executed periodically
// (Prototype is defined in startup.s)
void SysTick_Handler(){
	unsigned int freq;
	// Play next point in wave
	dac_out(*(music->wave + music->pos->wave_index));
	music->pos->wave_index++;
	music->pos->wave_index %= music->wave_size;
	
	
	// Increment the number of points played for the current note.
	music->pos->note_interrupts++;
	
	freq = (music->song->notes + music->pos->note_index)->tone->freq / 1000;
//	if (music->pos->note_interrupts == ((music->song->notes + music->pos->note_index)->duration * music->wave_size * freq) / 2) {	
//		PE5 = 0;
//	}
	if (music->pos->note_interrupts > ((music->song->notes + music->pos->note_index)->duration * music->wave_size * freq)) {	
		PE5 ^= 0x20;
		
		music->pos->note_interrupts = 0;	
		music->pos->note_index++;
		music->pos->note_index %= music->song->notes_size;
		systick_reload((music->song->notes + music->pos->note_index)->tone->delay);
	}
	
/*
	
	if (input > 0) {
		DAC_Out(wave[data]);
		data = (data + 1) % 32;	
	}
	
	count++;
	if (count == 16 * freq) PE5 = 0; // Turn LED off
	if (count > 32 * freq) {
		PE5 = 0x20;
		input = Piano_In();
		if (input > 0) Sound_Play(note[input + 11]);
		count = 0;
		//x = (x + 1) % 49;				
	}
	*/
}
