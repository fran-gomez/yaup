#include "../include/fs.h"
#include "../include/sched.h"
#include "../include/locks.h"
#include "../include/string.h"

static struct inode *dir_lookup(struct inode *ip, char *entry, off_t esize);


struct inode *namei(char *path) {

	char *entry;
	struct inode *i;

	if (*path == '/') {
		i = &(ROOT_INODE);
		path++;
	} else
		i = CWD_INODE;
	
	entry = path;
	while (*path) {
		while (*(path++) != '/');
		i = dir_lookup(i, entry, (path-entry));
		if (!i)
			panic("namei: entry not found");
		entry = ++path;
	}

	if (!i)
		panic("namei: entry not found");
	
	return i;
}

static struct inode *dir_lookup(struct inode *ip, char *entry, off_t esize) {

	int i, j;
	struct buffer *b;
	struct dirent d;

	// Look for entry inside ip's entry list
	for (i = 0; i < NDIRECT; i++) {
		b = bread(ip->i_dev, ip->i_addr[i]);
		for (j = 0; j < BLK_SZ; j += DENT_PER_BLK) {
			memcpy(&d, &(b->b_data[j]), sizeof(struct dirent));
			if (strncmp(d.name, entry,esize) == 0)
				return iget(ip->i_dev, d.d_inode);
		}
		brelease(b);
	}

	return NULL;
//	panic("namei: indirect block not yet implemented");
}