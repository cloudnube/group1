
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"
#include "lib/string.h"
#include "lib/stdlib.h"
#include <debug.h>

void write_log (int fd, char *content, int times);
void read_log (int fd, int length, int times);

void write_log (int fd, char *content, int times)
{
  buffer_stats_reset ();
  for (int i = 0; i < times; i ++)
    write (fd, content, strlen (content));
  int accesses = buffer_accesses ();
  int hits = accesses - buffer_miss_count ();
  msg ("wrote %d bytes, hits: %d, accesses %d", strlen (content) * times, hits, accesses);
}

void read_log (int fd, int length, int times)
{
  buffer_stats_reset ();
  char content[length];
  for (int i = 0; i < times; i ++)
    read (fd, content, length);
  int accesses = buffer_accesses();
  int hits = accesses - buffer_miss_count ();
  msg ("read %d bytes, hits: %d, accesses %d", length * times, hits, accesses);
}

void
test_main (void)
{
  create ("/mabel.selfie", 0);
  int fd = open ("/mabel.selfie");
  char *hoopla = "Two and a Half Men. I watch that.";
  char *line = "0123456789";
  char *blargo = malloc (2 << 20);
  buffer_reset ();
  write_log (fd, blargo, 1);
  seek (fd, 0);
  write_log (fd, line, 1000);
  seek (fd, 0);
  read_log (fd, strlen (line), 1000);
  free (blargo);
}