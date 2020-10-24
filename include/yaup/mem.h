#ifndef _MEM_H
#define _MEM_H

#include "../sys/types.h"

#define PAGE_SIZE 4096 /* Page size in bytes */

#ifndef NULL
#define NULL ((void*) 0)
#endif

/* Memory handler operations */
extern u64_t get_free_page(void);
extern u64_t put_page(u64_t page, u64_t addr);
extern void  free_page(u64_t addr);

extern int copy_page_tables(u64_t from, u64_t to, size_t size);
extern int free_page_tables(u64_t from, size_t size);

#endif