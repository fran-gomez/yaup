#include <errno.h>
#include <string.h>

#include <yaup/fs.h>

static char *next_dir_entry(char **path);

/* 
  Gets the inode asociated with
  the given path. If the path was
  previously addressed, it will be
  stored in the buffer, otherwise,
  we have to take it right from the disk
*/
struct inode *namei(const char *path) {

    int i;
    struct inode *ino;
    struct buffer *buf;
    struct dirent dent;
    char dir_ent[NAME_LEN];

    // Are we looking for an absolute
    // or relatve path?
    if (*path == '\0' || path == NULL)
        return NULL;
    else if (*path == '/') {
        ino = current->p_root;
        path++;
    } else if (*path)
        ino = current->p_cwd;
    
    // We have to look for every path's
    // component into the buffer, until
    // we reach the file pointed by path
    while (*path != '\0') {
        buf = NULL;
        memcpy(dir_ent, next_dir_entry(&path), NAME_LEN);

        for (i = HASH(ino->i_num); i < NR_BUF; i++)
            if (buffer_table[i]->b_ino == ino->i_num)
                buf = buffer_table[i];
    
        if (buf == NULL)
            buf = create_buffer(ino);
        
        for (i = 0; i < BLK_SZ; i += sizeof(struct dirent)) {
            memcpy(&dent, buf->b_data+i, sizeof(struct dirent));
            
            if (strncmp(dent.name, dir_ent, NAME_LEN) == 0) {
                iput(ino);
                flush_buffer(buf);
                ino = iget(ino->i_dev, dent.d_inode);
            } else 
                if (i >= BLK_SZ && buf->b_next) {
                    buf = buf->b_next;
                    i = 0;
                }
        }
    }

    // We haven't found the entry pointed to by path
    if (strncmp(dent.name, dir_ent, NAME_LEN) != 0)
        return NULL;

    return ino;
}

/*
  Put the inode pointed by i back into
  the inode's table on disk
*/
int iput(struct inode *i) {

    int hash;
    blk_t blk_addr;
    off_t blk_offset;
    struct buffer *block;
    struct super *sb;
    struct d_inode *di;

    // If there are more processes using this inode
    // just leave it in there
    if (--i->i_count > 0)
        return 0;

    // No more processes using the inode. take it away
    // from cache
    for (hash = HASH(i->i_num); hash < NR_INODE; hash++)
        if (inode_table[hash] != NULL)
            if (inode_table[hash]->i_num == i->i_num) {
                inode_table[hash] = NULL;
                break;
            }
    
    // If the inode was modified during its usage, update it on disk
    if (i->i_flag & IDIRT) {
        if (!(sb=super_block[i->i_dev]))
            return -ENODEV;

        blk_addr = (i->i_num / INODES_PER_BLOCK) + sb->s_ino_table;
        blk_offset = i->i_num % INODES_PER_BLOCK;

        di = malloc(sizeof(struct d_inode));
        di->i_mode = i->i_mode;
        di->i_nlink = i->i_nlink;
        di->i_uid = i->i_uid;
        di->i_gid = i->i_gid;

        di->i_size = i->i_size;
        memcpy(di->i_addr, i->i_addr, sizeof(blk_t)*8);

        di->i_ctime = i->i_ctime;
        di->i_atime = i->i_atime;
        di->i_mtime = i->i_mtime;

        if (!(block=bread(i->i_dev, blk_addr)))
            return -ENOTBLK;
        memcpy(block->b_data+blk_offset, di, sizeof(struct d_inode));
        if (!(bwrite(block)))
            return -EIO;
    }
}

/*
  Get the inode addressed by ino right from
  the dev's inodes table
*/
struct inode *iget(dev_t dev, ino_t ino) {

    blk_t blk_addr;
    off_t blk_offset;
    int hash;
    struct buffer *block;
    struct super *sb;
    struct inode *i;
    struct d_inode di;

    // First of all, we look in the inodes table for
    // the required inode. If its not present, then 
    // we get it from disk
    for (hash = HASH(ino); hash < NR_INODE; hash++) {
        if (inode_table[hash] != NULL)
            if (inode_table[hash]->i_num == ino) {
                inode_table[hash]->i_count++;
                return inode_table[hash];
            }
    }

    // Inode not present, we have to get it from disk
    if (!(sb = super_block[dev]))
        return -ENODEV;

    // Read the disk block containing the inode
    // and extract the inode data
    blk_addr = (ino / INODES_PER_BLOCK) + sb->s_ino_table;
    blk_offset = ino % INODES_PER_BLOCK;
    
    if (!(block = bread(dev, blk_addr)))
      return -ENOTBLK;
    
    memcpy(&di, block->b_data+blk_offset, sizeof(struct d_inode));

    // Copy the on disk inode into a memory inode
    i = malloc(sizeof(struct inode));
    i->i_num = ino;
    i->i_dev = dev;

    i->i_flag = 0;
    i->i_mode = di.i_mode;
    i->i_nlink = di.i_nlink;
    i->i_count = 1;

    i->i_uid = di.i_uid;
    i->i_gid = di.i_gid;

    i->i_size = di.i_size;
    memcpy(i->i_addr, di.i_addr, sizeof(blk_t)*8);

    i->i_ctime = di.i_ctime;
    i->i_mtime = di.i_mtime;
    i->i_atime = di.i_atime;

    return i;
}

/*
  Returns a new empty inode for the device
  addressed by dev
*/
struct inode *get_empty_inode(dev_t dev) {
    
    ino_t inum;
    struct super *sb;
    struct inode *ino;

    if (!(sb=getsuper(dev)))
        return -ENODEV;

    inum = get_free_pos(sb->s_free_ino_bmap);
    if (inum > 0)
        set_pos(sb->s_free_ino_bmap, inum);
    else
        return -ENOSPC;

    // Create the inode, and return it
    ino = malloc(sizeof(struct inode));
    ino->i_num = inum;
    ino->i_dev = dev;
    
    ino->i_flag = IDIRT | IACC;
    ino->i_mode = 0;
    ino->i_nlink = 1;
    ino->i_count = 1;

    ino->i_uid = current->p_uid;
    ino->i_gid = current->p_gid;

    ino->i_size = 0;
    memset(ino->i_addr, 0, 8*sizeof(blk_t));

    ino->i_ctime = time(NULL);
    ino->i_atime = ino->i_ctime;
    ino->i_mtime = ino->i_ctime;

    return ino;
}

/*
  ¿Frees an inode?
  Aca ves por que hay que documentar
  cuando diseñas, y no 46 meses despues...
  ¿Para que carajo puse esto?
*/
void free_inode(struct inode *i) {
    
}

/* Auxiliar functions */
static char *next_dir_entry(char **path) {

    int i = 0;
    char *entry;

    entry = malloc(sizeof(char)*NAME_LEN);
    while (**path != '/' || **path != '\0') {
        entry[i++] = **path;
        (*path)++;
    }

    if (**path == '/')
        (*path)++;
    
    return entry;
}