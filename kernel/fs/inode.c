#include "../include/fs.h"
#include "../include/locks.h"
#include "../include/string.h"

#include <errno.h>

#define ITABLE_BASE(dev) mtable.superblock[dev].s_ino_table
#define BLK_ADDR(dev, ino) ITABLE_BASE(dev) + (ino / IPB)
#define BLK_OFF(ino) ino % IPB

void inodes_init(void) {

}

struct inode *iget(dev_t dev, ino_t ino) {

	struct inode *i, *free = NULL;
	struct buffer *b;

	acquire(icache.sl);

	for (i = &(icache.table[0]); i != &(icache.table[NR_INODE]); i++)
		if (i->i_dev == dev && i->i_num == ino) {
			release(icache.sl);
			return i;
		} else if (i->i_count == 0 && !free)
			free = i;

	if (!free)
		panic("iget: no free inodes");
		
	// Inode not present in cache
	b = bread(dev, BLK_ADDR(dev,ino));
	memcpy(free, &(b->b_data[BLK_OFF(ino)]), sizeof(struct d_inode));
	free->i_num = ino;
	free->i_dev = dev;
	free->i_count++;
	initlock(free->sl);
	brelease(b);

	release(icache.sl);

	return free;
}

int iput(struct inode *i) {

	struct buffer *b;

	acquire(i->sl);

	if (i->i_flag & IDIRT) {
		b = bread(i->i_dev, BLK_ADDR(i->i_dev,i->i_num));
		memcpy(&(b->b_data[BLK_OFF(i->i_num)]), i, sizeof(struct d_inode));
		bwrite(b);
		i->i_flag &= !IDIRT;
		i->i_count--;
		release(i->sl);
	}

	return 0;
}

// As I will be running this on a SSD, there will be no
// periodic updates on disk
int iupdate(struct inode *i) {
	return EINPROGRESS;
}

int ifree(struct inode *i) {
	
	struct buffer *b;

	acquire(i->sl);

	// For security reasons, an unnused inode is set to zero
	b = bread(i->i_dev, BLK_ADDR(i->i_dev, i->i_num));
	memset(&(b->b_data[BLK_OFF(i->i_num)]), 0, sizeof(struct d_inode));
	bwrite(b);

	// Set the inode number as a free one

	// Mark the addressed blocks as free ones

	release(i->sl);

	return 0;
}