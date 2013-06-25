/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#ifndef INTERRUPTS_H
#define  INTERRUPTS_H

#define BOUNCEDELAY 200

#define PLAYPIN 13
#define STOPPIN 1
#define PREVPIN 14
#define NEXTPIN 15

void initInterrupts();
void play(void);
void prev(void);
void stop(void);
void next(void);

#endif	/* INTERRUPTS_H */

