#include "../include/fs.h"
#include "../include/locks.h"
#include "../include/string.h"
#include "../include/disk.h"

#define BTABLE_BASE(dev) mtable.superblock[dev].s_free_blk
#define BLK_ADDR(dev,blk) BTABLE_BASE(dev) + (blk / sizeof(bitmap))
#define BLK_OFF(blk) blk % sizeof(bitmap)

static struct buffer *bget(dev_t, blk_t);

void buffer_init(void) {

}

/*
  Reads the block addressed by blk from disk
  partition indexed by dev
*/
struct buffer *bread(dev_t dev, blk_t blk) {

	struct buffer *b;
	
	b = bget(dev, blk);
	if (b->b_count == 0) {
		disk_read(b);
		b->b_count++;
	}

	return b;
}

/*
  Write a buffered block back to disk
*/
int bwrite(struct buffer *b) {

	acquire(b->sl);

	if (!b->b_dirty) {
		release(b->sl);
		return 0;
	}
	
	disk_write(b);
	b->b_count--;
	// For security reasons, if the block is not used any more
	// remove its data from buffer
	if (b->b_count == 0) {
		b->b_blk = 0;
		b->b_dev = 0;
		memset(b->b_data, 0 ,BLK_SZ);
	}

	return 0;
}

/*int bzeroo(dev_t dev, blk_t blk) {

	struct buffer *b;

	b = bget(dev, blk);
	memset(b->b_data, 0, BLK_SZ);
	b->b_dirty = 1;
	bwrite(b);

	return 0;
}*/

int brelease(struct buffer *b) {
	 
	if (b->b_count <= 0)
		panic("brelease: unused buffer");
	b->b_count--;

	return 0;
}

// TODO: This is not even close to its final version
// it's still crap, it's just here as a scheme
int bfree(dev_t dev, blk_t blk) {

	struct buffer *b;

	b = bread(dev, BLK_ADDR(dev,blk));
	set_pos(b->b_data[BLK_OFF(blk)], BLK_OFF(blk));
	brelease(b);

	return 0;
}

static struct buffer *bget(dev_t dev, blk_t blk) {

	struct buffer *b;

	acquire(buffer.sl);

	for (b = buffer.head; b; b = b->b_next)
		if (b->b_dev == dev && b->b_blk == blk) {
			release(buffer.sl);
			return b;
		}
	
	for (b = buffer.head; b; b = b->b_next)
		if (b->b_count == 0) {
			b->b_blk = blk;
			b->b_dev = dev;
			b->b_dirty = 0;
			initlock(b->sl);
			release(buffer.sl);
			return b;
		}
	
	panic("bget: no buffer available");

	return NULL;
}