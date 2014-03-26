// Only allow this file to be included ONCE by other files.
#ifndef HEADER_TONE_INCLUDED
#define HEADER_TONE_INCLUDED

typedef int Tone;

static const int FREQ_MULTIPLIER = 1000;
// A list of all 88 piano key frequencies * 10,000, most have multiple names.
// An 's' denotes sharp; 'b' denotes flat.
static const Tone 	
	REST = FREQ_MULTIPLIER,
	// ### Octave 0 ###
	A0  = 27500,
	As0 = 29135,   Bb0 = As0,
    B0  = 30867,   Cb1 = B0,
	// ### Octave 1 ###
    C1  = 32703,   Bs0 = C1,   
	Cs1 = 34647,   Db1 = Cs1,
   	D1  = 36708,
	Ds1 = 38890,   Eb1 = Ds1,    
	E1  = 41203,   Fb1 = E1,	
    F1  = 43653,   Es1 = F1,
	Fs1 = 46249,   Gb1 = Fs1,   
	G1  = 48999,
	Gs1 = 51913,   Ab1 = Gs1,    
	A1  = 55000,
	As1 = 58270,   Bb1 = As1,    
	B1  = 61735,   Cb2 = B1,    	
	// ### Octave 2 ###
    C2  = 65406,   Bs1 = C2,	
	Cs2 = 69295,   Db2 = Cs2,
	D2  = 73416,
	Ds2 = 77781,   Eb2 = Ds2,
	E2  = 82406,   Fb2 = E2,
    F2  = 87307,   Es2 = F2,
	Fs2 = 92498,   Gb2 = Fs2,
	G2  = 97998,
	Gs2 = 103826,  Ab2 = Gs2,
	A2  = 110000,
	As2 = 116541,  Bb2 = As2,
	B2  = 123471,  Cb3 = B2,
	// ### Octave 3 ###
    C3  = 130813,  Bs2 = C3,
    Cs3 = 138591,  Db3 = Cs3,
	D3  = 146832,
	Ds3 = 155563,  Eb3 = Ds3,
	E3  = 164814,  Fb3 = E3,
    F3  = 174614,  Es3 = F3,
	Fs3 = 184997,  Gb3 = Fs3,    
	G3  = 195998,
	Gs3 = 207652,  Ab3 = Gs3,
	A3  = 220000,
	As3 = 233082,  Bb3 = As3,
	B3  = 246942,  Cb4 = B3,
	// ### Octave 4 ###
    C4  = 261626,  Bs3 = C4,	
	Cs4 = 277183,  Db4 = Cs4,
	D4  = 293665,
	Ds4 = 311127,  Eb4 = Ds4,
	E4  = 329628,  Fb4 = E4,
    F4  = 349228,  Es4 = F4,
	Fs4 = 369994,  Gb4 = Fs4,
	G4  = 391995,
	Gs4 = 415305,  Ab4 = Gs4,
	A4  = 440000,
	As4 = 466164,  Bb4 = As4,
	B4  = 493883,  Cb5 = B4,   	 
	// ### Octave 5 ###
	C5  = 523251,  Bs4 = C5, 
	Cs5 = 554365,  Db5 = Cs5,    
	D5  = 587330,
	Ds5 = 622254,  Eb5 = Ds5,    
	E5  = 659255,  Fb5 = E5,    	
    F5  = 698456,  Es5 = F5,
	Fs5 = 739989,  Gb5 = Fs5,
	G5  = 783991,
	Gs5 = 830609,  Ab5 = Gs5,    
	A5  = 880000,
	As5 = 932328,  Bb5 = As5,    
	B5  = 987767,  Cb6 = B5,	  
	// ### Octave 6 ###
	C6  = 1046500, Bs5 = C6,
	Cs6 = 1108730, Db6 = Cs6,
	D6  = 1174660,
	Ds6 = 1244510, Eb6 = Ds6,    
	E6  = 1318510, Fb6 = E6,	
    F6  = 1396910, Es6 = F6,
	Fs6 = 1479980, Gb6 = Fs6,   
	G6  = 1567980,
	Gs6 = 1661220, Ab6 = Gs6,    
	A6  = 1760000,
	As6 = 1864660, Bb6 = As6, 
	B6  = 1975530, Cb7 = B6,     
	// ### Octave 7 ###
	C7  = 2093000, Bs6 = C7,
	Cs7 = 2217460, Db7 = Cs7,    
	D7  = 2349320,
	Ds7 = 2489020, Eb7 = Ds7,   
	E7  = 2637020, Fb7 = E7,    	
    F7  = 2793830, Es7 = F7,
	Fs7 = 2959960, Gb7 = Fs7,    
	G7  = 3135960,
	Gs7 = 3322440, Ab7 = Gs7,    
	A7  = 3520000,
	As7 = 3729310, Bb7 = As7,    
	B7  = 3951070, Cb8 = B7,	  
	// ### Octave 8 ###
	C8  = 4186010, Bs7 = C8;
	
	
	
	
