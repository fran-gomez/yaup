#ifndef _FS_H
#define _FS_H

#include "locks.h"
#include "param.h"
#include "types.h"

/* Inode type bits */
#define IFMT  0170000
#define IFREG 0100000
#define IFDIR 0040000
#define IFCHR 0020000
#define IFBLK 0060000

/* Inode mode bits */
#define ISUID 04000
#define ISGID 02000
#define ISVTX 01000

#define IRWXU 0700
#define IRUSR 0400
#define IWUSR 0200
#define IXUSR 0100

#define IRWXG 070
#define IRGRP 040
#define IWGRP 020
#define IXGRP 010

#define IRWXO 07
#define IROTH 04
#define IWOTH 02
#define IXOTH 01

/* Inode flags */
#define ILCK  01  /* Inode is locked, bad planning here */
#define IDIRT 02  /* Inode has to be updated */
#define IACC  04  /* Update access time */
#define IMNT  010 /* Device mount point */
#define IWANT 020 /* Process waiting on lock */
#define ITEXT 040 /* IDK */

/* File flags */
#define FREAD 01
#define FWRT  02
#define FPIPE 04

/* Superblock flags */
#define FLOCK 01  /* Free blocks list lock */
#define ILOCK 02  /* Free inodes list lock */
#define SLOCK 04  /* Superblock update lock */
#define FMOD  010 /* Superblock modified */
#define RONLY 020 /* Read-only flag */

#ifndef NULL
	#define NULL ((void*)0)
#endif

#define MAX_NAME 25

#define ROOT_INODE icache.inodes[0] /* Yes, root's inode is hardcoded to position 0 */
#define CWD_INODE current_task()->p_cwd

typedef unsigned short bitmap;

/*
  Dir entry strcture. Each directory is conformed
  by a list of dir entries mapping the name by its
  inode
*/
struct dirent {
	ino_t d_inode;
	char name[MAX_NAME];
};

/*
  Inode structure as it appears on disk.
  Only used when getting an inode from disk
  to memory.
*/
struct d_inode {

	mode_t i_mode; /* RWX mode for UGA */
	u8_t  i_nlink; /* # of links to this inode */
	uid_t i_uid;   /* Owner's user ID */
	gid_t i_gid;   /* Owner's group ID */
	
	size_t i_size; /* Size in bytes */
	blk_t  i_addr[8]; /* Block addresses */

	time_t i_ctime; /* Creation time */
	time_t i_atime; /* Last access time */
	time_t i_mtime; /* Last modification time */
};

/*
  Main inode structure. There is only one strcutre
  for each file, directory or mounted device.
  An inode is distinguished by its (dev, inum) pair.
*/
struct inode {

	/* On disk fields */
	mode_t i_mode; /* RWX for UGA */
	u8_t  i_nlink; /* # of links to this inode */
	uid_t i_uid; /* Owner's user ID */
	gid_t i_gid; /* Owner's group ID */

	size_t i_size; /* Size of file in bytes */
	blk_t  i_addr[8]; /* Block addresses */

	time_t i_ctime; /* Creation time */
	time_t i_atime; /* Last access time */
	time_t i_mtime; /* Last modification time */

	/* In memory fields */
	ino_t i_num; /* Inode number */
	dev_t i_dev; /* Device number */
	u8_t  i_flag;  /* Inode flags */
	u8_t  i_count; /* # of procs using this inode */

	struct spinlock sl;
};

/*
  Buffer strcuture. Each open file has a buffer in
  memory containing its data. Each buffer is a linked
  list of blocks.
*/
struct buffer {
	byte b_data[BLK_SZ]; /* Pointer to data block */

	dev_t b_dev; /* Device number */
	blk_t b_blk; /* Block number */

	int  b_dirty; /* 0=clean, 1=dirty */
	u8_t b_count;  /* # of procs using this block */

	//struct buffer *b_prev; /* Pointer to previous block */
	//struct buffer *b_next; /* Pointer to next block */

	struct spinlock sl;
};

/*
  File structure for open/creat/pipe calls.
  Each process has its internal table of open files,
  being stdin, stdout and stderr common to all.
*/
struct file {

	mode_t f_mode; /* Open file mode (RWX) */
	u8_t  f_flag;
	u8_t  f_count; /* # of procs using this file */
	off_t f_off;   /* Cursor position in this file */
	struct inode *f_inode; /* File inode */

	struct spinlock sl;
};

/*
  File system super block. The root superblock
  is allocated when the file system is started,
  subsequently, a superblock is allocated when
  we mount a file system, and deallocated when
  we unmount the file system.
*/
struct superblock {

	/* On disk fields, read from partition's first block */
	u64_t s_magic; /* Magic number for xv8 FS */

	dev_t s_dev;   /* Device number */
	size_t s_size; /* Size in blocks of entire volume */

	blk_t s_ino_table; /* First block of inodes table */
	off_t s_ino_len;   /* Length of inodes table (in blocks) */

	blk_t s_free_blk;     /* First block of free blocks bitmap */
	off_t s_free_blk_len; /* Length of free blocks bitmap (in blocks) */

	blk_t s_free_ino;     /* First block of free inodes bitmap */
	off_t s_free_ino_len; /* Length of free inodes bitmap (in blocks) */

	u8_t s_flag; /* Superblock flags */

	time_t s_mtime; /* Date of last mount */
	time_t s_ctime; /* Date of creation */
	time_t s_atime; /* Date of last access */

	char s_mpoint[MAX_PATH]; /* Last mount point */

	struct spinlock sl;
};

/*
  This structure holds the requested information
  about a file
*/
struct stat {
	
	dev_t st_dev;  /* ID of device containing file */
	ino_t st_ino;  /* Inode number */

	mode_t st_mode;/* File type and mode */
	u8_t  st_nlink;/* Number of hard links */
	uid_t st_uid;  /* User ID of owner */
	gid_t st_gid;  /* Group ID of owner */

	dev_t st_rdev; /* Device ID (if special file) */
	
	off_t st_size; /* Total size, in bytes */
	u64_t st_blksize; /* Block size */
	u32_t st_blocks; /* # of blocks allocated */

	time_t st_atim; /* Time of last access */
	time_t st_mtim; /* Time of last modification */
	time_t st_ctim; /* Time of last status change */
};

/* File system internal tables */
struct {
	struct superblock superblock[NR_SUPER]; 
	struct spinlock sl;
} mtable; /* Mounted device table */

struct {
	struct inode inodes[NR_INODE];
	struct spinlock sl;
} icache; /* Inodes cache */

struct {
	struct buffer blocks[NR_BUF];
	struct spinlock sl;
} buffer; /* Buffered blocks table */

struct {
	struct file files[NR_FILES];
	struct spinlock sl;
} open_files; /* Open files */

/* Buffer ops */
struct buffer *bread(dev_t, blk_t);
int bwrite(struct buffer *);
int block(struct buffer *);
int brelease(struct buffer *);
struct buffer *balloc(dev_t);
int bfree(dev_t, blk_t);
//int bzeroo();

/* Inode ops */
struct inode *iget(dev_t, ino_t);
int iput(struct inode *);
int ilock(struct inode *);
int irelease(struct inode *);
struct inode *ialloc(dev_t);
int ifree(struct inode *);

struct inode *namei(char *);

/* Super block ops */
void sbread(dev_t, struct superblock *);
void sbwrite(struct superblock *);

#endif