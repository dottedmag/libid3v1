/*
 * Copyright (c) 2006 Mikhail Gusarov <dottedmag@dottedmag.net>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "libid3v1.h"

#define ID3V1_TAG_LENGTH 128
#define ID3V1_HEADER_LENGTH 3
#define ID3V1_HEADER "TAG"

#include <stdio.h>

int read_all(int fh, char* buf, size_t size)
{
    char* curr = buf;

    while(size > 0) {
        ssize_t read_ = read(fh, buf, size);
        if (read_ == -1 && (errno == EAGAIN || errno == EINTR)) continue;
        if (read_ == -1) return 0;
        if (read_ == 0) break;
        buf += read_;
        size -= read_;
    }
    return size == 0;
}

int id3v1_parse(int fh, id3v1_t* tag)
{
  char rawtag[ID3V1_TAG_LENGTH];

  if ((off_t)-1 == lseek(fh, (off_t)-128, SEEK_END)) return -1;
  if (!read_all(fh, rawtag, ID3V1_TAG_LENGTH)) return -1;
  if (memcmp(ID3V1_HEADER, rawtag, ID3V1_HEADER_LENGTH)) return 0;

  memset(tag, 0, sizeof(id3v1_t));
  memcpy(tag->songname, rawtag + 3, 30);
  memcpy(tag->artist, rawtag + 33, 30);
  memcpy(tag->album, rawtag + 63, 30);
  memcpy(tag->year, rawtag + 93, 4);
  memcpy(tag->comment, rawtag + 97, 30);
  tag->genre = rawtag[127];

  if (tag->comment[28] == 0) {
    tag->track = tag->comment[29];
    tag->comment[29] = 0;
  }

  return 1;
}

static char* genres[] = {
  "Unknown", "Blues", "Classic Rock", "Country", "Dance", "Disco",
  "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other",
  "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative",
  "Ska", "Death Metal", "Pranks", "Soundtrack", "Euro-Techno", "Ambient",
  "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical",
  "Instrumental", "Acid", "House", "Game", "Sound Clip", "Gospel", "Noise",
  "AlternRock", "Bass", "Soul", "Punk", "Space", "Meditative",
  "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic", "Darkwave",
  "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream",
  "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap",
  "Pop/Funk", "Jungle", "Native American", "Cabaret", "New Wave", "Psychedelic",
  "Rave", "Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz",
  "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk", "Folk/Rock",
  "National Folk", "Swing", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass",
  "Avantgarde", "Gothic Rock", "Progressive Rock", "Psychedelic Rock",
  "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening",
  "Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber Music", "Sonata",
  "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam",
  "Club", "Tango", "Samba", "Folklore"
};

const char* id3v1_get_genre(short genre)
{
  if (genre < 0 || genre >= sizeof(genres) / sizeof(genres[0])) return NULL;
  return genres[genre];
}
