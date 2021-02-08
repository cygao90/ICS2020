#include <common.h>

// #ifdef HAS_IOE

#include <device/map.h>
#include <SDL2/SDL.h>

#define AUDIO_PORT 0x200 // Note that this is not the standard
#define AUDIO_MMIO 0xa1000200
#define STREAM_BUF 0xa0800000
#define STREAM_BUF_MAX_SIZE 65536

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size, // max size of the stream buffer
  reg_init,      // init
  reg_count,    // size of used buffer
  reg_head,
  reg_tail,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

// This function is to read data from buffer.
static inline void audio_play(void *userdata, uint8_t *stream, int len) {
  int nread = len;
  if (audio_base[reg_count] < len)
    nread = audio_base[reg_count];
  
  if (audio_base[reg_tail] + nread < audio_base[reg_sbuf_size]) {
    memcpy(stream, sbuf + audio_base[reg_tail], nread);
    audio_base[reg_tail] += nread;
  } else {
    int first_copy = audio_base[reg_sbuf_size] - audio_base[reg_tail];
    memcpy(stream, sbuf + audio_base[reg_tail], first_copy);
    memcpy(stream + first_copy, sbuf, nread - first_copy);
    audio_base[reg_tail] = nread - first_copy;
  }
  audio_base[reg_count] -= nread;
  if (len > nread) 
    memset(stream + nread, 0, len - nread);
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  if (is_write && audio_base[reg_init]) {
    audio_base[reg_init] = false;
    SDL_AudioSpec s = {};
    s.format = AUDIO_S16LSB;
    s.userdata = NULL;
    s.freq = audio_base[reg_freq];
    s.channels = audio_base[reg_channels];
    s.samples = audio_base[reg_samples];
    s.callback = audio_play;

    audio_base[reg_sbuf_size] = STREAM_BUF_MAX_SIZE;
    audio_base[reg_count] = 0;
    audio_base[reg_head] = audio_base[reg_tail] = 0;
    int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (ret == 0) {
      SDL_OpenAudio(&s, NULL);
      SDL_PauseAudio(0);
    }
  }
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (void *)new_space(space_size);
  add_pio_map("audio", AUDIO_PORT, (void *)audio_base, space_size, audio_io_handler);
  add_mmio_map("audio", AUDIO_MMIO, (void *)audio_base, space_size, audio_io_handler);

  sbuf = (void *)new_space(STREAM_BUF_MAX_SIZE);
  add_mmio_map("audio-sbuf", STREAM_BUF, (void *)sbuf, STREAM_BUF_MAX_SIZE, NULL);
}
// #endif	/* HAS_IOE */
