#include <fcntl.h>
#include <string.h>

#include "../include/fs.h"
#include "../include/sched.h"

/**
 * @brief Lee un flujo de caracteres del archivo
 * @param fd Descriptor del archivo a leer
 * @param buf Buffer que contiene el resultado de la lectura
 * @param buffsize Tamaño maximo del buffer
 * @return Cantidad de bytes leidos
 */
int sys_read(int fd, char *buf, size_t buffsize) {
    
    int i, count;
    struct file *f;
    struct buffer *b;
    struct proc *current;

    // Ajuste del tamaño del buffer para evitar leer
    // mas alla del fin del archivo
    if (f->f_off + buffsize >= f->f_inode->i_size)
        buffsize = f->f_inode->i_size - f->f_off;

    if (fd >= NR_OFILES)
        panic("sys_read: invalid fd");

    if (!(current = current_task()))
        panic("sys_read: cannot get current task");

    acquire(current->sl);

    if (!(f = current->p_filp[fd]))
        panic("sys_read: invalid fd");
    
    // Se verifica que el proceso tenga permisos
    // de lectura sobre el archivo (user, group, others)
    if (!user_has_perm(i, IRUSR))
        if (!group_has_perm(i, IRGRP))
            if (!other_has_perm(i, IROTH))
                panic("sys_read: cannot read file (permission denied)");

    release(current->sl);

    // Si el archivo no fue abierto para lectura, falla
    if (f->f_flag & FREAD == 0)
        panic("sys_write: non readable file");
    
    // Se obtiene el bloque de datos correspondiente al
    // cursor del archivo
    if (!(b = bread(f->f_inode->i_dev, baddr(f))))
        panic("sys_read: cannot get block");
    
    // Lectura propiamente dicha
    count = 0;
    i = f->f_off % BLK_SZ;
    while (count < buffsize) {
        buf[count++] = b->b_data[i++];
        f->f_off++;

        // Si se llega al fin del bloque, y hay que
        // continuar la lectura, se obtiene el proximo bloque
        if (i == BLK_SZ) {
            brelease(b);
            if (!(b = bread(f->f_inode->i_dev, baddr(f))))
                panic("sys_read: cannot get block");
            i = f->f_off % BLK_SZ;
        }
    }

    return count;
}

/**
 * @brief Escribe en un archivo
 * @param fd Descriptor del archivo a escribir
 * @param buf Buffer que contiene los bytes a escribir
 * @param buffsize Tamaño maximo del buffer
 * @return Cantidad de caracteres escritos
 */
int sys_write(int fd, const char *buf, size_t buffsize) {

    int count, i;
    struct file *f;
    struct buffer *b;
    struct proc *current;

    if (fd >= NR_OFILES)
        panic("sys_write: bad fd");

    if (!(current = current_task()))
        panic("sys_write: cannot get current task");

    acquire(current->sl);

    if (!(f = current->p_filp[fd]))
        panic("sys_write: bad fd");
    
    // Se verifica que el proceso tenga permisos
    // de lectura sobre el archivo (user, group, others)
    if (!user_has_perm(i, IWUSR))
        if (!group_has_perm(i, IWGRP))
            if (!other_has_perm(i, IWOTH))
                panic("sys_read: cannot write file (permission denied)");
    
    release(current->sl);

    // Verificacion de modo de apertura
    if (f->f_flag & FWRT == 0)
        panic("sys_write: non writeable file");

    // Se obtiene el bloque de datos correspondiente
    // al cursor del archivo
    if (!(b = bread(f->f_inode->i_dev, baddr(f))))
        panic("sys_write: cannot get block");
    
    // Se escribe el buffer en el archivo
    count = 0;
    i = f->f_off % BLK_SZ;
    while (count < buffsize) {
        b->b_data[i++] = buf[count++];
        f->f_off++;

        // Si se lleno el bloque actual, se solicita uno nuevo
        if (i == BLK_SZ) {
            b->b_dirty = 1;
            brelease(b);
            if (!(b = balloc(f->f_inode->i_dev)))
                panic("sys_write: cannot get a new block");
            i = f->f_off % BLK_SZ;
        }
    }

    return count;
}

/**
 * @brief Desplaza el cursor de lectura/escritura del archivo
 * @param fd Descriptor del archivo a reposicionar
 * @param offset Offset del desplazamiento
 * @param whence Posicion de referencia para el desplazamiento
 * @return Cantidad de bytes desplazados
 */
off_t sys_seek(int fd, off_t offset, int whence) {

    off_t off;
    struct file *f;
    struct proc *current;

    if (fd >= NR_OFILES)
        panic("sys_seek: bad fd");

    if (!(current = current_task()))
        panic("sys_seek: cannot get current task");

    acquire(current->sl);

    if (!(f = current->p_filp[fd]))
        panic("sys_seek: invalid fd");
    
    release(current->sl);
    
    switch (whence) {
        case SEEK_CUR:
            off = f->f_off + offset;
            if (off <= 0 || off >= f->f_inode->i_size)
                panic("sys_seek: bad offset");

            f->f_off = off;
            break;
        case SEEK_END:
            off = f->f_off - offset;
            if (off <= 0 || off >= f->f_inode->i_size)
                panic("sys_seek: bad offset");

            f->f_off = off;
            break;
        case SEEK_SET:
            off = offset;
            if (off <= 0 || off >= f->f_inode->i_size)
                panic("sys_seek: bad offset");

            f->f_off = off;
            break;
        default:
            panic("sys_seek: invalid reference position");
    }

    return off;
}

// Devuelve la direccion del bloque correspondiente
// a la posicion actual de lectura/escritura
static blk_t baddr(struct file *f) {

    int addr;
    blk_t blk;
    
    addr = f->f_off/BLK_SZ;
    if (addr > NDIRECT)
        return -1;
    else
        blk = f->f_inode->i_addr[addr];

    return blk;
}

static int user_has_perm(struct inode *i, int perm) {

    mode_t mode = i->i_mode;
    uid_t  user = i->i_uid;

    if (current_task()->p_uid == user)
        if ((mode & IRWXU) != 0 || (mode & perm) != 0)
            return 1;
    
    return 0;
}

static int group_has_perm(struct inode *i, int perm) {

    mode_t mode = i->i_mode;
    uid_t  user = i->i_uid;

    if (current_task()->p_gid == i->i_gid)
        if ((mode & IRWXG) != 0 || (mode & perm) != 0)
            return 1;
    
    return 0;
}

static int other_has_perm(struct inode *i, int perm) {

    mode_t mode = i->i_mode;

    return (mode & IRWXO) != 0 ||
            (mode & perm) != 0;
}