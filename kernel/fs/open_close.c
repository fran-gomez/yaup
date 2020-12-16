#include <errno.h>
#include <fcntl.h>

#include "../include/fs.h"
#include "../include/sched.h"

/**
 * @brief Open a file relative to a path
 * @param path  Path of the file
 * @param flags Opening mode (See fcntl.h for more info)
 * @param mode  RWX mode specification for a new file
 * @return File descriptor or a negative error
 */
int sys_open(const char *path, int flags, mode_t mode) {

    int fd;
    struct file *f;
    struct inode *i;
    struct proc *current;

    i = namei(path);
    if (!i && (flags & O_CREAT) == 0) 
        panic("sys_open: cannot get inode");
    
    if (flags & O_CREAT != 0) {
        
    }

    // Check if path points to a regular file
    if ((i->i_mode & IFMT) != IFREG) {
        iput(i);
        panic("sys_open: this is not a regular file");
    }

    // Verify that current user has permissions over file

    // If create was passed, allocate a new inode

    // Check if opening mode conforms file mode
    
    if (!(f = fget(i, flags))) {
        iput(i);
        panic("sys_open: cannot get file struct");
    }

    if ((fd = fdget()) < 0) {
        iput(i);
        f->f_count--;
        panic("sys_open: cannot get a valid fd");
    }

    if (!(current = current_task()))
        panic("sys_open: cannot get current task");

    acquire(current->sl);
    current->p_filp[fd] = f;
    release(current->sl);

    return EINPROGRESS;
}

/**
 * @brief Cierra el archivo referenciado por el descriptor
 * @param fd Descriptor de archivo
 * @return 0 si el cierre fue exitoso, < 0 caso contrario
 */
int sys_close(int fd) {

    struct file *f;

    if (fd > NR_OFILES || fd < 0)
        panic("sys_close: bad fd");

    f = current_task()->p_filp[fd];
    if (!f)
        panic("sys_close: bad file table");
    
    acquire(open_files.sl);

    if (--(f->f_count) == 0) {
        f->f_flag = 0;
        iput(f->f_inode);
        f->f_inode = NULL;
        f->f_mode = 0;
        f->f_off = 0;
        f->sl.lock = 0; // I know, this is brutal, i'll fix it later
    }

    release(open_files.sl);

    return 0;
}

static struct file *fget(struct inode *i, int flags) {

    struct file *f, *empty = NULL;

    acquire(open_files.sl);

    for (f = &(open_files.files[0]); f < &(open_files.files[NR_FILES]); f++)
        if (f->f_inode->i_num == i->i_num) {
            f->f_count++;
            release(open_files.sl);
            return f;
        } else if (!empty && f->f_count == 0)
            empty = f;

    if (!empty) {
        release(open_files.sl);
        return NULL;
    }

    empty->f_count++;
    empty->f_flag = flags;
    empty->f_inode = i;
    empty->f_mode = i->i_mode;
    empty->f_off = 0;
    initlock(empty->sl);

    release(open_files.sl);

    return empty;
}

static int fdget() {

    int fd;

    acquire(proc_table.sl);
    for (fd = 3; fd < NR_OFILES; fd++)
        if (!current_task()->p_filp[fd]) {
            release(proc_table.sl);
            return fd;
        }

    return -1;
}