/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#include <wiringPi.h>
#include <lcd.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <mpd/client.h>
#include "mpdConn.h"
#include "screenLayout.h"

#include <iostream>
using namespace std;

#define ROWS 4
#define COLS 20
#define BITS 4
#define RS 6
#define STRB 5
#define DATA0 4
#define DATA1 0
#define DATA2 2
#define DATA3 3

int lcdHandle;

/*
 * Initialize the wiring pi stuff, write welcom message to the screen
 */
bool initWiringPi()
{
	if (wiringPiSetup() < 0)
	{
		return false;
	}

        lcdHandle = lcdInit(ROWS, COLS, BITS, RS, STRB, DATA0, DATA1, DATA2, DATA3, 0, 0, 0, 0);

	if (lcdHandle < 0)
	{
		return false;
	}

	lcdClear(lcdHandle);

	lcdPuts(lcdHandle, "                    ");
	lcdPuts(lcdHandle, "   Ramo's Jukebox   ");

	return true;
}

void print(const char** data)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			lcdPosition(lcdHandle, j, i);
			lcdPutchar(lcdHandle, data[i][j]);
//			cout << data[i][j];
		}
//		cout << endl;
	}
}

int main()
{
	if (!initWiringPi())
	{
		cerr << "Failed to intialize wiringPi and lcdInit" << endl;
		return false;
	}

        screenLayout screen;
        mpdConn mc("localhost", 6600);

	char ** data = new char*[ROWS];
	for (int i = 0; i < ROWS; i++)
	{
		data[i] = new char[COLS];
	}

	char title[MPD_CONN_STRING_LEN] = "";
        char artist[MPD_CONN_STRING_LEN] = "";
        char album[MPD_CONN_STRING_LEN] = "";
        char year[MPD_CONN_STRING_LEN] = "";
        char tmp[TIMELEN] = "";
        char elapsed[TIMELEN] = "";
        char duration[TIMELEN] = "";

	int songID = 0;
        int oldSongID = -1;

	bool state = true;
	mpd_state oldState = MPD_STATE_UNKNOWN;

	if (!mc.isConnected())
	{
		cerr << "Connection to MPD failed" << endl;
		return false;
	}

	while (state)
	{
		sleep(1);

		if (!mc.updateTags())
		{
			cerr << "Failed to update tags" << endl;
			return false;
		}

		songID = mc.getID();

                if (mc.getState() == MPD_STATE_PLAY || mc.getState() == MPD_STATE_PAUSE)
                {
                        mc.getTitle(title, MPD_CONN_STRING_LEN);
                        mc.getAlbum(album, MPD_CONN_STRING_LEN);
                        mc.getArtist(artist, MPD_CONN_STRING_LEN);
                        mc.getYear(year, MPD_CONN_STRING_LEN);
                }

                if (mc.getState() == MPD_STATE_STOP || songID == -1)
                {
                        strcpy(title, "Stopped");
			strcpy(album, "");
			strcpy(artist, "");
			strcpy(year, "");
                } else if (mc.getState() == MPD_STATE_UNKNOWN)
                {
                        strcpy(title, "Error - MPD STATE UNKNOWN");
			strcpy(album, "");
			strcpy(artist, "");
			strcpy(year, "");
			state = false;
                }

		elapsed[0] = 0;
                duration[0] = 0;

                sprintf(tmp, "%0d", mc.getElapsedTime() / 60);
                strcat(elapsed, tmp);
                strcat(elapsed, ":");
                sprintf(tmp, "%02d", mc.getElapsedTime() % 60);
                strcat(elapsed, tmp);

                sprintf(tmp, "%0d", mc.getDuration() / 60);
                strcat(duration, tmp);
                strcat(duration, ":");
                sprintf(tmp, "%02d", mc.getDuration() % 60);
                strcat(duration, tmp);

		if (songID != oldSongID || mc.getState() != oldState)
                {
                        screen.setContents(title, artist, album, year, elapsed, duration);
                        oldSongID = songID;
                } else
                {
                        screen.setTime(elapsed, duration);
                }
		if (mc.getState() == MPD_STATE_STOP || mc.getState() == MPD_STATE_UNKNOWN)
		{
			screen.setTime("0:00", "0:00");
		} else
		{
			screen.setScreen()
		}

		oldState = mc.getState();

                screen.scrollScreen();
		screen.getContents(data);
                print((const char**)data);
	}








}
