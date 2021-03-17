/*
	Rotary library made by Ben Buxton.
	Licenced under the GNU GPL Version 3.
	Contact: bb@cactii.net
	Copyright 2011

	Modified 5/04/2019 by Carlos Siles
	* Added other Encoder type

	Modified by: Vladimir Gendel aka S60Team
	* Added Faster Digital Write
	* Combined two types of Encoders
	* Moved all text from Header to ReadMe.txt
*/
#include "Arduino.h"
#include "Rotary.h"
#include "DigitalWriteFast.h"

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */
#define R_START 0x0
//
#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
#ifdef ENCODER2
// Modified 5/04/2019 by Carlos Siles
// Modified table to follow sequence 00>10>11>01>01
const unsigned char ttable[6][4] = {
	// R_START (00)
	{R_START,           R_CCW_BEGIN,  R_CW_BEGIN,    R_START_M},
	// R_CCW_BEGIN
	{R_START,           R_CCW_BEGIN,  R_START,       R_START_M | DIR_CCW},
	// R_CW_BEGIN
	{ R_START,          R_START,      R_CW_BEGIN ,   R_START_M | DIR_CW},
	// R_START_M (11)
	{R_START,           R_CW_BEGIN_M, R_CCW_BEGIN_M, R_START_M},
	// R_CW_BEGIN_M
	{R_START | DIR_CW,  R_CW_BEGIN_M, R_START_M,     R_START_M},
	// R_CCW_BEGIN_M
	{R_START | DIR_CCW, R_START_M,    R_CCW_BEGIN_M, R_START_M},
};
#else
const unsigned char ttable[6][4] = {
	// R_START (00)
	{R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
	// R_CCW_BEGIN
	{R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
	// R_CW_BEGIN
	{R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
	// R_START_M (11)
	{R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
	// R_CW_BEGIN_M
	{R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
	// R_CCW_BEGIN_M
	{R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};
#endif
#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6
//
#ifdef ENCODER2
// Modified 5/04/2019 by Carlos Siles
const unsigned char ttable[7][4] = {
// R_START
{R_START,           R_CCW_BEGIN, R_CW_BEGIN,   R_START},
	// R_CW_FINAL
{R_START | DIR_CW,  R_CW_FINAL,  R_START,      R_CW_NEXT},
	// R_CW_BEGIN
{R_START,           R_START,     R_CW_BEGIN,   R_CW_NEXT},
	// R_CW_NEXT
{R_START,           R_CW_FINAL,  R_CW_BEGIN,   R_CW_NEXT},
	// R_CCW_BEGIN
{R_START,           R_CCW_BEGIN, R_START,      R_CCW_NEXT},
	// R_CCW_FINAL
{R_START | DIR_CCW, R_START,     R_CCW_FINAL,  R_CCW_NEXT},
	// R_CCW_NEXT
{R_START,           R_CCW_BEGIN, R_CCW_FINAL,  R_CCW_NEXT},
};
#else
const unsigned char ttable[7][4] = {
	// R_START
	{R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
	// R_CW_FINAL
	{R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
	// R_CW_BEGIN
	{R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
	// R_CW_NEXT
	{R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
	// R_CCW_BEGIN
	{R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
	// R_CCW_FINAL
	{R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
	// R_CCW_NEXT
	{R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif
#endif
// Constructor. Each arg is the pin number for each encoder contact.
Rotary::Rotary(char _pin1, char _pin2) {
	// Assign variables.
	pin1 = _pin1;
	pin2 = _pin2;
	// Set pins to input.
	pinMode(pin1, INPUT);
	pinMode(pin2, INPUT);
#ifdef ENABLE_PULLUPS
	digitalWrite(pin1, HIGH);
	digitalWrite(pin2, HIGH);
#endif
	// Initialise state.
	state = R_START;
}
//
unsigned char Rotary::process() {
	// Grab state of input pins.
	#ifdef DWF
		unsigned char pinstate = (digitalReadFast(pin2) << 1) | digitalReadFast(pin1);
	#else
		unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);
	#endif
	// Determine new state from the pins and state table.
	state = ttable[state & 0xf][pinstate];
	// Return emit bits, ie the generated event.
	#ifdef ENCTEST
		int direction = state & 0x30;
		if (direction != 0){
			if (direction == DIR_CW){
				int x = (1 * state);
				return x;
			}
			else if (direction == DIR_CCW){
				int x = (-1 * state);
				return x;
			}
			else{return 0;}
		}
	#else
		return state & 0x30;
	#endif
}
