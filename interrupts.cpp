/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#include <wiringPi.h>
#include "mpdConn.h"
#include "interrupts.h"

extern mpdConn mc;

unsigned int lastPress;

void initInterrupts()
{
  pullUpDnControl(PLAYPIN, PUD_DOWN);
	pullUpDnControl(STOPPIN, PUD_DOWN);
	pullUpDnControl(PREVPIN, PUD_DOWN);
	pullUpDnControl(NEXTPIN, PUD_DOWN);

	pinMode(PLAYPIN, INPUT);
	pinMode(STOPPIN, INPUT);
	pinMode(PREVPIN, INPUT);
	pinMode(NEXTPIN, INPUT);

	wiringPiISR (PLAYPIN, INT_EDGE_FALLING, &play) ;
	wiringPiISR (STOPPIN, INT_EDGE_FALLING, &stop) ;
	wiringPiISR (PREVPIN, INT_EDGE_FALLING, &prev) ;
	wiringPiISR (NEXTPIN, INT_EDGE_FALLING, &next) ;

	lastPress = millis();
}

void play(void)
{
	if (lastPress + BOUNCEDELAY < millis())
	{
		mc.toggle();
		lastPress = millis();
	}
}

void prev(void)
{
	if (lastPress + BOUNCEDELAY < millis())
	{
		mc.prev();
		lastPress = millis();
	}
}

void stop(void)
{
	if (lastPress + BOUNCEDELAY < millis())
	{
		mc.stop();
		lastPress = millis();
	}
}

void next(void)
{
	if (lastPress + BOUNCEDELAY < millis())
	{
		mc.next();
		lastPress = millis();
	}
}
