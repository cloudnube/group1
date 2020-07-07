#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);
struct lock file_lock;
struct file *file_descriptors[128];

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_lock);
  int i;
  for (i = 0; i < 128; i ++) {
    file_descriptors[i] = NULL;
  }
  //file_descriptors[0] = STDIN_FILENO;
  //file_descriptors[1] = STDOUT_FILENO;
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  uint32_t* args = ((uint32_t*) f->esp);

  /*
   * The following print statement, if uncommented, will print out the syscall
   * number whenever a process enters a system call. You might find it useful
   * when debugging. It will cause tests to fail, however, so you should not
   * include it in your final submission.
   */

  /* printf("System call number: %d\n", args[0]); */

  if (args[0] == SYS_EXIT)
    {
      f->eax = args[1];
      printf ("%s: exit(%d)\n", &thread_current ()->name, args[1]);
      thread_exit ();
    }

  if (args[0] == SYS_CREATE
    || args[0] == SYS_REMOVE
    || args[0] == SYS_OPEN
    || args[0] == SYS_FILESIZE
    || args[0] == SYS_READ
    || args[0] == SYS_WRITE
    || args[0] == SYS_SEEK
    || args[0] == SYS_TELL
    || args[0] == SYS_CLOSE) {

      lock_acquire(&file_lock);

      if (args[0] == SYS_CREATE) {
        f->eax = filesys_create((char *) args[1], args[2]);
      } else if (args[0] == SYS_REMOVE) {
        f->eax = filesys_remove((char *) args[1]);
      } else if (args[0] == SYS_OPEN) {
        struct file* fp = filesys_open((char *) args[1]);
        int fd = 2;
        while (fd < 128) {
          if (file_descriptors[fd] == NULL) {
            file_descriptors[fd] = fp;
            break;
          }
          fd ++;
        }
        if (fd == 128) {
          f->eax = -1;
        }
        f->eax = fd + 2;
      } else if (args[0] == SYS_FILESIZE) {
        f->eax= file_length(file_descriptors[args[1]]);
      } else if (args[0] == SYS_READ) {
        f->eax = file_read(file_descriptors[args[1]], (void *) args[2], args[3]);
      } else if (args[0] == SYS_WRITE) {
        f->eax = file_write(file_descriptors[args[1]], (void *) args[2], args[3]);
      } else if (args[0] == SYS_SEEK) {
        file_seek(file_descriptors[args[1]], args[2]);
      } else if (args[0] == SYS_TELL) {
        f->eax = file_tell(file_descriptors[args[1]]);
      } else if (args[0] == SYS_CLOSE) {
        file_close(file_descriptors[args[1]]);
        file_descriptors[args[1]] = NULL;
      }


      lock_release(&file_lock);
    }

}
