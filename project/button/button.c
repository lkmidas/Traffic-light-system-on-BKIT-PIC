#include "button.h"

unsigned int keyCode[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char keyOutputMask[4] = {0x10, 0x20, 0x40, 0x80};
unsigned char keyInputMask[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char buttonInputMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void KeyMatrix_init(void){
	// Initialize key matrix
	TRIS_BUTTON = 0x0f;
	PORT_BUTTON = 0xff;
}

void Button_init(void){
    // Initialize buttons
    TRIS_BUTTON = 0xff;
	PORT_BUTTON = 0xff;
}

void Button_scan(void){
	int i, j;
    for (i = 0; i < 8; i++){
        if ((PORT_BUTTON & buttonInputMask[i]) == 0){
            keyCode[i] = keyCode[i] + 1;
		} else {
            keyCode[i] = 0;
		}
    }
}

void KeyMatrix_scan(void){
	int i, j;
	for (i = 0; i < MAX_ROW; i++){
		PORT_BUTTON = ~keyOutputMask[i];
		for(j = 0; j < MAX_COL; j++){
			if ((PORT_BUTTON & keyInputMask[j]) == 0){
				keyCode[i*MAX_ROW + j] = keyCode[i*MAX_ROW + j] + 1;
			} else {
				keyCode[i*MAX_ROW + j] = 0;
			}
		}
	}
}
