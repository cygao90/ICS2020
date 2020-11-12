#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define CASE(sym, func) case sym: do_##func break;

static int print_num(char* buf, int n, int base, int len, bool upper_case) {
  int real_len = 0;
  int tmp = n;

  do {
    tmp /= base;
    real_len++;
  } while (tmp > 0);

  if (len < real_len)
    len = real_len;

  int i;
  for (i = len - 1; i >= 0; --i, n /= base) {
    tmp = n % base;
    if (tmp > 9) {
      if (upper_case) {
        buf[i] = tmp - 10 + 'A';
      } else {
        buf[i] = tmp - 10 + 'a';
      }
    } else {
      buf[i] = tmp + '0';
    }
  }
  return len;
}

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  const char *buf = fmt;

  while (*buf != '\0') {
    bool upper_case;
    int _int = 0;
    char _char = 0;
    char* p_char = NULL; 
    int len;

    switch (*buf) {
      case '%':
        switch (*++buf) {
          case '%':
            *out++ = *buf;
            break;

          case 'd':
          case 'D':
            upper_case = false;
            _int = va_arg(ap, int);
            if (_int == 0) {
              *out++ = '0';
            } else {
              out += print_num(out, _int, 10, 0, upper_case);
            }
            break;

          case 'X':
            upper_case = true;
          case 'x':
            upper_case = false;
            _int = va_arg(ap, int);
            if (_int == 0) {
              *out++ = '0';
            } else {
              out += print_num(out, _int, 16, 0, upper_case);
            }
            break;

          case 'c':
            _char = va_arg(ap, int);
            *out++ = _char;
            break;
          case 's':
            p_char = va_arg(ap, char*);
            while (*p_char != '\0') {
              *out++ = *p_char++;
            }
            break;

          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:

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
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list arg;
  int done;

  va_start(arg, fmt);
  done = vsprintf(out, fmt, arg);
  va_end(arg);
  
  return done;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
