
#include "fd.h"
#include <stdlib.h>
#include <stdio.h>



int main(int _argc, char *const _argv[])
{
  fd t = 5;
  printf("(fd)t: [%d](%d)\n", (int)t, (bool)t);
  printf("(fd)t: F_GETFD(0x%X) F_GETFL(0x%X)\n", fcntl(t, F_GETFD), fcntl(t, F_GETFL));

  /*
  fd fd_stdout;
  fd_stdout = 1;
  printf("(fd)fd_stdout: [%d](%d)\n", (int)fd_stdout, (bool)fd_stdout);
  */

  fd fd_stdin = 0;
  printf("(fd)fd_stdin: [%d](%d)\n", (int)fd_stdin, (bool)fd_stdin);

  int o = -1;

  o = fd(1).dup2(t, FD_CLOEXEC);
  if (o == -1)  perror("fd(1).dup2(t)");
  printf("(int)1: F_GETFD(0x%X) F_GETFL(0x%X)\n", fcntl(1, F_GETFD), fcntl(1, F_GETFL));
  printf("(int)fd(1).dup2(t): [%d] F_GETFD(0x%X) F_GETFL(0x%X)\n", o, fcntl(o, F_GETFD), fcntl(o, F_GETFL));


  printf("FD_CLOEXEC = (0x%X), O_CLOEXEC = (0x%X)\n", FD_CLOEXEC, O_CLOEXEC);

  o = fd_stdin.mov2(t, FD_CLOEXEC);
  if (o == -1)  perror("fd_stdin.mov(t)");
  printf("(fd)t: [%d](%d)\n", (int)t, (bool)t);
  printf("(fd)fd_stdin: [%d](%d)\n", (int)fd_stdin, (bool)fd_stdin);
  printf("(fd)t: F_GETFD(0x%X) F_GETFL(0x%X)\n", fcntl(t, F_GETFD), fcntl(t, F_GETFL));
  printf("(fd)fd_stdin: F_GETFD(0x%X) F_GETFL(0x%X)\n", fcntl(fd_stdin, F_GETFD), fcntl(fd_stdin, F_GETFL));
  printf("(int)0: F_GETFD(0x%X) F_GETFL(0x%X)\n", fcntl(0, F_GETFD), fcntl(0, F_GETFL));
  fd fd0(0);
  printf("(fd)fd0: [%d](%d)\n", (int)fd0, (bool)fd0);

  return 0;
}
