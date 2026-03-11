/*
 * setup_moter.c
 */

#include "iodefine.h"
#include <stdint.h>
#include <stdbool.h>

// 変数
volatile int realSpeed = 0;			// 実際の速度を示す変数
volatile int turnL = 0;
volatile int turnR = 0;
volatile int speedL = 0;
volatile int speedR = 0;

// 定数の指定
#define PWM_OFF 0
#define ic_ON  0
#define ic_OFF 1

typedef struct{
	volatile uint16_t *TGR;
	volatile uint8_t *REG;
	uint8_t BIT_POS;
}MOTER_PIN_t;

typedef struct{
	MOTER_PIN_t IN;
	MOTER_PIN_t SD;
	MOTER_PIN_t HL;
}MOTER_CTRL_REG;

typedef struct{
	MOTER_CTRL_REG V;
	MOTER_CTRL_REG U;
	MOTER_CTRL_REG W;
}MOTER_t;


typedef enum{
	L,
	R
}MOTER_SIDE;

#define MOTER_PIN_IN(tgr){\
	(volatile uint16_t*)(tgr), \
	0, \
	0  \
}

#define MOTER_PIN_SD(port, bit){\
	0, \
	(volatile uint8_t*)&PORT##port.PODR.BYTE, \
	(uint8_t) (1 << (bit))\
}

#define MOTER_PIN_HL(port, bit){\
	0, \
	(volatile uint8_t*)&PORT##port.PIDR.BYTE, \
	(uint8_t) (1 << (bit))\
}

const MOTER_t MOTER[2] = {
	{// L
		{// V
			MOTER_PIN_IN(&MTU0.TGRD),
			MOTER_PIN_SD(5, 1),
			MOTER_PIN_HL(D, 0)
		},
		{// U
			MOTER_PIN_IN(&MTU0.TGRC),
			MOTER_PIN_SD(5, 0),
			MOTER_PIN_HL(D, 1)
		},
		{// W
			MOTER_PIN_IN(&MTU0.TGRB),
			MOTER_PIN_SD(5, 2),
			MOTER_PIN_HL(D, 2)
		},
	},
	{// R
		{// V
			MOTER_PIN_IN(&MTU1.TGRB),
			MOTER_PIN_SD(5, 4),
			MOTER_PIN_HL(D, 3)
		},
		{// U
			MOTER_PIN_IN(&MTU1.TGRA),
			MOTER_PIN_SD(5, 3),
			MOTER_PIN_HL(D, 4)
		},
		{// W
			MOTER_PIN_IN(&MTU2.TGRA),
			MOTER_PIN_SD(5, 5),
			MOTER_PIN_HL(D, 5)
		},
	}
};

void WRITE_MOTER_PIN(const MOTER_PIN_t *pin, bool val){
	if(val == 1) {
		*(pin->REG) |= pin->BIT_POS;
	}else {
		*(pin->REG) &= ~(pin->BIT_POS);
	}
}

uint8_t READ_MOTER_PIN(const MOTER_PIN_t *pin){
	if(*(pin->REG)&(pin->BIT_POS)) return 1;
	return 0;
}

uint8_t SENS_PATTERN(MOTER_SIDE side){
		if(
		   !READ_MOTER_PIN(&MOTER[side].U.HL)    &&
		   READ_MOTER_PIN(&MOTER[side].V.HL)     &&
		   !READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 1;

		else if(
		   !READ_MOTER_PIN(&MOTER[side].U.HL)    &&
		   READ_MOTER_PIN(&MOTER[side].V.HL)    &&
		   READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 2;

		else if(
			!READ_MOTER_PIN(&MOTER[side].U.HL)    &&
			!READ_MOTER_PIN(&MOTER[side].V.HL)    &&
			READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 3;

		else if(
			READ_MOTER_PIN(&MOTER[side].U.HL)    &&
			!READ_MOTER_PIN(&MOTER[side].V.HL)     &&
			READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 4;

		else if(
			READ_MOTER_PIN(&MOTER[side].U.HL)    &&
			!READ_MOTER_PIN(&MOTER[side].V.HL)    &&
		    !READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 5;

		else if(
			READ_MOTER_PIN(&MOTER[side].U.HL)    &&
			READ_MOTER_PIN(&MOTER[side].V.HL)    &&
			!READ_MOTER_PIN(&MOTER[side].W.HL)
		)  return 6;

		else return 0;
	}

void MOVEMENT_PATTERN(MOTER_SIDE side, uint8_t pattern){
	*MOTER[side].U.IN.TGR = PWM_OFF;
	*MOTER[side].V.IN.TGR = PWM_OFF;
	*MOTER[side].W.IN.TGR = PWM_OFF;

	switch(pattern){
		case 1:
			*MOTER[side].U.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_OFF);
			break;
		case 2:
			*MOTER[side].U.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_ON);
			break;
		case 3:
			*MOTER[side].V.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_ON);
			break;
		case 4:
			*MOTER[side].V.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_OFF);
			break;
		case 5:
			*MOTER[side].W.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_ON);
			break;
		case 6:
			*MOTER[side].W.IN.TGR = speedL;
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_ON);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_ON);
			break;
		default:
			WRITE_MOTER_PIN(&MOTER[side].U.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].V.SD, ic_OFF);
			WRITE_MOTER_PIN(&MOTER[side].W.SD, ic_OFF);
			break;
		}
}

void BLDC(MOTER_SIDE side)
{
	static int turn;
	int speed = (side == L) ? speedL : speedR;
	static int pattern = 0;//現在のパターン記憶用変数
	static int oldpattern = 0;
	int intCount = 0;
	turn = (side == L) ? 1 : -1;
	intCount++;
	// センサの状態からパターンを決定

	pattern = SENS_PATTERN(side);
	if(pattern == 0) return;

	// 回転方向に応じてパターンを変更
	if(turn >= 0) {
		pattern = (pattern % 6) + 1;
	//pattern = pattern ;
	} else {
	//pattern = (pattern - 4 + 6) % 6 + 1;
		pattern = (pattern - 3 + 6) % 6 + 1;
	}

	// パターンに応じた動作
	MOVEMENT_PATTERN(side, pattern);

	// 回転数の計測
	if(pattern != oldpattern) {
		realSpeed = intCount;
		intCount = 0;
	}
	oldpattern = pattern;
}

