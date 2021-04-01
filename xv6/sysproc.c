#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ptentry.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// encrypt memory
int
sys_mencrypt(void)
{
  char *virtual_addr;
  int len;
  if (argptr(0, &virtual_addr, sizeof(virtual_addr)) < 0 || argint(1, &len) < 0) {
    return -1;
  }
  return mencrypt(virtual_addr, len);
}

// get statistics of memory
int
sys_getpgtable(void)
{
  // implement
  int num;
  struct proc *curproc = myproc();

  if (argint(1, &num) < 0) {
    // something is invalid
    return -1;
  }

  struct pt_entry entries[curproc->sz / PGSIZE];
  return getpgtable(entries, num);
}

// allows the user to dump the raw content of one physical page where physical_addr resides
int
sys_dump_rawphymem(void) {
  // TODO: implement
  return 0;
}