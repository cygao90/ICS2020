#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,
#define NR_KEY (sizeof(keyname) / sizeof(keyname[0]))

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static uint8_t keystate[NR_KEY];

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[64] = {0};
  if (NDL_PollEvent(buf, sizeof(buf)) == 0) {
    return 0;
  }

  if (strcmp(buf, keyname[0]) == 0 || buf[0] == '\0') {
    ev->key.keysym.sym = SDLK_NONE;
    ev->type = SDL_USEREVENT;
    return 0;
  }

  int i;
  for (i = 1; i < NR_KEY; i++) {
    if (strcmp(keyname[i], buf + 3) == 0) {
      ev->key.keysym.sym = i;
      break;
    }
  }
  if (buf[1] == 'd') {
    // keydown
    ev->type = SDL_KEYDOWN;
    keystate[i] = 1;
  } else if (buf[1] == 'u') {
    // keyup
    ev->type = SDL_KEYUP;
    keystate[i] = 0;
  } else {
    assert(0);
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64] = {0};
  NDL_PollEvent(buf, sizeof(buf));
  buf[strlen(buf)] = '\0';
  int i;
  for (i = 0; i < NR_KEY; i++) {
    if (strcmp(keyname[i], buf + 3) == 0) {
      event->key.keysym.sym = i;
      break;
    }
  }
  if (buf[1] == 'd') {
    // keydown
    event->type = SDL_KEYDOWN;
    keystate[i] = 1;
  } else if (buf[1] == 'u') {
    // keyup
    event->type = SDL_KEYUP;
    keystate[i] = 0;
  }
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return keystate;
}
