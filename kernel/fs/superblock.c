#include "../include/fs.h"
#include "../include/string.h"


// I think that sb argument is useless... time (and code) will tell
void sbread(dev_t dev, struct superblock *sb) {

	struct buffer *b;

	acquire(mtable.sl);

	b = bread(dev, 1);
	memcpy(sb, b->b_data, sizeof(struct superblock));
	memcpy(&(mtable.superblock[dev]), sb, sizeof(struct superblock));
	brelease(b);

	release(mtable.sl);
}