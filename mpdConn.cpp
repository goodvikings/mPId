/*
 * ------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ramo@goodvikings.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return - Ramo
 * ------------------------------------------------------------------------------
 */

#include <mpd/client.h>
#include <pthread.h>
#include <string.h>
#include "mpdConn.h"

mpdConn::mpdConn(const char* address, const unsigned int port)
{
	lock = new pthread_mutex_t;
	pthread_mutex_init(lock, NULL);
	pthread_mutex_lock(lock);

	title = new char[MPD_CONN_STRING_LEN];
	artist = new char[MPD_CONN_STRING_LEN];
	album = new char[MPD_CONN_STRING_LEN];
	year = new char[MPD_CONN_STRING_LEN];
	elapsedTime = 0;
	duration = 0;
	id = -1;
	state = MPD_STATE_UNKNOWN;
	connected = false;

	conn = mpd_connection_new(address, port, 5000);

	if (mpd_connection_get_error(conn) == MPD_ERROR_SUCCESS)
	{
		connected = true;
	}

	pthread_mutex_unlock(lock);
}

mpdConn::~mpdConn()
{
	delete [] title;
	delete [] artist;
	delete [] album;
	delete [] year;

	pthread_mutex_destroy(lock);
	mpd_connection_free(conn);

	delete lock;
}

bool mpdConn::isConnected()
{
	return connected;
}

/*
 * Returns true if anything has changed
 */
bool mpdConn::updateTags()
{
	pthread_mutex_lock(lock);
	
	bool retVal = false;

	status = mpd_run_status(conn);
	song = mpd_run_current_song(conn);

	if (status == NULL)
	{
		connected = false;
		retVal = false;
	} else if (song != NULL)
	{	// copy across info
		strncpy(title, mpd_song_get_tag(song, MPD_TAG_TITLE, 0), MPD_CONN_STRING_LEN);
		strncpy(artist, mpd_song_get_tag(song, MPD_TAG_ARTIST, 0), MPD_CONN_STRING_LEN);
		strncpy(album, mpd_song_get_tag(song, MPD_TAG_ALBUM, 0), MPD_CONN_STRING_LEN);
		strncpy(year, mpd_song_get_tag(song, MPD_TAG_DATE, 0), MPD_CONN_STRING_LEN);

		elapsedTime = mpd_status_get_elapsed_time(status);
		duration = mpd_status_get_total_time(status);
		id = mpd_status_get_song_id(status);
	}
	
	state = mpd_status_get_state(status);
	retVal = true;

	if (song)
	{
		mpd_song_free(song);
	}
	if (status)
	{
		mpd_status_free(status);
	}
	
	pthread_mutex_unlock(lock);
	
	return retVal;
}

void mpdConn::getTitle(char* buff, unsigned int buffLen) const
{
	strncpy(buff, title, buffLen > MPD_CONN_STRING_LEN ? MPD_CONN_STRING_LEN : buffLen);
}

void mpdConn::getArtist(char* buff, unsigned int buffLen) const
{
	strncpy(buff, artist, buffLen > MPD_CONN_STRING_LEN ? MPD_CONN_STRING_LEN : buffLen);
}

void mpdConn::getAlbum(char* buff, unsigned int buffLen) const
{
	strncpy(buff, album, buffLen > MPD_CONN_STRING_LEN ? MPD_CONN_STRING_LEN : buffLen);
}

void mpdConn::getYear(char* buff, unsigned int buffLen) const
{
	strncpy(buff, year, buffLen > MPD_CONN_STRING_LEN ? MPD_CONN_STRING_LEN : buffLen);
}

unsigned int mpdConn::getElapsedTime() const
{
	return elapsedTime;
}

unsigned int mpdConn::getDuration() const
{
	return duration;
}

int mpdConn::getID() const
{
	return id;
}

mpd_state mpdConn::getState() const
{
	return state;
}
