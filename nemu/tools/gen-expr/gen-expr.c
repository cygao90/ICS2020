#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";


static int choose(int n) {
  return rand() % n;
}

static void gen_num() {
  sprintf(buf + strlen(buf), "%u", (unsigned int)rand() % 10000);
  buf[strlen(buf)] = '\0';
}

static void gen(char a) {
  sprintf(buf + strlen(buf), "%c", a);
  buf[strlen(buf)] = '\0';
}

static void gen_rand_op() {
  char opt;
  switch (rand() % 4) {
    case 0: opt = '+'; break;
    case 1: opt = '-'; break;
    case 2: opt = '*'; break;
    case 3: opt = '/'; break;
  }
  sprintf(buf + strlen(buf), "%c", opt);
  buf[strlen(buf)] = '\0';
}

static inline void gen_rand_expr() {
  if (strlen(buf) < 1000) {
    switch (choose(3)) {
      case 0: gen_num(); break;
      case 1: gen('('); gen_rand_expr(); gen(')'); break;
      default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
    }
  } else {
    gen_num();
  }
  buf[strlen(buf)] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    memset(buf, 0, strlen(buf));
    gen_rand_expr();
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    char buf1[30] = { 0 };
    fread(buf1, sizeof(buf1), 1, fp);
    if (strlen(buf1) == 0)
      continue;

    int result;
    sscanf(buf1, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
