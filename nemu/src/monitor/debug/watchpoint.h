#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[32];
} WP;

WP* new_wp();
void free_wp(int NO);
bool check_watchpoint();
void watchpoint_display();

#endif
