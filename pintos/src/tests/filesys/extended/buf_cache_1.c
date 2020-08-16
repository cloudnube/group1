
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"
#include "lib/string.h"
#include "lib/stdlib.h"
#include <debug.h>

// both logging functions return the amount of cache misses
int write_misses (int fd, char *content, int length, int times);
int read_misses (int fd, char *content, int length, int times);
#define LARGE_SIZE 512 * 10
static char blargo[LARGE_SIZE];

int write_misses (int fd, char *content, int length, int times)
{
  buffer_stats_reset ();
  for (int i = 0; i < times; i ++)
    write (fd, content, length);
  int misses = buffer_miss_count ();
  return misses;
}

int read_misses (int fd, char *content, int length, int times)
{
  buffer_stats_reset ();
  for (int i = 0; i < times; i ++)
    read (fd, content, length);
  int misses = buffer_miss_count ();
  return misses;
}

void
test_main (void)
{
  create ("/temp.file", 0);
  int fd = open ("/temp.file");
  buffer_reset ();
  ASSERT (write_misses (fd, blargo, LARGE_SIZE, 1) > 0);
  buffer_reset ();
  seek (fd, 0);
  ASSERT (read_misses (fd, blargo, LARGE_SIZE, 1) > 0);
  seek (fd, 0);
  ASSERT (read_misses (fd, blargo, LARGE_SIZE, 1) == 0);
}