// -----------------------------------------------------------------------------
// Altair 8800 Simulator
//
// This file contains copies of the following programs:
// - Kill-the-Bit game (C) Dean McDaniel 1975
// - 'Daisy, Daisy...' music (C) Steve Dompier 1975
// - Pong on Altair front panel (C) Unknown
// - Pong on Altair via terminal (C) David Hansel 2017
//
// While the Altair 8800 simulator code itself is released under GPLv3, 
// the contents of this file are excluded from that license for obvious reasons.
// -----------------------------------------------------------------------------

#include <Arduino.h>

#include "prog_games.h"
#include "prog.h"
#include "mem.h"

static const byte PROGMEM killbits[] = {
0x21, 0x00, 0x00, 0x16, 0x80, 0x01, 0x0E, 0x00, 0x1A, 0x1A, 0x1A, 0x1A, 0x09, 0xD2, 0x08, 0x00,
0xDB, 0xFF, 0xAA, 0x0F, 0x57, 0xC3, 0x08, 0x00};


static const byte PROGMEM pong[] = {
0001, 0016, 0000, 0061, 0175, 0000, 0041, 0000, 0000, 0042, 0200, 0000, 0021, 0000, 0200, 0303,
0036, 0000, 0041, 0000, 0000, 0032, 0032, 0032, 0032, 0011, 0322, 0025, 0000, 0311, 0315, 0022,
0000, 0333, 0377, 0346, 0001, 0312, 0055, 0000, 0172, 0263, 0346, 0037, 0137, 0172, 0346, 0001,
0312, 0105, 0000, 0173, 0346, 0002, 0302, 0077, 0000, 0173, 0346, 0001, 0302, 0163, 0000, 0041,
0201, 0000, 0064, 0036, 0000, 0172, 0017, 0127, 0303, 0036, 0000, 0315, 0022, 0000, 0333, 0377,
0346, 0200, 0312, 0135, 0000, 0172, 0017, 0017, 0017, 0263, 0346, 0037, 0137, 0172, 0346, 0200,
0312, 0165, 0000, 0173, 0346, 0010, 0302, 0157, 0000, 0173, 0346, 0020, 0302, 0103, 0000, 0041,
0200, 0000, 0064, 0036, 0000, 0172, 0007, 0127, 0303, 0113, 0000};


static const byte PROGMEM saver[] = {
  0x06, 0x80, 0x21, 0x00, 0x00, 0xDB, 0x10, 0x07, 0xDA, 0x05, 0x02, 0x7E, 0xD3, 0x11, 0x23, 0x05, 
  0xC2, 0x05, 0x02, 0x76 };

static const byte PROGMEM loader[] = {
  0xDB, 0x11, 0xB7, 0xCA, 0x00, 0x03, 0x21, 0x00, 0x00, 0x77, 0x23, 0xDB, 0x10, 0x0F, 0xD2, 0x0B,
  0x03, 0xDB, 0x11, 0xC3, 0x09, 0x03 };


