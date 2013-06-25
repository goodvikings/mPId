/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#ifndef SCREENLAYOUT_H
#define	SCREENLAYOUT_H

#include <pthread.h>
#include <string>

#define ROWS 4
#define COLS 20
#define PADDING 10

class screenLayout
{
public:
	screenLayout();
	~screenLayout();
	void setScreen(const char** data);
	void setContents(const char* title, const char* artist, const char* album, const char* year, const char* elapsed, const char* duration);
	void setTime(const char* elapsed, const char* duration);
	void getContents(char** data) const;
	void scrollScreen();
	void clearScreen();
private:
	void pad(std::string* str, unsigned long maxLen);

	char** screen;

	std::string titleStr;
	std::string artistStr;
	std::string albumYearStr;
	std::string timeStr;

	unsigned int titlePos;
	unsigned int artistPos;
	unsigned int albumYearPos;

	pthread_mutex_t* lock;
};

#endif	/* SCREENLAYOUT_H */

