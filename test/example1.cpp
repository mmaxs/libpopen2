
#include "popen2.h"
#include <stdio.h>
#include <sys/wait.h>


void wait_all()  { while (wait(0) != -1 or errno != ECHILD); }


int main(int _argc, char *const _argv[])
{
  printf("\n");
  printf("%s\n", "cat /etc/passwd");
  {
    const char* prog1[] = { "cat", "/etc/passwd", 0 };

    if (popen2io(prog1[0], prog1, 0, 1) == -1)  perror(prog1[0]);

    wait_all();
  }

  printf("\n");
  printf("%s\n", "cat </etc/passwd");
  {
    const char* prog1[] = { "cat", 0 };

    int passwd = -1;
    if ((passwd = open("/etc/passwd", O_RDONLY)) == -1)  perror("/etc/passwd");

    if (popen2io(prog1[0], prog1, passwd, 1) == -1)  perror(prog1[0]);

    wait_all();
    close(passwd);
  }

  printf("\n");
  printf("%s\n", "cat /etc/passwd | grep root");
  {
    const char* prog1[] = { "cat", "/etc/passwd", 0 };
    const char* prog2[] = { "grep", "root", 0 };

    int rd = -1;

    if (popen2i(prog1[0], prog1, 0, &rd) == -1)  perror(prog1[0]);
    if (popen2io(prog2[0], prog2, rd, 1) == -1)  perror(prog2[0]);
    close(rd);

    wait_all();
  }

  return 0;
}
