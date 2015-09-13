
#include "popen2.h"
#include <stdio.h>
#include <sys/wait.h>


void wait_all()  { while (wait(0) != -1 or errno != ECHILD); }


int main(int _argc, char *const _argv[])
{

  printf("\n");
  printf("%s\n", "cat </dev/urandom | tr -c '[:alnum:]\\n' . | head -n 1");

  const char* prog1[] = { "cat", 0 };
  const char* prog2[] = { "tr", "-c", "'[:alnum:]\\n'", ".", 0 };
  const char* prog3[] = { "head", "-n", "1", 0 };

  int rd1 = -1, rd2 = -1;
  
  int urandom = -1;
  if ((urandom = open("/dev/urandom", O_RDONLY)) == -1)  perror("/dev/urandom");

  if (popen2i(prog1[0], prog1, urandom, &rd1) == -1)  perror(prog1[0]);
  if (popen2i(prog2[0], prog2, rd1, &rd2) == -1)  perror(prog2[0]);
  if (popen2io(prog3[0], prog3, rd2, 1) == -1)  perror(prog3[0]);

  // close all descriptors intended for child processes
  // so the prog3 completion results in terminating all of the parts of the pipeline
  close(rd2); close(rd1); close(urandom);

  wait_all();
  
  return 0;
}
