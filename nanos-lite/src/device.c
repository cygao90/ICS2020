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
  if (ev.keycode != AM_KEY_NONE) {
    if (ev.keydown) {
      sprintf(buf, "kd %s\n", keyname[ev.keycode]);
    } else {
      sprintf(buf, "ku %s\n", keyname[ev.keycode]);
    }
  }
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
  int x = offset % w;
  int y = offset / w;
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

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
