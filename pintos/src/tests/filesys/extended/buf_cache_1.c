
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  mkdir ("dir");
  create ("dir/mabel.selfie", 0);
  int fd = open ("dir/mabel.selfie");
  char *hoopla = "Two and a\n half men. I watch that.";
  write (fd, hoopla, 20);
  return 162;
}