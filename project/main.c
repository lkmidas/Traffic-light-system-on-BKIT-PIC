#include "main.h"

#define LED  PORTD
#define HIGH 1
#define LOW  0

#define INIT                   0
#define STANDBY                1
#define PHASE1_GREEN_AUTO      2
#define PHASE1_YELLOW_AUTO     3
#define PHASE2_GREEN_AUTO      4
#define PHASE2_YELLOW_AUTO     5
#define PHASE1_GREEN_MANUAL    6
#define PHASE1_YELLOW_MANUAL   7
#define PHASE2_GREEN_MANUAL    8
#define PHASE2_YELLOW_MANUAL   9
#define PHASE1_GREEN_SET       10
#define PHASE1_YELLOW_SET      11
#define PHASE2_GREEN_SET       12
#define PHASE2_YELLOW_SET      13

#define PHASE_1_GREEN_TIME_NORMAL   25
#define PHASE_1_YELLOW_TIME_NORMAL  3
#define PHASE_2_GREEN_TIME_NORMAL   20
#define PHASE_2_YELLOW_TIME_NORMAL  3
#define PHASE_1_GREEN_TIME_RUSH     35
#define PHASE_1_YELLOW_TIME_RUSH    3
#define PHASE_2_GREEN_TIME_RUSH     10
#define PHASE_2_YELLOW_TIME_RUSH    3