static const byte PROGMEM pong_terminal[] = {
  0x31, 0x57, 0x02, 0x21, 0x1C, 0x02, 0xCD, 0x04, 0x02, 0x3E, 0x00, 0x32, 0x1A, 0x02, 0x32, 0x1B,
  0x02, 0xCD, 0xAD, 0x01, 0x3E, 0x09, 0x32, 0x45, 0x02, 0x57, 0x3E, 0x09, 0x32, 0x46, 0x02, 0x5F,
  0x42, 0x0E, 0x01, 0xCD, 0xCA, 0x01, 0x21, 0x31, 0x02, 0xCD, 0x04, 0x02, 0x43, 0x0E, 0x46, 0xCD,
  0xCA, 0x01, 0x21, 0x31, 0x02, 0xCD, 0x04, 0x02, 0x06, 0x89, 0x0E, 0xA3, 0x78, 0xE6, 0x7F, 0xFE,
  0x14, 0xC2, 0x45, 0x00, 0x47, 0xFE, 0x02, 0xC2, 0x4D, 0x00, 0xF6, 0x80, 0x47, 0x78, 0xE6, 0x7F,
  0x67, 0x79, 0xE6, 0x7F, 0x6F, 0xFE, 0x45, 0xC2, 0x69, 0x00, 0x7C, 0xBB, 0xFA, 0x5A, 0x01, 0xD6,
  0x03, 0xBB, 0xF2, 0x5A, 0x01, 0x4D, 0xC3, 0x7E, 0x00, 0x7D, 0xFE, 0x02, 0xC2, 0x7E, 0x00, 0x7C,
  0xBA, 0xFA, 0x7B, 0x01, 0xD6, 0x03, 0xBA, 0xF2, 0x7B, 0x01, 0x7D, 0xF6, 0x80, 0x4F, 0xC5, 0xDB,
  0x10, 0x0F, 0xD2, 0xD3, 0x00, 0xDB, 0x11, 0xE6, 0xDF, 0x21, 0x45, 0x02, 0xFE, 0x41, 0xC2, 0x9D,
  0x00, 0x3A, 0x45, 0x02, 0xFE, 0x02, 0xFA, 0x7F, 0x00, 0x35, 0xC3, 0x7F, 0x00, 0xFE, 0x5A, 0xC2,
  0xAE, 0x00, 0x3A, 0x45, 0x02, 0xFE, 0x12, 0xF2, 0x7F, 0x00, 0x34, 0xC3, 0x7F, 0x00, 0x21, 0x46,
  0x02, 0xFE, 0x4B, 0xC2, 0xC2, 0x00, 0x3A, 0x46, 0x02, 0xFE, 0x02, 0xFA, 0x7F, 0x00, 0x35, 0xC3,
  0x7F, 0x00, 0xFE, 0x4D, 0xC2, 0x7F, 0x00, 0x3A, 0x46, 0x02, 0xFE, 0x12, 0xF2, 0x7F, 0x00, 0x34,
  0xC3, 0x7F, 0x00, 0x3A, 0x45, 0x02, 0x42, 0x0E, 0x01, 0xCD, 0x15, 0x01, 0x50, 0x3A, 0x46, 0x02,
  0x43, 0x0E, 0x46, 0xCD, 0x15, 0x01, 0x58, 0xC1, 0xCD, 0xCA, 0x01, 0x3E, 0x20, 0xCD, 0x0E, 0x02,
  0x79, 0xE6, 0x01, 0xCA, 0x00, 0x01, 0x78, 0xB7, 0xF2, 0xFF, 0x00, 0x04, 0xC3, 0x00, 0x01, 0x05,
  0x79, 0xB7, 0xF2, 0x09, 0x01, 0x0C, 0xC3, 0x0A, 0x01, 0x0D, 0xCD, 0xCA, 0x01, 0x3E, 0x2A, 0xCD,
  0x0E, 0x02, 0xC3, 0x3C, 0x00, 0xB8, 0xCA, 0x4A, 0x01, 0xFA, 0x32, 0x01, 0xCD, 0xCA, 0x01, 0x3E,
  0x20, 0xCD, 0x0E, 0x02, 0x04, 0x04, 0x04, 0xCD, 0xCA, 0x01, 0x3E, 0x49, 0xCD, 0x0E, 0x02, 0x05,
  0x05, 0xC9, 0x05, 0xCD, 0xCA, 0x01, 0x3E, 0x49, 0xCD, 0x0E, 0x02, 0x04, 0x04, 0x04, 0xCD, 0xCA,
  0x01, 0x3E, 0x20, 0xCD, 0x0E, 0x02, 0x05, 0x05, 0x05, 0xC9, 0xCD, 0xCA, 0x01, 0x3E, 0x49, 0xCD,
  0x0E, 0x02, 0xCD, 0xCA, 0x01, 0x3E, 0x49, 0xC3, 0x0E, 0x02, 0x3A, 0x1A, 0x02, 0x3C, 0x32, 0x1A,
  0x02, 0xCD, 0xAD, 0x01, 0xCD, 0xCA, 0x01, 0x3E, 0x20, 0xCD, 0x0E, 0x02, 0x3A, 0x1A, 0x02, 0xFE,
  0x0A, 0xCA, 0x9C, 0x01, 0x06, 0x89, 0x0E, 0x23, 0xC3, 0x3C, 0x00, 0x3A, 0x1B, 0x02, 0x3C, 0x32,
  0x1B, 0x02, 0xCD, 0xAD, 0x01, 0xCD, 0xCA, 0x01, 0x3E, 0x20, 0xCD, 0x0E, 0x02, 0x3A, 0x1B, 0x02,
  0xFE, 0x0A, 0xCA, 0x9C, 0x01, 0x06, 0x89, 0x0E, 0xA3, 0xC3, 0x3C, 0x00, 0x21, 0x27, 0x02, 0xCD,
  0x04, 0x02, 0xDB, 0x10, 0x0F, 0xD2, 0xA2, 0x01, 0xDB, 0x11, 0xC3, 0xA2, 0x01, 0xC5, 0x06, 0x01,
  0x0E, 0x0A, 0xCD, 0xCA, 0x01, 0x3A, 0x1A, 0x02, 0xCD, 0xEB, 0x01, 0x06, 0x01, 0x0E, 0x3C, 0xCD,
  0xCA, 0x01, 0x3A, 0x1B, 0x02, 0xCD, 0xEB, 0x01, 0xC1, 0xC9, 0x3E, 0x1B, 0xCD, 0x0E, 0x02, 0x3E,
  0x5B, 0xCD, 0x0E, 0x02, 0x78, 0xE6, 0x7F, 0xCD, 0xEB, 0x01, 0x3E, 0x3B, 0xCD, 0x0E, 0x02, 0x79,
  0xE6, 0x7F, 0xCD, 0xEB, 0x01, 0x3E, 0x48, 0xCD, 0x0E, 0x02, 0xC9, 0x26, 0x00, 0x24, 0xDE, 0x0A,
  0xD2, 0xED, 0x01, 0x25, 0xC6, 0x0A, 0x6F, 0x7C, 0xC6, 0x30, 0xCD, 0x0E, 0x02, 0x7D, 0xC6, 0x30,
  0xCD, 0x0E, 0x02, 0xC9, 0x7E, 0xB7, 0xC8, 0xCD, 0x0E, 0x02, 0x23, 0xC3, 0x04, 0x02, 0xF5, 0xDB,
  0x10, 0x0F, 0x0F, 0xD2, 0x0F, 0x02, 0xF1, 0xD3, 0x11, 0xC9, 0x00, 0x00, 0x1B, 0x5B, 0x32, 0x4A,
  0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x6C, 0x00, 0x1B, 0x5B, 0x48, 0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x68,
  0x00, 0x49, 0x1B, 0x5B, 0x31, 0x44, 0x1B, 0x5B, 0x31, 0x42, 0x49, 0x1B, 0x5B, 0x31, 0x44, 0x1B,
  0x5B, 0x31, 0x42, 0x49, 0x00, 0x00, 0x00};



