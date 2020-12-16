#include "../include/fs.h"
#include "../include/sched.h"

#include <errno.h>

int sys_stat(const char *path, struct stat *st) {
    
    struct inode *i;

    if (!(i = namei(path)))
        return -ENOENT;
    ilock(i);
    
    st->st_ino = i->i_num;
    st->st_dev = i->i_dev;

    st->st_mode = i->i_mode;
    st->st_nlink = i->i_nlink;

    st->st_uid = i->i_uid;
    st->st_gid = i->i_gid;

    st->st_rdev = 0;

    st->st_size = i->i_size;
    st->st_blksize = BLK_SZ;
    st->st_blocks = i->i_size / 1024;

    st->st_atim = i->i_atime;
    st->st_mtim = i->i_mtime;
    st->st_ctim = i->i_ctime;

    iput(i);
    irelease(i);

    return 0;
}

int sys_fstat(int fd, struct stat *st) {

    struct file *f;
    struct inode *i;

    if (fd >= NR_OFILES)
        return -EBADFD;

    if (!(f = current_task()->p_filp[fd]))
        return -ENOENT;

    if (!(i = f->f_inode))
        panic("sys_fstat: cannot get inode");
    ilock(i);
    
    st->st_ino = i->i_num;
    st->st_dev = i->i_dev;

    st->st_mode = i->i_mode;
    st->st_nlink = i->i_nlink;

    st->st_uid = i->i_uid;
    st->st_gid = i->i_gid;

    st->st_rdev = 0;

    st->st_size = i->i_size;
    st->st_blksize = BLK_SZ;
    st->st_blocks = i->i_size / 512*1024;

    st->st_atim = i->i_atime;
    st->st_mtim = i->i_mtime;
    st->st_ctim = i->i_ctime;

    irelease(i);

    return 0;
}