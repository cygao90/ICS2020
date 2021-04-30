static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  // TODO();
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(push_esp) {
  // TODO();
  rtl_mv(s, s0, ddest);
  rtl_push(s, s0);
  print_asm_template1(push);
}

static inline def_EHelper(push_ib) {
  rtl_sext(s, ddest, ddest, id_dest->width);
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  // TODO();
  rtl_pop(s, ddest);
  operand_write(s, id_dest, ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pop_esp) {
  rtl_lm(s, ddest, &reg_l(R_ESP), 0, 4);
  operand_write(s, id_dest, ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  // TODO();
  rtl_mv(s, t0, &reg_l(R_ESP));
  rtl_push(s, &reg_l(R_EAX));
  rtl_push(s, &reg_l(R_ECX));
  rtl_push(s, &reg_l(R_EDX));
  rtl_push(s, &reg_l(R_EBX));
  rtl_push(s, t0);
  rtl_push(s, &reg_l(R_EBP));
  rtl_push(s, &reg_l(R_ESI));
  rtl_push(s, &reg_l(R_EDI));
  print_asm("pusha");
}

static inline def_EHelper(popa) {
  // TODO();
  rtl_pop(s, &reg_l(R_EDI));
  rtl_pop(s, &reg_l(R_ESI));
  rtl_pop(s, &reg_l(R_EBP));
  rtl_pop(s, t0);
  rtl_pop(s, &reg_l(R_EBX));
  rtl_pop(s, &reg_l(R_EDX));
  rtl_pop(s, &reg_l(R_ECX));
  rtl_pop(s, &reg_l(R_EAX));
  print_asm("popa");
}

static inline def_EHelper(leave) {
  // TODO();
  rtl_mv(s, &reg_l(R_ESP), &reg_l(R_EBP));
  rtl_pop(s, &reg_l(R_EBP));
  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
    // TODO();
    rtl_lr(s, s0, R_AX, 2);
    if ((int16_t)(*s0) < 0) {
      rtl_li(s, s1, 0xFFFF);
      rtl_sr(s, R_DX, s1, 2);
    } else {
      rtl_li(s, s1, 0);
      rtl_sr(s, R_DX, s1, 2);
    }
  } else {
    // TODO();
    rtl_lr(s, s0, R_EAX, 4);
    if ((int32_t)(*s0) < 0) {
      rtl_li(s, s1, 0xFFFFFFFF);
      rtl_sr(s, R_EDX, s1, 4);
    } else {
      rtl_li(s, s1, 0);
      rtl_sr(s, R_EDX, s1, 4);
    }
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    // TODO();
    rtl_lr(s, s0, R_AL, 1);
    rtl_sext(s, s0, s0, 1);
    rtl_sr(s, R_AX, s0, 2);
  } else {
    // TODO();
    rtl_lr(s, s0, R_AX, 2);
    rtl_sext(s, s0, s0, 2);
    rtl_sr(s, R_EAX, s0, 4);
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}

static inline def_EHelper(movsb) {
  rtl_lm(s, s0, dsrc1, 0, 1);
  rtl_sm(s, ddest, 0, s0, 1); 
  rtl_addi(s, ddest, ddest, 1); 
  rtl_addi(s, dsrc1, dsrc1, 1);
  operand_write(s, id_dest, ddest);
  operand_write(s, id_src1, dsrc1);
  print_asm_template1(movs);
}

static inline def_EHelper(movs) {
  rtl_lm(s, s0, dsrc1, 0, id_src1->width);
  rtl_sm(s, ddest, 0, s0, id_dest->width); 
  rtl_addi(s, ddest, ddest, id_dest->width); 
  rtl_addi(s, dsrc1, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  operand_write(s, id_src1, dsrc1);
  print_asm_template1(movs);
}