// http://www.digibarn.com/collections/weirdstuff/altair-sheetmusic
// http://kevindriscoll.org/projects/ccswg2012/fool_on_a_hill.html
static const byte PROGMEM daisy[] = {
  0041, 0040, 0000, 0176, 0376, 0377, 0312, 0000, 
  0000, 0026, 0070, 0005, 0302, 0020, 0000, 0106,
  0015, 0302, 0013, 0000, 0025, 0302, 0013, 0000,
  0054, 0303, 0003, 0000, 0000, 0000, 0000, 0000,
  0034, 0034, 0034, 0042, 0042, 0042, 0053, 0053, 
  0053, 0071, 0071, 0071, 0063, 0055, 0053, 0063, 
  0063, 0053, 0071, 0071, 0071, 0071, 0071, 0071, 
  0046, 0046, 0046, 0034, 0034, 0034, 0042, 0042, 
  0042, 0053, 0053, 0053, 0063, 0055, 0053, 0046, 
  0046, 0042, 0046, 0046, 0046, 0046, 0046, 0042, 
  0040, 0042, 0046, 0034, 0034, 0042, 0046, 0053, 
  0053, 0053, 0053, 0046, 0042, 0042, 0053, 0063, 
  0063, 0053, 0063, 0071, 0071, 0071, 0071, 0071, 
  0053, 0053, 0042, 0046, 0046, 0071, 0053, 0053, 
  0042, 0046, 0042, 0040, 0034, 0042, 0053, 0046, 
  0046, 0071, 0053, 0053, 0053, 0053, 0002, 0377};


uint16_t prog_games_copy_killbits()
{
  if( prog_copy_to_ram(0x0000, killbits, sizeof(killbits)) )
    {
#ifdef __AVR_ATmega2560__
      // modify game to run at reasonable speed on (slow) MEGA
      MWRITE(6, 056);
#endif
      return 0x0000;
    }
  else
    return 0xFFFF;
}

