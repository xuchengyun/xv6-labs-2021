#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int sleep_ticks;

  if (argc < 2) {
    fprintf(2, "Usage: sleep ticks\n");
    exit(1);
  }

  sleep_ticks = atoi(argv[1]);
  sleep(sleep_ticks);

  exit(0);
}
