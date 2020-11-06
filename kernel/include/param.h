#ifndef _PARAM_H
#define _PARAM_H

#define NR_PROC 150 /* # of process in ready queue */
#define WPROC 30  /* # of process in wait queue */
#define NR_CPU 4 /* # of cpus per system */

#define NR_FILE  10  /* # of max open files per process */
#define NR_INODE 150 /* # of cache'd inodes */
#define NR_SUPER 10  /* # of max mounted devices */
#define NR_BUF   NR_INODE /* # of max buffered blocks */

#define BLK_SZ   4096 /* Block size */
#define NDIRECT  9
#define IPB      (BLK_SZ/sizeof(struct d_inode)) /* Inodes per block */
#define DPB      (BLK_SZ/sizeof(struct dirent))  /* Dir entries per block */
#define MAX_PATH 127 /* Max path length */

#define PAGE_SIZE 4096 /* Page size in bytes */

#endif