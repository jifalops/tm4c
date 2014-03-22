#include "header.h"
#include "songs.h"
#include "waves.h"
#include "music_player.h"

Port portF;

int main(void) {
    portF = portInit('F', 0x11, 0x0E);
    
    musicInit('E', 0x00, 0x0F);
    musicLoad(MaryHadALittleLamb, piano32, 32); 
    musicPlay();
    
	// Main loop
    while(TRUE) {	
        WaitForInterrupt();
    }
}


void onPlayNote(Note note) {
//    if (read(portF.p2)) {
//        clear(portF.p2);
//        set(portF.p3);        
//    } else {
//        clear(portF.p3);
//        set(portF.p2);
//    }
    toggle(portF.p2);
}

void onBeat(void) {
    toggle(portF.p1);
}
