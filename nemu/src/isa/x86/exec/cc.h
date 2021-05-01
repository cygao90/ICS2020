#ifndef __CC_H__
#define __CC_H__

#include "../local-include/rtl.h"

enum {
  CC_O, CC_NO, CC_B,  CC_NB,
  CC_E, CC_NE, CC_BE, CC_NBE,
  CC_S, CC_NS, CC_P,  CC_NP,
  CC_L, CC_NL, CC_LE, CC_NLE
};


/* Condition Code */

static inline const char* get_cc_name(int subcode) {
  static const char *cc_name[] = {
    "o", "no", "b", "nb",
    "e", "ne", "be", "nbe",
    "s", "ns", "p", "np",
    "l", "nl", "le", "nle"
  };
  return cc_name[subcode];
}

static inline void rtl_setcc(DecodeExecState *s, rtlreg_t* dest, uint32_t subcode) {
  uint32_t invert = subcode & 0x1;

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O: rtl_li(s, dest, eflag(OF) == 1 ? 1 : 0); break;
    case CC_B: rtl_li(s, dest, eflag(CF) == 1 ? 1 : 0); break;
    case CC_E: rtl_li(s, dest, eflag(ZF) == 1 ? 1 : 0); break;
    case CC_BE: rtl_li(s, dest, (eflag(CF) == 1 || eflag(ZF) == 1) ? 1 : 0); break;
    case CC_S: rtl_li(s, dest, eflag(SF) == 1 ? 1 : 0); break;
    case CC_L: rtl_li(s, dest, eflag(SF) != eflag(OF) ? 1 : 0); break;
    case CC_LE: rtl_li(s, dest, (eflag(SF) != eflag(OF) || eflag(ZF) == 1) == 1 ? 1 : 0); break;
       // TODO();
    default: panic("should not reach here");
    case CC_P: panic("PF is not supported");
  }

  if (invert) {
    rtl_xori(s, dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}

#endif
