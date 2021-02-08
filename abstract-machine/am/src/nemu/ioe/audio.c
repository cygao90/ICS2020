#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)
#define AUDIO_HEAD_ADDR      (AUDIO_ADDR + 0x18)
#define AUDIO_TAIL_ADDR      (AUDIO_ADDR + 0x1c)

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}

// Initialize the sound card according to ctrl
void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR, ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
  outl(AUDIO_INIT_ADDR, true);
  // *(uint32_t*)(BASE + AUDIO_FREQ_ADDR) = ctrl->freq;
  // *(uint32_t*)(BASE + AUDIO_CHANNELS_ADDR) = ctrl->channels;
  // *(uint32_t*)(BASE + AUDIO_SAMPLES_ADDR) = ctrl->samples;
  // uint32_t a = inl(AUDIO_FREQ_ADDR), b = inl(AUDIO_CHANNELS_ADDR), c = inl(AUDIO_SAMPLES_ADDR);
  // printf("%08x\n%08x\n%08x\n", a, b, c);
  // printf("\n%08x\n%08x\n%08x\n", ctrl->freq, ctrl->channels, ctrl->samples);
  // printf("\n%X\n", AUDIO_FREQ_ADDR);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

// This function is to write data into buffer
void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  int len = ctl->buf.end - ctl->buf.start;
  int sbuf_size = inl(AUDIO_SBUF_SIZE_ADDR);
  uint32_t count = inl(AUDIO_COUNT_ADDR), head = inl(AUDIO_HEAD_ADDR);//, tail = inl(AUDIO_TAIL_ADDR);
  uint32_t sbuf = (uint32_t)AUDIO_SBUF_ADDR;
  while(count + len > sbuf_size);

  int free = sbuf_size - count;
  int nwrite = len;
  if (free < len)
    nwrite = free;
  
  if (head + nwrite < sbuf_size) {
    // printf("%X\n", head + sbuf);
    memcpy(sbuf + head, ctl->buf.start, nwrite);
    head += nwrite;
  } else {
    int first_copy = sbuf_size - head;
    memcpy(sbuf + head, ctl->buf.start, first_copy);
    memcpy(sbuf, ctl->buf.start + first_copy, nwrite - first_copy);
    head = nwrite - first_copy;
  }
  count += nwrite;
  outl(AUDIO_HEAD_ADDR, head);
  // outl(AUDIO_TAIL_ADDR, tail);
  outl(AUDIO_COUNT_ADDR, count);
}
