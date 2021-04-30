#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  // TODO();
  uint32_t idt = cpu.IDTR.base;
  uint32_t lo = vaddr_read(idt + 8 * NO, 2);
  uint32_t hi = vaddr_read(idt + 8 * NO + 6, 2);

  cpu.EFLAGS.IF = 0;

  rtl_push(s, &cpu.EFLAGS.val);
  rtl_push(s, &cpu.SEGMENT_REG.CS);
  rtl_push(s, &ret_addr);
  rtl_j(s, (hi << 16) | lo);
}

void query_intr(DecodeExecState *s) {
  TODO();
}
