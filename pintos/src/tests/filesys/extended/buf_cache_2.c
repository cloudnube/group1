
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"
#include "lib/string.h"
#include "lib/stdlib.h"
#include <debug.h>

#define SECTORS 128
#define ACCESS_SIZE 512 * SECTORS
#define TOLERANCE 4

inline within_tolerance (int value, int target, int tolerance)
{
  return value <= target + tolerance && value >= target - tolerance;
}

void
test_main (void)
{
  create ("/testfile420", ACCESS_SIZE);
  int fd = open ("/testfile420");
  char z;
  int writes = device_writes ();
  for (int i = 0; i < ACCESS_SIZE; i ++)
    write (fd, &z, 1);
  writes = device_writes () - writes;
  ASSERT (within_tolerance (writes, SECTORS, TOLERANCE));
  seek (fd, 0);
  int reads = device_reads ();
  for (int i = 0; i < ACCESS_SIZE; i ++)
    read (fd, &z, 1);
  reads = device_reads () - reads;
  ASSERT (within_tolerance (reads, SECTORS, TOLERANCE));
  return;
}