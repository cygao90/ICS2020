#include <monitor/difftest.h>

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(lidt) {
  TODO();
  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();
  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();
  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(int) {
  TODO();
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  TODO();
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(in) {
  // TODO();
  rtl_li(s, s0, 0);
  switch (id_src1->width) {
    case 1: *s0 = pio_read_b(id_src1->val); break;
    case 2: *s0 = pio_read_w(id_src1->val); break;
    case 4: *s0 = pio_read_l(id_src1->val); break;
    default: panic("Invalid width");
  }
  operand_write(s, id_dest, s0);
  print_asm_template2(in);
}

static inline def_EHelper(out) {
  // TODO();
  switch (id_src1->width) {
    case 1: pio_write_b(id_dest->val, id_src1->val); break;
    case 2: pio_write_w(id_dest->val, id_src1->val); break;
    case 4: pio_write_l(id_dest->val, id_src1->val); break;
    default: panic("Invalid width");
  }
  print_asm_template2(out);
}