uint16_t prog_games_copy_pong()
{
  if( prog_copy_to_ram(0x0000, pong, sizeof(pong)) )
    {
      prog_copy_to_ram(0x0200, saver, sizeof(saver));
      prog_copy_to_ram(0x0300, loader, sizeof(loader));
#ifdef __AVR_ATmega2560__
      // modify game to run at reasonable speed on (slow) MEGA
      MWRITE(1, 056);
#endif
      return 0x0000;
    }
  else
    return 0xFFFF;
}


uint16_t prog_games_copy_pongterm()
{
  if( prog_copy_to_ram(0x0000, pong_terminal, sizeof(pong_terminal)) )
    return 0x0000;
  else
    return 0xFFFF;
}


uint16_t prog_games_copy_daisy()
{
  if( prog_copy_to_ram(0x0000, daisy, sizeof(daisy)) )
    return 0x0000;
  else
    return 0xFFFF;
}



/* ------------------------------------------------------------------------
 * -----------------------  PONG on terminal ------------------------------
 * ------------------------------------------------------------------------

	ORG	0H

	;INITIALIZE SCREEN
START:	LXI	SP,STCK	;SET STACK POINTER
	LXI	H,INITS	;CLEAR SCREEN AND HIDE CURSOR
	CALL	STROUT
	;INITIALIZE SCORE
	MVI	A,0	;SET INITIAL SCORES TO 0
	STA	SCR1
	STA	SCR2
	CALL	PSCR	;PRINT INITIAL SCORES
	;INITIALIZE PLAYERS
	MVI	A,9	;PLAYER 1 STARTS AT ROW 9
	STA	P1R
	MOV	D,A
	MVI	A,9	;PLAYER 2 STARTS AT ROW 9
	STA	P2R
	MOV	E,A
	MOV	B,D	;MOVE CURSOR TO ROW/COL OF PLAYER 1
	MVI	C,1
	CALL	MOVC
	LXI	H,PLYRS	;DISPLAY PLAYER
	CALL	STROUT
	MOV	B,E	;MOVE CURSOR TO ROW/COL OF PLAYER 2
	MVI	C,70
	CALL	MOVC
	LXI	H,PLYRS	;DISPLAY PLAYER
	CALL	STROUT
	;INITIALIZE BALL
	MVI	B,089H	;START AT ROW 9,GOING DOWN
	MVI	C,0A3H	;START AT COLUMN 35,GOING RIGHT
	;MANAGE VERTICAL BALL DIRECTION
LOOP:	MOV	A,B	;CHECK ROW
	ANI	7FH	;MASK OUT BIT 7
	CPI	20	;AT ROW 20?
	JNZ	L1
	MOV	B,A	;YES => CLEAR BIT 7 (REVERSE DIRECTION)
L1:	CPI	2	;AT ROW 2?
	JNZ	L2
	ORI	80H	;YES => SET BIT 7 (REVERSE DIRECTION)
	MOV	B,A
	;MANAGE HORIZONTAL BALL DIRECTION
L2:	MOV	A,B	;BALL ROW TO H
	ANI	7FH
	MOV	H,A
	MOV	A,C	;BALL COLUMN TO L
	ANI	7FH
	MOV	L,A
	CPI	69	;CHECK IF COLUMN = 69
	JNZ	L41                
	MOV	A,H
	CMP	E
	JM	SCP1	;BALL ROW < PLAYER 2 => PLAYER 1 SCORES
	SUI	3
	CMP	E
	JP	SCP1	;BALL ROW - 3 >= PLAYER 2 => PLAYER 1 SCORES
	MOV	C,L	;MAKE BALL MOVE LEFT
	JMP	L42
L41:	MOV	A,L
	CPI	2	;CHECK IF COLUMN = 2
	JNZ	L42
	MOV	A,H
	CMP	D
	JM	SCP2	;BALL ROW < PLAYER 1 => PLAYER 2 SCORES
	SUI	3
	CMP	D
	JP	SCP2	;BALL ROW - 3 >= PLAYER 1 => PLAYER 2 SCORES
	MOV	A,L	;MAKE BALL MOVE RIGHT
	ORI	80H
	MOV	C,A
L42:	PUSH	B	;SAVE BALL POSITION
	; CHECK INPUTS
CHKIN:	IN	10H
	RRC
	JNC	MPLYR
	IN      11H
	ANI	0DFH	;ACCEPT LOWERCASE
	LXI	H,P1R
	CPI	65	;"A" => PLAYER 1 UP
        JNZ	L3A
	LDA	P1R
	CPI	2
	JM	CHKIN
        DCR	M
	JMP	CHKIN
L3A:	CPI	90	;"Z" => PLAYER 1 DOWN
	JNZ	L3B
	LDA	P1R
	CPI	18
	JP	CHKIN
        INR	M
	JMP	CHKIN
L3B:	LXI	H,P2R
	CPI	75	;"K" => PLAYER 2 UP
        JNZ	L3C
	LDA	P2R
	CPI	2
	JM	CHKIN
        DCR	M
	JMP	CHKIN
L3C:	CPI	77	;"M" => PLAYER 2 DOWN
	JNZ	CHKIN
	LDA	P2R
	CPI	18
	JP	CHKIN
        INR	M
	JMP	CHKIN

	;MANAGE PLAYER 1
MPLYR:	LDA	P1R
	MOV	B,D
	MVI	C,1
        CALL	PLYR
	MOV	D,B

	;MANAGE PLAYER 2
	LDA	P2R
	MOV	B,E
	MVI	C,70
        CALL	PLYR
	MOV	E,B

	;MOVE BALL
	POP	B	;RESTORE BALL POSIION
	CALL 	MOVC	;MOVE CURSOR TO CURRENT BALL POSITION
	MVI	A," "	;ERASE BALL AT CURRENT POSITION
	CALL	CHROUT
	MOV	A,C	;ONLY MOVE VECRTIAL AT EVERY OTHER
	ANI	1	;HORIZONTAL STEP
	JZ	L11
	MOV	A,B	;CHECK VERTICAL BALL DIRECTION
	ORA	A
	JP	L10	;JUMP IF BIT 7 IS 0 (MOVING DOWN)
	INR	B	;BIT 7 IS 1 (MOVING UP)
	JMP	L11                 
L10:	DCR	B	;BIT 7 IS 0 (MOVING DOWN)
L11:	MOV	A,C	;CHECK HORIZONTAL BALL DIRECTION
	ORA	A                   
	JP	L20	;JUMP IF BIT 7 IS 0 (MOVING LEFT)
	INR	C	;BIT 7 IS 1 (MOVING RIGHT)
	JMP	L21
L20:	DCR	C	;BIT 7 IS 0 (MOVING LEFT)
L21:	CALL	MOVC	;MOVE CURSOR TO NEW BALL POSITION
	MVI	A,"*"	;DRAW BALL AT NEW POSITION
	CALL	CHROUT
	;START NEXT CYCLE
	JMP	LOOP

PLYR:	CMP	B
	JZ	PNOP
	JM	PUP
PDN:	CALL	MOVC	;MOVING DOWN
	MVI	A," "
	CALL	CHROUT
	INR     B
	INR     B
	INR     B
	CALL	MOVC
	MVI	A,"I"
	CALL	CHROUT
	DCR	B
	DCR	B
	RET
PUP:	DCR	B	;MOVING UP
	CALL	MOVC
	MVI	A,"I"
	CALL	CHROUT
	INR	B
	INR	B
	INR	B
	CALL	MOVC
	MVI	A," "
	CALL	CHROUT
	DCR	B
	DCR	B
	DCR	B
	RET
PNOP:	CALL	MOVC	;NOT MOVING
	MVI	A,"I"
	CALL	CHROUT
	CALL	MOVC
	MVI	A,"I"
	JMP	CHROUT

	;PLAYER 1 SCORES
SCP1:	LDA	SCR1	;INCREASE SCORE OF PLAYER 1
	INR	A
	STA	SCR1
	CALL	PSCR	;PRINT SCORES
	CALL	MOVC	;MOVE TO CURRENT POSITION OF BALL
	MVI	A," "	;ERASE BALL
	CALL	CHROUT
	LDA	SCR1
	CPI	10
	JZ	END	;IF SCORE=10 then end game
	MVI	B,89H	;START IN ROW 9,MOVING DOWN
	MVI	C,23H	;START IN COLUMN 35,MOVING LEFT
	JMP	LOOP	;START NEXT CYCLE
	;PLAYER 2 SCORES
SCP2:	LDA  SCR2	;INCREASE SCORE OF PLAYER 2
	INR	A
	STA	SCR2
	CALL	PSCR	;PRINT SCORES
	CALL	MOVC	;MOVE TO CURRENT POSITION OF BALL
	MVI	A," "	;ERASE BALL
	CALL	CHROUT
	LDA	SCR1
	CPI	10
	JZ	END	;IF SCORE=10 then end game
	MVI	B,089H	;START IN ROW 9,MOVING DOWN
	MVI	C,0A3H	;START IN COLUMN 35,MOVING RIGHT
	JMP	LOOP	;START NEXT CYCLE
	;PRINT BOTH PLAYER'S SCORES
END:	LXI	H,ENDS	;CLEAR SCREEN AND SHOW CURSOR
	CALL	STROUT
EMPTY:	IN	10H	;EMPTY INPUT BUFFER
	RRC
	JNC	EMPTY
	IN	11H
	JMP	EMPTY
PSCR:	PUSH B		;SAVE BALL POSITION
	MVI	B,1	;MOVE CURSOR TO ROW 1,COLUMN 10
	MVI	C,10               
	CALL	MOVC
	LDA	SCR1	;LOAD PLAYER 1 SCORE
	CALL	DSPN	;PRINT NUMBER IN A
	MVI	B,1	;MOVE CURSOR TO ROW 1,COLUMN 60
	MVI	C,60
	CALL	MOVC
	LDA	SCR2	;LOAD PLAYER 2 SCORE
	CALL	DSPN	;PRINT NUMBER IN A
	POP	B	;RESTORE BALL POSITION
	RET
	;MOVE CURSOR TO POSITION IN B (ROW) AND C (COLUMN)
MOVC:	MVI	A,1BH	;PRINT ESC
	CALL	CHROUT
	MVI	A,"["	;PRINT '['
	CALL	CHROUT
	MOV	A,B	;ROW TO A
	ANI	7FH	;CLEAR BIT 7
	CALL	DSPN	;PRINT NUMBER IN A
	MVI	A,";"	;PRINT ';'
	CALL	CHROUT
	MOV	A,C	;COLUMN TO A
	ANI	7FH	;CLEAR BIT 7
	CALL	DSPN	;PRINT NUMBER IN A
	MVI	A,"H"	;PRINT 'H;
	CALL	CHROUT
	RET
	;PRINT 2-DIGIT DECIMAL NUMBER IN A
DSPN:	MVI H,0	;10S DIGIT IS 0
DSPL1:	INR H	;INCREASE 10S DIGIT
	SBI	10	;SUBTRACT 10 FROM SCORE
	JNC	DSPL1	;REPEAT UNTIL OVERFLOW
	DCR	H	;OVERSHOT BY ONE
	ADI	10	;INCREASE BY 10 AGAIN TO GET 1S DIGIT
	MOV	L,A	;SAVE 1S DIGIT
	MOV	A,H	;GET 10S DIGIT
	ADI	30H	;ADD 48 TO GET ASCII VALUE
	CALL	CHROUT	;PRINT DIGIT
	MOV	A,L	;GET 1S DIGIT
	ADI	30H	;ADD 48 TO GET ASCII VALUE
	CALL	CHROUT	;PRINT DIGIT
	RET
	;PRINT STRING POINTED TO BY H/L
STROUT:	MOV	A,M	;LOAD NEXT CHARACTER
	ORA	A	;UPDATE Z FLAG
	RZ		;RETURN IF 0
	CALL	CHROUT	;OUTPUT CHARACTER
	INX	H	;INCREASE POINTER
	JMP	STROUT	;START OVER
CHROUT:	PUSH	A
WAIT:	IN	10H
	RRC
	RRC
	JNC	WAIT
	POP	A
	OUT	11H
	RET
SCR1:	DB	0		;PLAYER 1 SCORE
SCR2:	DB	0		;PLAYER 2 SCORE
INITS:	DB	27,"[2J"	;CLEAR SCREEN
	DB	27,"[?25",108	;HIDE CURSOR
	DB	0
ENDS:	DB	27,"[H"		;CURSOR HOME
	DB	27,"[?25",104	;SHOW CURSOR
	DB	0
	;PLAYER ICON
PLYRS:	DB	"I"		;'I'
	DB	27,"[1D"	;CURSOR LEFT
	DB	27,"[1B"	;CURSOR DOWN
	DB	"I"		;'I'
	DB	27,"[1D"	;CURSOR LEFT
	DB	27,"[1B"	;CURSOR DOWN
	DB	"I"		;'I'
	DB	0
P1R:	DB	0	;PLAYER1 REQUESTED POS
P2R:	DB	0	;PLAYER2 REQUESTED POS
STCKB:	DS	16	;RESERVE 16 BYTES FOR STACK
STCK:	BEG	START
	END	PONG
	EOA

*/
