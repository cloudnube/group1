
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"
#include "lib/string.h"
#include "lib/stdlib.h"
#include <debug.h>

void write_bbb (int fd, char *content);
void read_bbb (int fd, int length);

void write_bbb (int fd, char *content)
{
  buffer_stats_reset ();
  for (int i = 0; i < 65536; i ++)
    write (fd, content, 1);
  msg("hello\n");
  cache_stats ();
  // int accesses = buffer_accesses ();
  // int hits = accesses - buffer_miss_count ();
  // msg ("wrote %d bytes, hits: %d, accesses %d", strlen (content) * times, hits, accesses);
}

void read_bbb (int fd, int length)
{
  buffer_stats_reset ();
  char content[length];
  for (int i = 0; i < 65536; i ++)
    read (fd, content, 1);
  cache_stats ();
  // int accesses = buffer_accesses();
  // int hits = accesses - buffer_miss_count ();
  // msg ("read %d bytes, hits: %d, accesses %d", length * times, hits, accesses);
}


void
test_main (void)
{
  create ("/testfile420", 0);
  int fd = open ("/testfile420");

  static char sixtyfourkb[65536];
  buffer_reset ();
  write_bbb (fd, sixtyfourkb);
  msg("hello1\n");
  seek (fd, 0);
  read_bbb (fd, 65536);
  seek (fd, 0);
  // free (sixtyfourkb);
}