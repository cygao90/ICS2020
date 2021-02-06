#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (SCREEN_ADDR + 4)

static inline int min(int x, int y) { return (x < y) ? x : y; }

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint16_t w, h;
  h = inw(SCREEN_ADDR);
  w = inw(SCREEN_ADDR + 2);

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 0);
  }

  uint32_t *fb = (uint32_t*)(uintptr_t)FB_ADDR;
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  uint16_t max_h = inl(SCREEN_ADDR);
  uint16_t max_w = inl(SCREEN_ADDR + 2);
  int cp_bytes = sizeof(uint32_t) * min(w, max_w - x);
  // Copy the bytes to fb in row first.
  for (int i = 0; i < h && i + y < max_h; i ++) {
    memcpy(&fb[(y + i) * max_w + x], pixels, cp_bytes);
    // for (int j )
    pixels += w;
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
