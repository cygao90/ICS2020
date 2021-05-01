#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include "memory/paddr.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
  int n;
  if (args == NULL) {
    n = 1;
  } else {
    n = atoi(args);
  }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args) {
  if (!strcmp(args, "r")) {
    isa_reg_display();
    return 0;
  }
  else if (!strcmp(args, "w")) {
    watchpoint_display();
    return 0;
  }
  return -1;
}

static int cmd_x(char *args) {
  if (args == NULL) {
    return 0;
  }
  char* token;
  token = strtok(args, " ");
  int64_t n = -1;
  sscanf(token, "%ld", &n);
  if (n < 0) {
    printf("Invalid number\n");
    return 0;
  }
  token = strtok(NULL, "");
  if (token == NULL) {
    return 0;
  }
  /*
  uint32_t val;
  sscanf(token, "0x%x", &val);
  */
  uint32_t val;
  bool success;
  val = expr(token, &success);
  assert(success == true);
  for (int64_t i = 0; i < n; i++) {
    printf("0x%08lx   ", val + i * 4);
    printf("0x%08x\n", paddr_read(val + i * 4, 4));
  }
  return 0;
}

static int cmd_p(char *args) {
  bool success = true;
  printf("%u\n", expr(args, &success));
  return 0;
}

static int cmd_w(char *args) {
  WP* p = new_wp();
  strcpy(p->expr, args);
  return 0;
}

static int cmd_d(char *args) {
  int no;
  sscanf(args, "%d", &no);
  free_wp(no);
  return 0;
}

static int cmd_detach(char *args) {
  is_difftest = false;
  return 0;
}

static int cmd_attach(char *args) {
  is_difftest = true;
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */

  { "si", "si [N] - Let program excute N steps, default N is 1", cmd_si },
  { "info", "info [rw] r - print the status of regs, w - print watching point info", cmd_info },
  { "x", "x [N] [EXPR], evaluate the EXPR, use the result as the start address and output N consecutive 4 bytes in hex form", cmd_x },
  { "p", "evaluate", cmd_p },
  { "w", "w [EXPR] watchpoint, stop the program when EXPR is true", cmd_w },
  { "d", "d [N] delete the watchpoint", cmd_d },
  { "detach", "close difftest", cmd_detach },
  { "attach", "start difftest", cmd_attach },
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