unsigned char outputMap[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char outputStatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buttonNumberMap[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

// Important functions
void setup(void);
void loop(void);
void initSystem(void);
void delay(int value);
void outputOpen(int index);
void outputClose(int index);
void outputTest(void);
void outputReverse(int index);

// State machine variables and functions
unsigned char numArray[2] = {'0', '0'};
unsigned char numIndex = 0;
unsigned char numValue;
unsigned char state = INIT;
unsigned char phase1GreenTime  = PHASE_1_GREEN_TIME_NORMAL;
unsigned char phase1YellowTime = PHASE_1_YELLOW_TIME_NORMAL;
unsigned char phase2GreenTime  = PHASE_2_GREEN_TIME_NORMAL;
unsigned char phase2YellowTime = PHASE_2_YELLOW_TIME_NORMAL;
unsigned char lightTime  = 0;
unsigned char lightCount = 0;
unsigned char standbyCount = 0;
unsigned char setBlinkCount = 0;
unsigned char setBlinkFlag = 0;
void trafficLightStateMachine(void);
void standby(void);

void phase1GreenOn(void);
void phase1GreenOff(void);
void phase1YellowOn(void);
void phase1YellowOff(void);
void phase1RedOn(void);
void phase1RedOff(void);

void phase2GreenOn(void);
void phase2GreenOff(void);
void phase2YellowOn(void);
void phase2YellowOff(void);
void phase2RedOn(void);
void phase2RedOff(void);

void phase1GreenSet(void);
void phase1YellowSet(void);
void phase2GreenSet(void);
void phase2YellowSet(void);

void phase1GreenSetRush(void);
void phase1YellowSetRush(void);
void phase2GreenSetRush(void);
void phase2YellowSetRush(void);

// State machine transitions
unsigned char isButtonNumber();
unsigned char isButtonSet();
unsigned char isButtonClearSet();
unsigned char isButtonSwitchMode();
unsigned char isButtonNext();
unsigned char isButtonStandby();
unsigned char isButtonSwap();
unsigned char isRushHour();
unsigned char isNotRushHour();

void setup(void){
    initSystem();
    LCD_printStr(0, 0, "Project:        ");
    LCD_printStr(1, 0, "TRAFFIC LIGHT   ");
    delay(2000);
}

void loop(void){
    while (!Timer3_flag){}
    Timer3_flag = 0;
    KeyMatrix_scan();
    trafficLightStateMachine();
    LCD_display();
}

void main(void){
	setup();
	while (1){
        loop();
	}
}

void delay(int value){
	int i, j;
	for (i = 0; i < value; i++)
		for (j = 0; j < 260; j++);
}

void initSystem(void){
    // Init PORTB
    TRISB = 0x00;
    // Init PORTD
    TRISD = 0x00;
    LED = 0x00;
    // Init LCD
    LCD_init();
    LCD_clearSync();
    // Init timers and interrupts
    Interrupt_init();
    delay(1000);
    Timer0_init(4695);  // 1ms timing
    Timer1_init(9390);  // 2ms timing
    Timer3_init(46950); // 10ms timing
    Timer0_set(2);
    Timer1_set(10);
    Timer3_set(50);     // cycle for all inputs, processes, outputs
    // Init key matrix
    KeyMatrix_init();
}

void outputOpen(int index){
	if (index >= 0 && index <= 7){
		LED = LED | outputMap[index];
	}
}

void outputClose(int index){
	if (index >= 0 && index <= 7){
		LED = LED & ~outputMap[index];
	}
}

void outputReverse(int index){
    if (outputStatus[index] == HIGH){
        outputClose(index);
        outputStatus[index] = LOW;
    } else {
        outputOpen(index);
        outputStatus[index] = HIGH;
    }
}

unsigned char isButtonNumber(){
    unsigned char i;
    for (i = 0; i <= 15; i++){
        if (i == 3 || i == 7 || i == 11 || i == 12 || i == 14 || i == 15){
            continue;
        }
        if (keyCode[i] == 1){
            numValue = buttonNumberMap[i];
            return 1;
        }
    }
    return 0;
}

unsigned char isButtonSet(){
    if (keyCode[12] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonClearSet(){
    if (keyCode[3] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonStandby(){
    if (keyCode[14] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonSwitchMode(){
    if (keyCode[3] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonNext(){
    if (keyCode[7] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonSwap(){
    if (keyCode[13] == 1)
        return 1;
    else
        return 0;
}

unsigned char isRushHour(){
    if (keyCode[11] == 1)
        return 1;
    else
        return 0;
}

unsigned char isNotRushHour(){
    if (keyCode[15] == 1)
        return 1;
    else
        return 0;
}

void phase1GreenSet(void){
    setBlinkCount = (setBlinkCount + 1) % 20;
    if (setBlinkCount == 0){
        setBlinkFlag = !setBlinkFlag;
    }
    if (numIndex == 0){
        LCD_printCharSync(1, 13, setBlinkFlag ? '_' : ' ');
    } else if (numIndex == 1){
        LCD_printCharSync(1, 14, setBlinkFlag ? '_' : ' ');
    } else {
        LCD_printCharSync(1, 13, setBlinkFlag ? numArray[0] : ' ');
        LCD_printCharSync(1, 14, setBlinkFlag ? numArray[1] : ' ');
    }
    if (isButtonNumber() && numIndex < 2){
        LCD_printCharSync(1, numIndex + 13, numValue);
        numArray[numIndex] = numValue;
        numIndex++;
        if (numIndex >= 2){
            phase1GreenTime = (numArray[0] - '0') * 10 + (numArray[1] - '0');
            if (phase1GreenTime <= 10){
                phase1GreenTime = 10;
            }
        }
    }
}

void phase1YellowSet(void){
    setBlinkCount = (setBlinkCount + 1) % 20;
    if (setBlinkCount == 0){
        setBlinkFlag = !setBlinkFlag;
    }
    if (numIndex == 0){
        LCD_printCharSync(1, 13, setBlinkFlag ? '_' : ' ');
    } else if (numIndex == 1){
        LCD_printCharSync(1, 14, setBlinkFlag ? '_' : ' ');
    } else {
        LCD_printCharSync(1, 13, setBlinkFlag ? numArray[0] : ' ');
        LCD_printCharSync(1, 14, setBlinkFlag ? numArray[1] : ' ');
    }
    if (isButtonNumber() && numIndex < 2){
        LCD_printCharSync(1, numIndex + 13, numValue);
        numArray[numIndex] = numValue;
        numIndex++;
        if (numIndex >= 2){
            phase1YellowTime = (numArray[0] - '0') * 10 + (numArray[1] - '0');
            if (phase1YellowTime <= 3){
                phase1YellowTime = 3;
            }
            if (phase1YellowTime >= 10){
                phase1YellowTime = 10;
            }
        }
    }
}

void phase2GreenSet(void){
    setBlinkCount = (setBlinkCount + 1) % 20;
    if (setBlinkCount == 0){
        setBlinkFlag = !setBlinkFlag;
    }
    if (numIndex == 0){
        LCD_printCharSync(1, 13, setBlinkFlag ? '_' : ' ');
    } else if (numIndex == 1){
        LCD_printCharSync(1, 14, setBlinkFlag ? '_' : ' ');
    } else {
        LCD_printCharSync(1, 13, setBlinkFlag ? numArray[0] : ' ');
        LCD_printCharSync(1, 14, setBlinkFlag ? numArray[1] : ' ');
    }
    if (isButtonNumber() && numIndex < 2){
        LCD_printCharSync(1, numIndex + 13, numValue);
        numArray[numIndex] = numValue;
        numIndex++;
        if (numIndex >= 2){
            phase2GreenTime = (numArray[0] - '0') * 10 + (numArray[1] - '0');
            if (phase2GreenTime <= 10){
                phase2GreenTime = 10;
            }
        }
    }
}

void phase2YellowSet(void){
    setBlinkCount = (setBlinkCount + 1) % 20;
    if (setBlinkCount == 0){
        setBlinkFlag = !setBlinkFlag;
    }
    if (numIndex == 0){
        LCD_printCharSync(1, 13, setBlinkFlag ? '_' : ' ');
    } else if (numIndex == 1){
        LCD_printCharSync(1, 14, setBlinkFlag ? '_' : ' ');
    } else {
        LCD_printCharSync(1, 13, setBlinkFlag ? numArray[0] : ' ');
        LCD_printCharSync(1, 14, setBlinkFlag ? numArray[1] : ' ');
    }
    if (isButtonNumber() && numIndex < 2){
        LCD_printCharSync(1, numIndex + 13, numValue);
        numArray[numIndex] = numValue;
        numIndex++;
        if (numIndex >= 2){
            phase2YellowTime = (numArray[0] - '0') * 10 + (numArray[1] - '0');
            if (phase2YellowTime <= 3){
                phase2YellowTime = 3;
            }
            if (phase2YellowTime >= 10){
                phase2YellowTime = 10;
            }
        }
    }
}

void standby(void){
    outputClose(2);
    outputClose(6);
    outputClose(3);
    outputClose(7);
    standbyCount = (standbyCount + 1) % 20;
    if (standbyCount == 0){
        outputReverse(4);
        outputReverse(5);
    }
}

void phase1GreenOn(void){
    outputOpen(2);
}
void phase1GreenOff(void){
    outputClose(2);
}
void phase1YellowOn(void){
    outputOpen(4);
}
void phase1YellowOff(void){
    outputClose(4);
}
void phase1RedOn(void){
    outputOpen(6);
}
void phase1RedOff(void){
    outputClose(6);
}
void phase2GreenOn(void){
    outputOpen(3);
}
void phase2GreenOff(void){
    outputClose(3);
}
void phase2YellowOn(void){
    outputOpen(5);
}
void phase2YellowOff(void){
    outputClose(5);
}
void phase2RedOn(void){
    outputOpen(7);
}
void phase2RedOff(void){
    outputClose(7);
}

void trafficLightStateMachine(void){
    lightCount = (lightCount + 1) % 20;
    if (lightCount == 0){
        lightTime++;
    }
    switch (state){
        case INIT:
            LCD_clearSync();
            lightCount = 0;
            lightTime = 0;
            state = PHASE1_GREEN_AUTO;
            break;
        case STANDBY:
            LCD_printStrSync(0, 0, "STANDBY         ");
            LCD_printStrSync(1, 0, "                ");
            standby();
            if (isButtonStandby() || isButtonSwitchMode()){
                state = INIT;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                state = PHASE1_GREEN_SET;
            }
            break;
        case PHASE1_GREEN_AUTO:
            LCD_printStrSync(0, 0, "P1: GREEN       ");
            LCD_printStrSync(1, 0, "P2: RED         ");
            LCD_printNumSync(0, 13, phase1GreenTime - lightTime);
            LCD_printNumSync(1, 13, phase1GreenTime + phase1YellowTime - lightTime);
            phase1GreenOn();
            phase1YellowOff();
            phase1RedOff();
            phase2GreenOff();
            phase2YellowOff();
            phase2RedOn();
            if (isRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_RUSH;
                phase1YellowTime = PHASE_1_YELLOW_TIME_RUSH;
                phase2GreenTime  = PHASE_2_GREEN_TIME_RUSH;
                phase2YellowTime = PHASE_2_YELLOW_TIME_RUSH;
            }
            if (isNotRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_NORMAL;
                phase1YellowTime = PHASE_1_YELLOW_TIME_NORMAL;
                phase2GreenTime  = PHASE_2_GREEN_TIME_NORMAL;
                phase2YellowTime = PHASE_2_YELLOW_TIME_NORMAL;
            }
            if (lightTime >= phase1GreenTime){
                lightTime = 0;
                state = PHASE1_YELLOW_AUTO;
            }
            if (isButtonSwap()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE2_GREEN_AUTO;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                state = PHASE1_GREEN_MANUAL;
            }
            break;
        case PHASE1_YELLOW_AUTO:
            LCD_printStrSync(0, 0, "P1: YELLOW      ");
            LCD_printStrSync(1, 0, "P2: RED         ");
            LCD_printNumSync(0, 13, phase1YellowTime - lightTime);
            LCD_printNumSync(1, 13, phase1YellowTime - lightTime);
            phase1GreenOff();
            phase1YellowOn();
            phase1RedOff();
            phase2GreenOff();
            phase2YellowOff();
            phase2RedOn();
            if (isRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_RUSH;
                phase1YellowTime = PHASE_1_YELLOW_TIME_RUSH;
                phase2GreenTime  = PHASE_2_GREEN_TIME_RUSH;
                phase2YellowTime = PHASE_2_YELLOW_TIME_RUSH;
            }
            if (isNotRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_NORMAL;
                phase1YellowTime = PHASE_1_YELLOW_TIME_NORMAL;
                phase2GreenTime  = PHASE_2_GREEN_TIME_NORMAL;
                phase2YellowTime = PHASE_2_YELLOW_TIME_NORMAL;
            }
            if (lightTime >= phase1YellowTime){
                lightTime = 0;
                state = PHASE2_GREEN_AUTO;
            }
            if (isButtonSwap()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE2_GREEN_AUTO;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                state = PHASE1_YELLOW_MANUAL;
            }
            break;
        case PHASE2_GREEN_AUTO:
            LCD_printStrSync(0, 0, "P1: RED         ");
            LCD_printStrSync(1, 0, "P2: GREEN       ");
            LCD_printNumSync(0, 13, phase2GreenTime + phase2YellowTime - lightTime);
            LCD_printNumSync(1, 13, phase2GreenTime - lightTime);
            phase1GreenOff();
            phase1YellowOff();
            phase1RedOn();
            phase2GreenOn();
            phase2YellowOff();
            phase2RedOff();
            if (isRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_RUSH;
                phase1YellowTime = PHASE_1_YELLOW_TIME_RUSH;
                phase2GreenTime  = PHASE_2_GREEN_TIME_RUSH;
                phase2YellowTime = PHASE_2_YELLOW_TIME_RUSH;
            }
            if (isNotRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_NORMAL;
                phase1YellowTime = PHASE_1_YELLOW_TIME_NORMAL;
                phase2GreenTime  = PHASE_2_GREEN_TIME_NORMAL;
                phase2YellowTime = PHASE_2_YELLOW_TIME_NORMAL;
            }
            if (lightTime >= phase2GreenTime){
                lightTime = 0;
                state = PHASE2_YELLOW_AUTO;
            }
            if (isButtonSwap()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE1_GREEN_AUTO;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                state = PHASE2_GREEN_MANUAL;
            }
            break;
        case PHASE2_YELLOW_AUTO:
            LCD_printStrSync(0, 0, "P1: RED         ");
            LCD_printStrSync(1, 0, "P2: YELLOW      ");
            LCD_printNumSync(0, 13, phase2YellowTime - lightTime);
            LCD_printNumSync(1, 13, phase2YellowTime - lightTime);
            phase1GreenOff();
            phase1YellowOff();
            phase1RedOn();
            phase2GreenOff();
            phase2YellowOn();
            phase2RedOff();
            if (isRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_RUSH;
                phase1YellowTime = PHASE_1_YELLOW_TIME_RUSH;
                phase2GreenTime  = PHASE_2_GREEN_TIME_RUSH;
                phase2YellowTime = PHASE_2_YELLOW_TIME_RUSH;
            }
            if (isNotRushHour()){
                lightTime = 0;
                lightCount = 0;
                phase1GreenTime  = PHASE_1_GREEN_TIME_NORMAL;
                phase1YellowTime = PHASE_1_YELLOW_TIME_NORMAL;
                phase2GreenTime  = PHASE_2_GREEN_TIME_NORMAL;
                phase2YellowTime = PHASE_2_YELLOW_TIME_NORMAL;
            }
            if (lightTime >= phase2YellowTime){
                lightTime = 0;
                state = PHASE1_GREEN_AUTO;
            }
            if (isButtonSwap()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE1_GREEN_AUTO;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                state = PHASE2_YELLOW_MANUAL;
            }
            break;
        case PHASE1_GREEN_MANUAL:
            LCD_printStrSync(0, 0, "P1: GREEN       ");
            LCD_printStrSync(1, 0, "P2: RED         ");
            LCD_printCharSync(1, 14, 'M');
            phase1GreenOn();
            phase1YellowOff();
            phase1RedOff();
            phase2GreenOff();
            phase2YellowOff();
            phase2RedOn();
            if (isButtonNext()){
                lightTime = 0;
                state = PHASE1_YELLOW_MANUAL;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE1_GREEN_AUTO;
            }
            break;
        case PHASE1_YELLOW_MANUAL:
            LCD_printStrSync(0, 0, "P1: YELLOW      ");
            LCD_printStrSync(1, 0, "P2: RED         ");
            LCD_printCharSync(1, 14, 'M');
            phase1GreenOff();
            phase1YellowOn();
            phase1RedOff();
            phase2GreenOff();
            phase2YellowOff();
            phase2RedOn();
            if (isButtonNext()){
                lightTime = 0;
                state = PHASE2_GREEN_MANUAL;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE1_YELLOW_AUTO;
            }
            break;
        case PHASE2_GREEN_MANUAL:
            LCD_printStrSync(0, 0, "P1: RED         ");
            LCD_printStrSync(1, 0, "P2: GREEN       ");
            LCD_printCharSync(1, 14, 'M');
            phase1GreenOff();
            phase1YellowOff();
            phase1RedOn();
            phase2GreenOn();
            phase2YellowOff();
            phase2RedOff();
            if (isButtonNext()){
                lightTime = 0;
                state = PHASE2_YELLOW_MANUAL;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE2_GREEN_AUTO;
            }
            break;
        case PHASE2_YELLOW_MANUAL:
            LCD_printStrSync(0, 0, "P1: RED         ");
            LCD_printStrSync(1, 0, "P2: YELLOW      ");
            LCD_printCharSync(1, 14, 'M');
            phase1GreenOff();
            phase1YellowOff();
            phase1RedOn();
            phase2GreenOff();
            phase2YellowOn();
            phase2RedOff();
            if (isButtonNext()){
                lightTime = 0;
                state = PHASE1_GREEN_MANUAL;
            }
            if (isButtonStandby()){
                state = STANDBY;
            }
            if (isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            if (isButtonSwitchMode()){
                lightTime = 0;
                lightCount = 0;
                state = PHASE2_YELLOW_AUTO;
            }
            break;
        case PHASE1_GREEN_SET:
            LCD_printStrSync(0, 0, "SET P1 GREEN    ");
            LCD_printStrSync(1, 0, "(10-99s)");
            phase1GreenSet();
            if (numIndex >= 2 && isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE1_YELLOW_SET;
            }
            if (isButtonClearSet()){
                LCD_printStrSync(1, 13, "   ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE1_GREEN_SET;
            }
            break;
        case PHASE1_YELLOW_SET:
            LCD_printStrSync(0, 0, "SET P1 YELLOW   ");
            LCD_printStrSync(1, 0, "(3-10s)");
            phase1YellowSet();
            if (numIndex >= 2 && isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE2_GREEN_SET;
            }
            if (isButtonClearSet()){
                LCD_printStrSync(1, 13, "   ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE1_YELLOW_SET;
            }
            break;
        case PHASE2_GREEN_SET:
            LCD_printStrSync(0, 0, "SET P2 GREEN    ");
            LCD_printStrSync(1, 0, "(10-99s)");
            phase2GreenSet();
            if (numIndex >= 2 && isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE2_YELLOW_SET;
            }
            if (isButtonClearSet()){
                LCD_printStrSync(1, 13, "   ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE2_GREEN_SET;
            }
            break;
        case PHASE2_YELLOW_SET:
            LCD_printStrSync(0, 0, "SET P2 YELLOW   ");
            LCD_printStrSync(1, 0, "(3-10s)");
            phase2YellowSet();
            if (numIndex >= 2 && isButtonSet()){
                LCD_printStrSync(1, 0, "                ");
                numIndex = 0;
                setBlinkCount = 0;
                state = INIT;
            }
            if (isButtonClearSet()){
                LCD_printStrSync(1, 13, "   ");
                numIndex = 0;
                setBlinkCount = 0;
                state = PHASE2_YELLOW_SET;
            }
            break;
        default:
            break;
    }
}
