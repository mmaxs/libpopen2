
#include "popen2.h"
#include <stdlib.h>
#include <stdio.h>



int main(int _argc, char *const _argv[])
{

  pid_t pid;
  int rd, wr;

  /* open the child process and establish the pipes to/from its stdin/stdout */
  const char* args[] = { "cat", 0 };
  if ((pid = popen2(args[0], args,  &rd, &wr)) == -1)
  {
    perror("popen2()");
    return 1;
  }
  else fprintf(stderr, "debug: rd=[%i] wr=[%i]\n", rd, wr);

  /* write to child's stdin */
  FILE *s_wr = fdopen(wr, "w");

  if (fprintf(s_wr, "Hello, libpopen2!\n") == 0)
  {
    perror("fprintf()");
    return 2;
  };
  
  if (fflush(s_wr) == EOF)
  {
    perror("fflush()");
    return 3;
  };

  /* read from child's stdout */
  FILE *s_rd = fdopen(rd, "r");

  char *line = NULL;
  size_t lsize = 0;
  ssize_t n = 0;
  if ((n = getline(&line, &lsize, s_rd)) != -1)  printf("[%zd/%zu]: %s", n, lsize, line);
  else
  {
    perror("getline()");
    return 4;
  };

  free(line);

  return 0;
}
