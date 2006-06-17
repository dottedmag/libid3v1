#ifndef LIBID3V1_H
#define LIBID3V1_H

/*
 * libid3v1 (http://libid3v1.dottedmag.net/)
 * 
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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This struct represents ID3v1 tag.
 *
 * missing genre is '-1' in .genre field
 * missing track number is '0' in .track field
 */

typedef struct _id3v1_t
{
    char songname[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    short genre;
    short track;
} id3v1_t;

/*
 * Parses the ID3v1 tag and fills the provided struct. Name of genre
 * may be obtained using id3v1_genre_name. If no track information
 * present, .track will be set to 0. No charset conversion is
 * performed, data is read as is.
 *
 * Tag is read from the end of fh provided.
 *
 * Returns
 * 1 if ID3v1 tag is present in file (filling the provided struct),
 * 0 if tag is not present,
 * -1 if error occured during read (check the errno for the reason)
 */

int id3v1_parse(int fh, id3v1_t* tag);

/*
 * Gets the genre name by the ID3v1 genre number. Returned string is
 * statically allocated and hence must not be freed by caller.
 *
 * Returns NULL if invalid number passed
 */

const char* id3v1_genre_name(short genre);

#ifdef __cplusplus
}
#endif

#endif
