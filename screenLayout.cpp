/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#include <string>
#include <string.h>
#include "screenLayout.h"

screenLayout::screenLayout()
{
	lock = new pthread_mutex_t;
	pthread_mutex_init(lock, NULL);
	pthread_mutex_lock(lock);

	screen = new char*[ROWS];
	for (int i = 0; i < ROWS; i++)
	{
		screen[i] = new char[COLS];
	}

	clearScreen();

	titlePos = 0;
	artistPos = 0;
	albumYearPos = 0;

	pthread_mutex_unlock(lock);
}

screenLayout::~screenLayout()
{
	pthread_mutex_lock(lock);

	for (int i = 0; i < ROWS; i++)
	{
		delete [] screen[i];
	}

	delete [] screen;

	pthread_mutex_unlock(lock);
	pthread_mutex_destroy(lock);
	delete lock;
}

/*
 * Data should be a char[ROWS][COLS] sized array
 */
void screenLayout::setScreen(const char** data)
{
	pthread_mutex_lock(lock);

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			screen[i][j] = data[i][j];
		}
	}

	pthread_mutex_unlock(lock);
}

/*
 * All of these must be null terminated strings. Year may be NULL.
 */
void screenLayout::setContents(const char* title, const char* artist, const char* album, const char* year, const char* elapsed, const char* duration)
{
	pthread_mutex_lock(lock);

	titleStr.assign(title);
	artistStr.assign(artist);
	albumYearStr.assign(album);

	if (year != NULL && strlen(year))
	{
		albumYearStr += " (";
		albumYearStr += year;
		albumYearStr += ")";
	}

	unsigned long maxLen = titleStr.length() > artistStr.length() ? titleStr.length() : artistStr.length();
	maxLen = maxLen > albumYearStr.length() ? maxLen : albumYearStr.length();

	pad(&titleStr, maxLen);
	pad(&artistStr, maxLen);
	pad(&albumYearStr, maxLen);


	/* setTime needs it's own locking, so we need to unlock before calling it */
	pthread_mutex_lock(lock);

	setTime(elapsed, duration);

	/* and then unlock */
	pthread_mutex_unlock(lock);

	titlePos = 0;
	artistPos = 0;
	albumYearPos = 0;

	pthread_mutex_unlock(lock);
}

void screenLayout::setTime(const char* elapsed, const char* duration)
{
	pthread_mutex_lock(lock);

	timeStr.assign(elapsed);
	timeStr += "/";
	timeStr += duration;
	timeStr.insert(timeStr.begin(), COLS - timeStr.length(), ' ');

	if (timeStr == "                   /")
	{
		timeStr = "                    ";
	}

	pthread_mutex_unlock(lock);
}

/*
 * Scrolls the lines on the screen as necessary
 */
void screenLayout::scrollScreen()
{
	pthread_mutex_lock(lock);

	clearScreen();

	for (unsigned int j = 0; j < COLS; j++)
	{
		screen[0][j] = titleStr[(j + titlePos) % titleStr.length()];
		screen[1][j] = artistStr[(j + artistPos) % artistStr.length()];
		screen[2][j] = albumYearStr[(j + albumYearPos) % albumYearStr.length()];
		screen[3][j] = timeStr[j];
	}

	titlePos = titleStr.length() > COLS ? (titlePos + 1) % titleStr.length() : 0;
	artistPos = artistStr.length() > COLS ? (artistPos + 1) % artistStr.length() : 0;
	albumYearPos = albumYearStr.length() > COLS ? (albumYearPos + 1) % albumYearStr.length() : 0;

	pthread_mutex_unlock(lock);
}

/*
 * data should be allocated already as a ROWS by COLS sized array
 */
void screenLayout::getContents(char** data) const
{
	pthread_mutex_lock(lock);

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			data[i][j] = screen[i][j];
		}
	}

	pthread_mutex_unlock(lock);
}

void screenLayout::clearScreen()
{
	pthread_mutex_lock(lock);

	for (int i = 0; i < ROWS; i++)
	{
		memset(screen[i], 0, COLS);
	}

	pthread_mutex_unlock(lock);
}

void screenLayout::pad(std::string* str, unsigned long len)
{
	unsigned long padLen = str->length() >= COLS ? (len - str->length()) + PADDING : COLS - str->length();

	for (unsigned long i = 0; i < padLen; i++)
	{
		str->append(" ");
	}
}
