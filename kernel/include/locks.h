#ifndef _LOCKS_H
#define _LOCKS_H

#include "types.h"

/* Mutual exclusion lock (Busy waiting) */
struct spinlock {
    u8_t lock;
};

/* Mutual exclusion lock (Sleep waiting) */
struct sleeplock {
    u8_t lock;
    struct spinlock slk;
};

extern void initlock(struct spinlock);
extern void acquire(struct spinlock);
extern void release(struct spinlock);

extern void init_sleeplock(struct sleeplock);
extern void sleep_acquire(struct sleeplock);
extern void sleep_release(struct sleeplock);

#endif