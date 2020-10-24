#ifndef _FS_H
#define _FS_H

#include <sys/types.h>

#include <yaup/sched.h>

#define NR_INODE 150 /* # of cache'd inodes */
#define NR_SUPER 5   /* # of max mounted devices */
#define NR_FILE  10  /* # of max open files */
#define NR_BUF   NR_INODE /* # of max buffered files */

#define BLK_SZ 4096 /* Block size */
#define INODES_PER_BLOCK      (BLK_SZ/sizeof(struct d_inode)) /* Inodes per block */
#define DIR_ENTRIES_PER_BLOCK (BLK_SZ/sizeof(struct dirent))  /* Dir entries per block */

#define NAME_LEN 25 /* Max name length */

#define HASH(x) x

/* Inode mode */
#define IFMT  0170000
#define     IFREG 0100000
#define     IFDIR 0040000
#define     IFCHR 0020000
#define     IFBLK 0060000
#define ISUID 04000
#define ISGID 02000
#define ISVTX 01000
#define IREAD 0400
#define IWRT  0200
#define IEXEC 0100

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

typedef unsigned short bitmap;

/*
  Dir entry strcture. Each directory is conformed
  by a list of dir entries mapping the name by its
  inode
*/
struct dirent {
    ino_t d_inode;
    char name[NAME_LEN];
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

    ino_t i_num; /* Inode number */
    dev_t i_dev; /* Device number */

    u8_t  i_flag;  /* Inode flags */
    mode_t i_mode; /* RWX for UGA */
    u8_t  i_nlink; /* # of links to this inode */
    u8_t  i_count; /* # of procs using this inode */

    uid_t i_uid; /* Owner's user ID */
    gid_t i_gid; /* Owner's group ID */

    size_t i_size; /* Size of file in bytes */
    blk_t  i_addr[8]; /* Block addresses */

    time_t i_ctime; /* Creation time */
    time_t i_atime; /* Last access time */
    time_t i_mtime; /* Last modification time */
};

/*
  Buffer strcuture. Each open file has a buffer in
  memory containing its data. Each buffer is a linked
  list of blocks.
*/
struct buffer {
    char *b_data; /* Pointer to data block (1024 bytes) */

    ino_t b_ino; /* Inode number */
    dev_t b_dev; /* Device number */
    blk_t b_blk; /* Block number */

    int  b_dirt; /* 0=clean, 1=dirty */
    int  b_lock; /* 0=ok, 1=locked */
    u8_t b_cnt;  /* # of procs using this block */

    struct buffer *b_prev; /* Pointer to previous block */
    struct buffer *b_next; /* Pointer to next block */
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
    struct inode *f_inode; /* Inode number */

    char *ptr; /* Read/Write pointer */
};

/*
  File system super block. The root superblock
  is allocated when the file system is started,
  subsequently, a superblock is allocated when
  we mount a file system, and deallocated when
  we unmount the file system.
*/
struct super {

    /* On disk fields, read from partition's first block */
    u64_t s_magic; /* Magic number for yaup FS */
    size_t s_size;/* Size in blocks of entire volume */

    blk_t s_ino_table; /* First block of inodes table */
    off_t s_ino_len;   /* Length of inodes table (in blocks) */

    blk_t s_free_bmap; /* First block of free resources bitmap */
    off_t s_free_len;  /* Length of free blocks and inodes bitmap (in blocks) */

    u8_t s_flag; /* Superblock flags */

    time_t s_mtime; /* Date of last mount */
    time_t s_ctime; /* Date of creation */
    time_t s_atime; /* Date of last access */

    char s_mpoint[50]; /* Last mount point */

    /* In memory fields */
    bitmap *s_free_blk_bmap; /* Free blocks bitmap */
    bitmap *s_free_ino_bmap; /* Free inodes bitmap */

    struct inode *s_mount_point; /* Inode of mount point. NYI */

    dev_t s_dev; /* Device number */
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
    u8_t st_blksize; /* Block size */
    u32_t st_blocks; /* # of blocks allocated */

    time_t st_atim; /* Time of last access */
    time_t st_mtim; /* Time of last modification */
    time_t st_ctim; /* Time of last status change */
};

/* File system internal tables */
extern struct super *super_block[NR_SUPER]; /* Mounted device table */
extern struct inode *inode_table[NR_INODE]; /* Inodes cache */
extern struct buffer *buffer_table[NR_BUF]; /* Buffered files table */

/* File system operations */
extern struct inode *namei(const char *path);
extern int iput(struct inode *i);
extern struct inode *iget(dev_t dev, ino_t ino);
extern struct inode *get_empty_inode(dev_t dev);
extern void free_inode(struct inode *i);

extern struct buffer *bread(dev_t dev, blk_t blk);
extern int bwrite(struct buffer *b);
extern struct buffer *get_empty_blk(dev_t dev);
extern void bfree(dev_t dev, blk_t blk);

extern struct buffer *create_buffer(struct inode *);
extern struct buffer *get_buffer(ino_t);
extern int flush_buffer(struct buffer *);

extern void mount_root(void);

extern struct super *getsuper(dev_t);

/* File system calls */
extern int open(const char *, mode_t);
extern int creat(const char *, mode_t);
extern int close(int );

extern int read(int, char *, size_t);
extern int write(int, const char *, size_t);
extern int seek(int, off_t, int);

extern int link(const char *, const char *);
extern int unlink(const char *);

extern int mount(dev_t dev, const char *path); // NYI
extern int umount(dev_t dev);                  // NYI

extern int rename(const char *, const char *);
extern int remove(const char *);

extern int mkdir(const char *, mode_t);

extern int sys_stat(const char *, struct stat *);
extern int sys_fstat(int, struct stat *);

#endif