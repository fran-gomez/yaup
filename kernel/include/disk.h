#ifndef _DISK_H
#define _DISK_H

#include "fs.h"

void disk_read(struct buffer *b);
void disk_write(struct buffer *b);

#endif