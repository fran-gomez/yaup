#include <errno.h>

#include <yaup/fs.h>

struct buffer *create_buffer(struct inode *ino) {

    int i;
    struct buffer *head, *aux;

    if (!ino)
        return NULL;
    
    head = malloc(sizeof(struct buffer));
    if (!head)
        return -ENOMEM;
    
    // Initialize the base fields for the buffer entry
    head->b_ino = ino->i_num;
    head->b_dev = ino->i_dev;
    head->b_dirt = 0;
    head->b_lock = 0;
    head->b_cnt = 1;

    // Load the 5 direct blocks, the 
    // indirect, the double indirect
    // and the triple indirect


    return head;
}


/*
  Flush the whole buffer pointed by b
  back to disk and close the memory
  buffer
*/
int flush_buffer(struct buffer *b) {
  
}