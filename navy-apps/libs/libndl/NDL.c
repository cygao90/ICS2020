#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;
static int canvas_x = 0, canvas_y = 0;
static int fd_disp = 0;
static int fd_fb = 0;
static int fd_event = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  assert(fd_event != 0);
  int ret = read(fd_event, buf, len);
  if (ret == 0)
    return 0;
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] == '\n') {
      buf[i] = '\0';
      return 1;
    }
  }
  return 0;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  char buf[64];
  read(fd_disp, buf, sizeof(buf));
  sscanf(buf, "WIDTH:%d\nHEIGHT:%d", &screen_w, &screen_h);
  if (*w > screen_w || *h > screen_h) {
    printf("Canvas size out of screen.\n");
    assert(0);
  }
  if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  canvas_w = *w;
  canvas_h = *h;
  canvas_x = (screen_w - canvas_w) / 2;
  canvas_y = (screen_h - canvas_h) / 2; // 居中画布
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int sx = x + canvas_x;
  int sy = y + canvas_y;
  if (sx + w > canvas_x + canvas_w)
    w -= sx + w - (canvas_x + canvas_w);
  if (sy + h > canvas_y + canvas_h)
    h -= sy + h - (canvas_y + canvas_h);
  int start = 0;
  for (int i = 0; i < h; i++) {
    start = ((sy + i) * screen_w + sx) * 4;
    lseek(fd_fb, start, SEEK_SET);
    write(fd_fb, pixels + w * i, w * 4);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  char buf[64];
  fd_disp = open("/proc/dispinfo", 0, 0);
  fd_fb = open("/dev/fb", 0, 0);
  fd_event = open("/dev/events", 0, 0);
  return 0;
}

void NDL_Quit() {
  close(fd_disp);
  close(fd_fb);
}