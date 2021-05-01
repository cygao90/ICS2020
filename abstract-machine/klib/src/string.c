#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) { // tested
  const char* char_ptr = s;
  while (*char_ptr != '\0')
    char_ptr++;
  return char_ptr - s;
}

char *strcpy(char* dst,const char* src) { // tested
  return memcpy((void*)dst, (const void*)src, strlen(src) + 1);
}

char* strncpy(char* dst, const char* src, size_t n) { // tested
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; ++i) {
    dst[i] = src[i];
  }
  for (; i < n; ++i) {
    dst[i] = '\0';
  }
  return dst;
}

char* strcat(char* dst, const char* src) { // tested
  strcpy(dst + strlen(dst), src);
  return dst;
}

int strcmp(const char* s1, const char* s2) { //tested
  const char* p1 = s1;
  const char* p2 = s2;
  char a, b;
  do {
    a = *p1++;
    b = *p2++;
    if (a == '\0') {
      return a - b;
    }
  } while(a == b);
  return a - b;
}

int strncmp(const char* s1, const char* s2, size_t n) { // tested
  uint8_t c1 = '\0';
  uint8_t c2 = '\0';
  while (n > 0) {
    c1 = (uint8_t) *s1++;
    c2 = (uint8_t) *s2++;
    if (c1 == '\0' || c1 != c2) {
      return c1 - c2;
    }
    n--;
  }
  return c1 - c2;
}

void* memset(void* v,int c,size_t n) { // tested
  uint8_t* dst = (uint8_t*)v;
  while (n > 0) {
    dst[0] = c;
    dst++;
    n--;
  }
  return v;
}

void* memmove(void* dst,const void* src,size_t n) { // tested
  uint8_t* c1 = (uint8_t*)dst;
  uint8_t* c2 = (uint8_t*)src;
  while (n > 0) {
    *c1 = *c2;
    c1++;
    c2++;
    n--;
  }
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) { // tested
  uint8_t* dest = (uint8_t*)out;
  uint8_t* src = (uint8_t*)in;
  size_t i;
  for (i = 0; i < n; ++i, ++dest, ++src) {
    *dest = *src;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) { // tested
  uint8_t* a = (uint8_t*)s1;                           // I don't know why the result in macOS is different from the result in linux
  uint8_t* b = (uint8_t*)s2;
  int32_t a0, b0, res;
  while (n > 0) {
    a0 = ((uint8_t*) a)[0];
    b0 = ((uint8_t*) b)[0];
    a++;
    b++;
    res = a0 - b0;
    if (res != 0) {
      return res;
    }
    n--;
  }
  return 0;
}

#endif