// Frequencies for the 88 keys on a piano, lowest first. (currently unused)
/*static const float PIANO_NOTE_FREQ[] = {
	
	// Octave 0
	27.5, 				// A
	29.1352,			// A#
	30.8677,			// B
	
	// Octave 1
	32.7032,			// C
	34.6478,			// C#
	36.7081,			// D
	38.8909,			// D#
	41.2034,			// E
	43.6535,			// F
	46.2493,			// F#
	48.9994,			// G
	51.9131,			// G#
	55.0,					// A
	58.2705,			// A#
	61.7354,			// B
	
	// Octave 2
	65.4064,			// C
	69.2957,			// C#
	73.4162,			// D
	77.7817,			// D#
	82.4069,			// E
	87.3071,			// F
	92.4986,			// F#
	97.9989,			// G
	103.826,			// G#
	110,					// A
	116.541,			// A#
	123.471,			// B
	
	// Octave 3
	130.813,			// C
	138.591,			// C#
	146.832,			// D
	155.563,			// D#
	164.814,			// E
	174.614,			// F
	184.997,			// F#
	195.998,			// G
	207.652,			// G#
	220,					// A
	233.082,			// A#
	246.942,			// B
	
	// Octave 4
	261.626,			// C
	277.183,			// C#
	293.665,			// D
	311.127,			// D#
	329.628,			// E
	349.228,			// F
	369.994,			// F#
	391.995,			// G
	415.305,			// G#
	440,					// A
	466.164,			// A#
	493.883,			// B
	
	// Octave 5
	523.251,			// C
	554.365,			// C#
	587.330,			// D
	622.254,			// D#
	659.255,			// E
	698.456,			// F
	739.989,			// F#
	783.991,			// G
	830.609,			// G#
	880,					// A
	932.328,			// A#
	987.767,			// B
	
	// Octave 6
	1046.50,			// C
	1108.73,			// C#
	1174.66,			// D
	1244.51,			// D#
	1318.51,			// E
	1396.91,			// F
	1479.98,			// F#
	1567.98,			// G
	1661.22,			// G#
	1760,					// A
	1864.66,			// A#
	1975.53,			// B
	
	// Octave 7
	2093.00,			// C
	2217.46,			// C#
	2349.32,			// D
	2489.02,			// D#
	2637.02,			// E
	2793.83,			// F
	2959.96,			// F#
	3135.96,			// G
	3322.44,			// G#
	3520,					// A
	3729.31,			// A#
	3951.07,			// B
	
	// Octave 8
	4186.01				// C
};*/

/*
static const double TWELFTH_ROOT_OF_2 		= 1.0594630943592952645618252949463;		// Find next tone higher
static const double TWELFTH_ROOT_OF_2_INV = 0.94387431268169349664191315666753;		// Reciprical (1/TWELFTH_ROOT_OF_2) to find next tone lower.

// Private function prototypes.
static short calc_distFromMiddleA(tone_t tone);
static float calc_freq(short distFromMiddleA);
static unsigned int calc_delay(float freq);

// Function for making a new tone, instead of using the predefined tones below.
tone_t make_tone(unsigned short base, short adj, unsigned short octave) {
	short dist;
	tone_t tone;
	tone.base = base;
	tone.adj = adj;
	tone.octave = octave;
	dist = calc_distFromMiddleA(tone);
	tone.freq = calc_freq(dist);
	tone.delay = calc_delay(tone.freq);
	return tone;
}

// Number of half-tones from Middle A.
static short calc_distFromMiddleA(tone_t tone) {
	short dist;
	switch (tone.base) {
		case 0: dist = 0;
		case 1: dist = 2;
		case 2: dist = 3;
		case 3: dist = 5;
		case 4: dist = 7;
		case 5: dist = 8;
		case 6: dist = 10;
	}
	dist += tone.adj;	
	dist += 12 * (tone.octave - 4);	
	return dist;
}

// Recursively calculate frequency, once for each half-tone.
static float calc_freq(short distFromMiddleA) {		
	static float freq = 440;
	if (distFromMiddleA > 0) {
		freq *= TWELFTH_ROOT_OF_2;
		distFromMiddleA--;
	}
	else if (distFromMiddleA < 0) {
		freq *= TWELFTH_ROOT_OF_2_INV;
		distFromMiddleA++;
	}
	
	if (distFromMiddleA == 0) {
		return freq;
	}
	else {
		return calc_freq(distFromMiddleA);
	}
}

// Systick reload value to achieve desired frequency.
static unsigned int calc_delay(float freq) {
	return MAX_FREQ / freq;
}
*/


// public function prototypes
//tone_t make_tone(unsigned short base, short adj, unsigned short octave);

#endif //HEADER_TONE_INCLUDED
