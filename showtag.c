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
