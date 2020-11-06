#ifndef _SCHED_H
#define _SCHED_H

#include "param.h"
#include "types.h"

#include "fs.h"
#include "mem.h"



/* State codes */
#define SSLEEP 1 /* Process sleeping, high priority */
#define SWAIT  2 /* Process sleeping, low priority */
#define SRUN   3 /* Process running */
#define SIDL   4 /* Process in creation */
#define SZOMB  5 /* Process in termination */
#define SSTOP  6 /* Process being stoped */

/* Flag codes */
#define SLOAD 01  /* Process is being loaded */
#define SSYS  02  /* Process current scheduling */
#define SLOCK 04  /* Process cannot be swaped out of cpu */
#define SSWAP 010 /* Process is being swapped out of cpu */
#define STRC  020 /* UNUSED */
#define SWTED 040 /* UNUSED */

/*
  Task state segment. Saves a snapshot
  of the current cpu internal state
  at a context sitch.
  Based on intel x86_64 architecture
*/
struct tss {
    // General purpose registers
    u64_t rax, rbx, rcx, rdx;
    u64_t rsi, rdi, rsp, rbp;
    u64_t r8, r9, r10, r11;
    u64_t r12, r13, r14, r15;

    u64_t rip; /* Instruction pointer */

    // Segment registers
    u16_t cs, ds, ss;
    u16_t es, fs, gs;

    u64_t rflags; /* Cpu flags */

    // Control registers
    u64_t cr0, cr2, cr4, cr8;

    // Debug registers
    u32_t dr0, dr1, dr2, dr3;
    u32_t dr6, dr7;
};

/*
  One proc structure is allocated per
  active process. Each cpu has its own
  process queue. User and system fields
  are maintained in this structure to
  simplify the programming
*/
struct proc {

    // Process related fields
    u8_t p_stat; /* Process status */
    u8_t p_flag; /* Process flags */

    u8_t p_pri;  /* Process priority, lower value is higher */
    u8_t p_nice; /* Process niceness */

    pid_t p_pid;  /* Process ID */
    pid_t p_ppid; /* Parent process ID */
    
    clock_t p_cpu;   /* Cpu ticks for scheduling */
    time_t  p_utime; /* Time used by user instructions */
    time_t  p_stime; /* Time used by sys instructions */

    // Security related fields
    uid_t p_euid; /* Effective user ID */
    uid_t p_suid; /* Saved user ID */
    uid_t p_uid;  /* Real user ID */
    
    gid_t p_egid;
    gid_t p_sgid;
    gid_t p_gid;

    // File system related fields
    mode_t p_umask; /* User permission mask */
    struct inode *p_cwd; /* Process current work dir */
    struct inode *p_root; /* Process root dir */
    struct file  *p_filp[NR_FILE]; /* Open files table */

    // Execution related fields
    long *p_textp; /* Pointer to text segment */
    long *p_datap; /* Pointer to data segment */
    long *p_heapp; /* Pointer to heap segment */

    struct tss *tss; /* Pointer to process state segment */
};

/* Scheduler internal tables */
extern struct proc *proc_table[NR_PROC];
extern struct proc *current;
extern time_t startup_time;

/* Scheduler operations */
void init_sched(void);
void init_traps(void);
void load_init(void);

void schedule(void);
void switch_task(pid_t n);

void panic(const char *str);

#endif