#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  //char tmp[64], event1[3], event2[20];
  int fd = open("/dev/events", 0, 0);
  read(fd, buf, len); // len is not used
  // sscanf(buf, "%s %s\n", event1, event2);
  // if (event1[0] == 'k' && (event1[1] == 'u' || event1[1] == 'd'))
  // does not judge its validity for now
  close(fd);
  return 1;
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
  if (*w > screen_w || *h > screen_h) {
    printf("Canvas size out of screen.\n");
    return;
  }
  if (*w == 0 && *h == 0) {
    *w == screen_w;
    *h = screen_h;
  }
  printf("WIDTH:%d\n", *w);
  printf("HEIGHT:%d\n", *h);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb", 0, 0);
  // screen_h = 800;
  // screen_w = 600;
  lseek(fd, (x * screen_h + screen_w) * sizeof(uint32_t), SEEK_SET);
  // write(fd, pixels, w * h * sizeof(uint32_t));

  // Write each line to the corresponding position
  int i, len = 0;
  for (i = 0; i < h; i++) {
    printf("len: %d\n", len);
    len += (write(fd, pixels + len, w * sizeof(uint32_t))) / sizeof(uint32_t);
    lseek(fd, ((x + i) * screen_w + y) * sizeof(uint32_t), SEEK_SET);
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
  int fd = open("/proc/dispinfo", 0, 0);
  read(fd, buf, sizeof(buf));
  sscanf(buf, "WIDTH:%d\nHEIGHT:%d", &screen_w, &screen_h);
  close(fd);
  return 0;
}

void NDL_Quit() {
}
