#include <common.h>
#include "syscall.h"

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield: 
      yield(); 
      c->GPRx = 0;
      break;

    case SYS_exit: 
      halt(a[0]);  
      break;

    case SYS_write:
      c->GPRx = fs_write(c->GPR2, c->GPR3, c->GPR4);
      break;

    case SYS_brk:
      c->GPRx = 0;
      break;

    case SYS_open:
      c->GPRx = fs_open(c->GPR2, c->GPR3, c->GPR4);
      break;

    case SYS_read:
      c->GPRx = fs_read(c->GPR2, c->GPR3, c->GPR4);
      break;

    case SYS_lseek:
      c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
      break;

    case SYS_close:
      c->GPRx = fs_close(c->GPR2);
      break;

    case SYS_gettimeofday:  
      c->GPRx = timer(c->GPR2, c->GPR3);
      break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
