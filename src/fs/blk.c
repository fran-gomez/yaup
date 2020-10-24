#include <yaup/fs.h>


/*
  Reads the block addressed by blk from disk
  partition indexed by dev
*/
struct buffer *bread(dev_t dev, blk_t blk) {

}

/*
  Write a buffered block back to disk, without
  removing it from memory
*/
int bwrite(struct buffer *b) {

}

/*
  Gets a new empty block from the partition
  indexed by dev
*/
struct buffer *get_empty_blk(dev_t dev) {

}

/*
  Marks the block addressed by blk as free
  in the partition indexed by dev
*/
void bfree(dev_t dev, blk_t blk) {

}
