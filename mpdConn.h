/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#ifndef MPDCONN_H
#define	MPDCONN_H

#include <mpd/client.h>
#include <pthread.h>

#define MPD_CONN_STRING_LEN 256
#define TIMELEN 8

class mpdConn
{
public:
	mpdConn(const char* address, const unsigned int port);
	~mpdConn();

	bool isConnected();
	bool updateTags();

	void getTitle(char* buff, unsigned int buffLen) const;
	void getArtist(char* buff, unsigned int buffLen) const;
	void getAlbum(char* buff, unsigned int buffLen) const;
	void getYear(char* buff, unsigned int buffLen) const;
	unsigned int getElapsedTime() const;
	unsigned int getDuration() const;
	int getID() const;
	mpd_state getState() const;
private:
	// Data held
	char* title;
	char* artist;
	char* album;
	char* year;
	unsigned int elapsedTime;
	unsigned int duration;
	int id;
	mpd_state state;

	// connection
	mpd_connection* conn;
	mpd_status* status;
	mpd_song* song;
	bool connected;

	// thread stuff
	pthread_mutex_t* lock;
};

#endif	/* MPDCONN_H */

