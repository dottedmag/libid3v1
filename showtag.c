/*
 * libid3v1 (http://libid3v1.dottedmag.net/)
 *
 * Example program for libid3v1 - reads the ID3v1 tags form the command-line
 * arguments.
 *
 * Copyright (c) 2006,2007 Mikhail Gusarov <dottedmag@dottedmag.net>
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

#include "libid3v1.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int i;
    for (i = 1; i < argc; ++i) {
        int fh;
        id3v1_t tag;
        int res;
        fh = open(argv[i], O_RDONLY);
        if (fh == -1) {
            perror(argv[0]);
            continue;
        }
        res = id3v1_parse(fh, &tag);
        if (res == -1) {
            perror(argv[0]);
            if (-1 == close(fh))
                perror(argv[0]);
            continue;
        }

        if (-1 == close(fh)) {
            perror(argv[0]);
            continue;
        }
        
        if (res == 0) {
            printf("%s: No ID3V1 tag found\n", argv[i]);
        } else {
            printf("%s: ID3V1 tag found\n", argv[i]);
            printf("songname: %s\nartist: %s\n", tag.songname, tag.artist);
            printf("album: %s\nyear: %s\n", tag.album, tag.year);
            printf("comment: %s\n", tag.comment);
            if (tag.genre == -1) {
                printf("genre: *none*\n");
            } else {
                printf("genre: %s\n", id3v1_genre_name(tag.genre));
            }
            if (tag.track) {
                printf("track: %hd\n", tag.track);
            }
        }
    }
    return EXIT_SUCCESS;
}
