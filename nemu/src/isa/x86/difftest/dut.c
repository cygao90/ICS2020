#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  // if (ref_r->eax != cpu.eax || ref_r->ecx != cpu.ecx || ref_r->edx != cpu.edx || 
  //     ref_r->ebx != cpu.ebx || ref_r->esp != cpu.esp || ref_r->ebp != cpu.ebp ||
  //     ref_r->esi != cpu.esi || ref_r->edi != cpu.edi || ref_r->pc != cpu.pc || 
  //     ref_r->EFLAGS.CF != cpu.EFLAGS.CF || ref_r->EFLAGS.ZF != cpu.EFLAGS.ZF ||
  //     ref_r->EFLAGS.SF != cpu.EFLAGS.SF || ref_r->EFLAGS.IF != cpu.EFLAGS.IF ||
  //     ref_r->EFLAGS.OF != cpu.EFLAGS.OF)
  //   return false;
  // return true;
  bool flag[15];
  flag[0]=difftest_check_reg("eax",pc,ref_r->eax,cpu.eax);
  flag[1]=difftest_check_reg("ebx",pc,ref_r->ebx,cpu.ebx);
  flag[2]=difftest_check_reg("ecx",pc,ref_r->ecx,cpu.ecx);
  flag[3]=difftest_check_reg("edx",pc,ref_r->edx,cpu.edx);
  flag[4]=difftest_check_reg("esp",pc,ref_r->esp,cpu.esp);
  flag[5]=difftest_check_reg("ebp",pc,ref_r->ebp,cpu.ebp);
  flag[6]=difftest_check_reg("esi",pc,ref_r->esi,cpu.esi);
  flag[7]=difftest_check_reg("edi",pc,ref_r->edi,cpu.edi);
  flag[8]=difftest_check_reg("pc",pc,ref_r->pc,cpu.pc);
  flag[9]=difftest_check_reg("CF",pc,ref_r->EFLAGS.CF,cpu.EFLAGS.CF);
  flag[10]=difftest_check_reg("ZF",pc,ref_r->EFLAGS.ZF,cpu.EFLAGS.ZF);
  flag[11]=difftest_check_reg("OF",pc,ref_r->EFLAGS.OF,cpu.EFLAGS.OF);
  flag[12]=difftest_check_reg("SF",pc,ref_r->EFLAGS.SF,cpu.EFLAGS.SF);
  flag[13]=difftest_check_reg("IF",pc,ref_r->EFLAGS.IF,cpu.EFLAGS.IF);
  for(int i = 0;i < 14;i++)
  {
    if(!flag[i]) return false;
  }
  return true;
}

void isa_difftest_attach() {
}
