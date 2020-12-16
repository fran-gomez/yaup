#ifndef _PARAM_H
#define _PARAM_H

/* Scheduler constants */
#define NR_PROC 128 /* # of process in ready queue */
#define WPROC   64  /* # of process in wait queue */
#define NR_CPU  4   /* # of logical cpus per system */

/* File system constants */
#define NR_OFILES 128   /* # of max open files per process */
#define NR_INODE  1024  /* # of cache'd inodes */
#define NR_FILES  NR_INODE    /* # of max open files on the system */
#define NR_BUF    3*NR_INODE  /* # of max buffered blocks */
#define NR_SUPER  32   /* # of max mounted devices */

#define BLK_SZ    4096 /* Block size (in bytes) */
#define NDIRECT   9    /* # of direct address blocks */
#define INDIRECT  10   /* Indirect address block (521 blocks until here) */
#define DINDIRECT 11   /* Double indirect address block (262655 blocks until here) */
#define TINDIRECT 12   /* Triple indirect address block (like 13 million blocks here) */

#define INO_PER_BLK  (BLK_SZ/sizeof(struct d_inode)) /* Inodes per block */
#define DENT_PER_BLK (BLK_SZ/sizeof(struct dirent))  /* Dir entries per block */
#define BITS_PER_BLK (BLK_SZ*8)                      /* Bits per block */

#define MAX_PATH  127 /* Max path length */

/* Virtual memory constants */
#define PAGE_SIZE 4096 /* Page size in bytes */

#endif