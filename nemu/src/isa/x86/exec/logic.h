#include "cc.h"

static inline def_EHelper(test) {
  // TODO();
  rtl_and(s, s0, ddest, dsrc1);
  rtl_set_OF(s, rz);
  rtl_set_CF(s, rz);
  rtl_update_ZFSF(s, s0, id_dest->width);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  // TODO();
  rtl_sext(s, t0, dsrc1, id_src1->width);
  rtl_and(s, ddest, ddest, t0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  // TODO();
  rtl_sext(s, t0, dsrc1, id_src1->width);
  rtl_xor(s, ddest, ddest, t0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  // TODO();
  rtl_sext(s, t0, dsrc1, id_src1->width);
  rtl_or(s, ddest, ddest, t0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(or);
}

static inline def_EHelper(not) {
  // TODO();
  rtl_not(s, ddest, ddest);
  operand_write(s, id_dest, ddest);
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_mv(s, s0, ddest);
  rtl_mv(s, s1, dsrc1);
  rtl_sar(s, ddest, s0, s1);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_mv(s, s0, ddest);
  rtl_mv(s, s1, dsrc1);
  rtl_shl(s, ddest, s0, s1);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_mv(s, s0, ddest);
  rtl_mv(s, s1, dsrc1);
  rtl_shr(s, ddest, s0, s1);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
