#include "../include/fs.h"

#include <sys/types.h>
#include <errno.h>

off_t get_free_pos(bitmap *b) {

    int i, j;
    off_t off;

    // Find an entry with at least one free bit(1)
    while (b)
        if (*b == 0) {
            b++;
            i += sizeof(*b);
        }
    
    if (b == NULL)
        return -ENOSPC;

    // Find the position of the free bit in its entry
    for (j = 0; j < sizeof(*b)*8; j++)
        if ((*b & 1 << j) != 0)
            off = i + j;
    
    return off;
}

void set_pos(bitmap *b, off_t pos) {

    int i = pos / sizeof(*b)*8;
    int j = pos % sizeof(*b)*8;

    *(b+i) |= (1 << j);
}

// TODO Check this expression, I think its pure crap
void unset_pos(bitmap *b, off_t pos) {
    int i = pos / sizeof(*b)*8;
    int j = pos % sizeof(*b)*8;

    *(b+i) |= ~(1 << j);
}