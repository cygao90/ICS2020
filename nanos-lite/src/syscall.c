#include <common.h>
#include "syscall.h"
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
    {
      int ret = -1;
      int count = 0;
      uint8_t *buf = c->GPR3;
      int max_len = c->GPR4;
      if (c->GPR2 == 1 || c->GPR2 == 2) {
        while (count < max_len) {
          putch(*buf);
          buf++;
          count++;
        }
        ret = count;
      }
      c->GPRx = ret;
    }
    Log("one write\n");
    break;

    case SYS_brk:
      c->GPRx = 0;
      break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
