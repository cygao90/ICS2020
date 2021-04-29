#include <common.h>
#include <sys/time.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  size_t count = 0;
  uint8_t *ptr = (uint8_t*)buf;
  while (count < len) {
    putch(*ptr);
    ptr++;
    count++;
  }
  return count;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  int count = 0;
  char buf1[64] = {0};
  if (ev.keycode != AM_KEY_NONE) {
    if (ev.keydown) {
      sprintf(buf1, "kd %s\n", keyname[ev.keycode]);
    } else {
      sprintf(buf1, "ku %s\n", keyname[ev.keycode]);
    }
  } else {
    // KEY_NONE
    sprintf(buf1, "NONE\n");
  }
  memcpy(buf, buf1, strlen(buf1));
  return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int width = io_read(AM_GPU_CONFIG).width;
  int height = io_read(AM_GPU_CONFIG).height;
  sprintf(buf, "WIDTH:%d\nHEIGHT:%d", width, height); // 约定没有空格
  return strlen(buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  offset /= sizeof(uint32_t); // 4 bytes per pixel
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  int x = offset % w;
  int y = offset / w;
  if (offset + len > w * h * sizeof(uint32_t))
    len = w * h * sizeof(uint32_t) - offset;
  io_write(AM_GPU_FBDRAW, x, y, buf, len / sizeof(uint32_t), 1, true); // one line

  return len;
}

int timer(struct timeval *tv, struct timezone *tz) {
  AM_TIMER_UPTIME_T uptime;
  uptime = io_read(AM_TIMER_UPTIME);
  int sec = tv->tv_sec = uptime.us / 1000000;
  tv->tv_usec = uptime.us - sec * 1000000;
  tz = NULL;
  return 0;
}

size_t fb_sync(const void *buf,size_t offset,size_t len)
{
  assert(((char*)buf)[0] == '1');
  //printf("refresh\n");
  assert(offset == 0);
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  return 1;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
