#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define MAX_BUF 1024

#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')

#define do_dec                                                                 \
  case 'd':                                                                    \
  case 'D':                                                                    \
    dec = va_arg(ap, int);                                                     \
    out += print_num(out, dec, 10, len, upper_case, zero_filling);             \
    break;

#define do_percent                                                             \
  case '%':                                                                    \
    *out++ = *buf;                                                             \
    break;                                                                     \

#define do_hex                                                                 \
  case 'X':                                                                    \
    upper_case = true;                                                         \
  case 'x':                                                                    \
    hex = va_arg(ap, int);                                                     \
    out += print_num(out, hex, 16, len, upper_case, zero_filling);             \
    break;                                                                     \

#define do_ch                                                                  \
  case 'c':                                                                    \
    _char = va_arg(ap, int);                                                   \
    out += print_char(out, _char, len);                                        \
    break;

#define do_str                                                                 \
  case 's':                                                                    \
    p_char = va_arg(ap, char *);                                               \
    out += print_str(out, p_char, len);                                        \
    break;                                                                     \

#define do_num                                                                 \
  case '0':                                                                    \
    zero_filling = true;                                                       \
    buf++;                                                                     \
  case '1':                                                                    \
  case '2':                                                                    \
  case '3':                                                                    \
  case '4':                                                                    \
  case '5':                                                                    \
  case '6':                                                                    \
  case '7':                                                                    \
  case '8':                                                                    \
  case '9':                                                                    \
    while (is_digit(*buf)) {                                                   \
      len = len * 10 + (*buf - '0');                                           \
      buf++;                                                                   \
    }                                                                          \
    --buf;                                                                     \
    goto again;

#define do_oct                                                                 \
  case 'o':                                                                    \
  case 'O':                                                                    \
    oct  = va_arg(ap, int);                                                    \
    out += print_num(out, oct, 8, len, upper_case, zero_filling);              \
    break;                                                                     \

#define CASE(func) do_##func break;

const char upper_digits[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lower_digits[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

static int print_num(char *buf, long long n, int base, int len, bool upper_case,
                     bool zero) {
  enum { MINUS, PLUS };
  int real_len = 0;
  int sym = n >= 0 ? PLUS : MINUS;
  n = n >= 0 ? n : -n;
  long long tmp = n;

  do {
    tmp /= base;
    real_len++;
  } while (tmp > 0);

  if (len <= real_len) {
    len = sym == MINUS ? real_len + 1 : real_len;
  } 

  char* workend = buf + len;
  const char* digits = upper_case ? upper_digits : lower_digits;

  do {
    *--workend = digits[n % base];
  } while ((n /= base) != 0);

  if (zero) { 
    while (workend != buf) {
      *--workend = '0';
    }
    if (sym == MINUS)
      *workend = '-';
  } else {
    if (sym == MINUS) {
      *--workend = '-';
    }
    while (workend != buf) {
      *--workend = ' ';
    }
  }

  return len;
}

static int print_str(char *buf, char* p_str, int len) {
  int str_len = strlen(p_str);
  len = str_len > len ? str_len : len;
  char* workend = buf + len;
  char* str_end = p_str + str_len;
  while (workend != buf && str_end != p_str) {
    *--workend = *--str_end;
  }
  while (workend != buf)
    *--workend = ' ';
  return len;
}

static int print_char(char *buf, char ch, int len) {
  len = len == 0 ? 1 : len;
  char* workend = buf + len;
  *--workend = ch;
  while (workend != buf) {
    *--workend = ' ';
  }
  return len;
}

int printf(const char *fmt, ...) {
  char buf[MAX_BUF];
  int res;
  va_list arg;

  va_start(arg, fmt);
  res = vsprintf(buf, fmt, arg);
  va_end(arg);
  putstr(buf);
  
  return res;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  const char *buf = fmt;
  char* tmp = out;

  while (*buf != '\0') {

    bool zero_filling = false;
    bool upper_case = false;
    char _char = 0;
    char *p_char = NULL;
    int len = 0;
    int dec = 0;
    unsigned int hex = 0;
    unsigned int oct = 0;

    switch (*buf) {
    case '%':
    again:
      switch (*++buf) {

        CASE(percent) // %%

        CASE(oct) // %[oO]

        CASE(dec) // %[dD]

        CASE(hex) // %[xX]

        CASE(ch) // %c

        CASE(str) // %s

        CASE(num)

      default:
        assert(0);
        break;
      }

      break;
    default:
      *out++ = *buf;
      break;
    }
    buf++;
  }
  *out = '\0';
  return out - tmp;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list arg;
  int done;

  va_start(arg, fmt);
  done = vsprintf(out, fmt, arg);
  va_end(arg);

  return done;
}

int snprintf(char *out, size_t n, const char *fmt, ...) { return 0; }

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) { return 0; }

#endif
