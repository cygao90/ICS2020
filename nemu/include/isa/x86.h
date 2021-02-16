#ifndef __ISA_X86_H__
#define __ISA_X86_H__

#include <common.h>


// memory
#define x86_IMAGE_START 0x100000
#define x86_PMEM_BASE 0x0

// reg

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
  /*
  struct {
    uint32_t _32;
    uint16_t _16;
    uint8_t _8[2];
  } gpr[8];
  */
  struct {
    union {
      union {
        uint32_t _32;
        uint16_t _16;
        uint8_t _8[2];
      } gpr[8];
      struct {
        /* Do NOT change the order of the GPRs' definitions. */

        /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
         * in PA2 able to directly access these registers.
         */
        rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
      };
    };
  }; 

  vaddr_t pc;

  union {
    struct {
      uint32_t CF       : 1;
      uint32_t resv1    : 1;
      uint32_t PF       : 1;
      uint32_t resv2    : 1;
      uint32_t AF       : 1;
      uint32_t resv3    : 1;
      uint32_t ZF       : 1;
      uint32_t SF       : 1;
      uint32_t TF       : 1;
      uint32_t IF       : 1;
      uint32_t DF       : 1;
      uint32_t OF       : 1;
      uint32_t IOPL     : 2;
      uint32_t NT       : 1;
      uint32_t resv4    : 1;
      uint32_t RF       : 1;
      uint32_t VM       : 1;
      uint32_t resv5    : 14;
    };
    uint32_t val;
  } EFLAGS;

  struct {
    uint32_t CS;
    uint32_t SS;
    uint32_t DS;
    uint32_t ES;
    uint32_t FS;
    uint32_t GS;
  } SEGMENT_REG;

  struct {
    uint32_t base;
    uint16_t limit;
  } IDTR;

} x86_CPU_state;

// decode
typedef struct {
  bool is_operand_size_16;
  uint8_t ext_opcode;
  const rtlreg_t *mbase;
  rtlreg_t mbr;
  word_t moff;
} x86_ISADecodeInfo;

#define suffix_char(width) ((width) == 4 ? 'l' : ((width) == 1 ? 'b' : ((width) == 2 ? 'w' : '?')))
#define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
#define x86_has_mem_exception() (false)

#endif
