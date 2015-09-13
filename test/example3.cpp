
#include "popen2.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


void wait_all()  { while (wait(0) != -1 or errno != ECHILD); }


int main(int _argc, char *const _argv[])
{

  const char* prog1[] = { "cat", "/dev/urandom", 0 };
  const char* prog2[] = { "wc", "-c", 0 };

  pid_t pid1 = -1, pid2 = -1;

  
  printf("\n");
  printf("%s\n", "cat /dev/urandom | wc -c  # launched from left to right");
  
  /*** start pipeline parts from left to right ***/
  int rd = -1;
  if ((pid1 = popen2i(prog1[0], prog1, 0, &rd)) == -1)  { perror(prog1[0]); return 1; };
  if ((pid2 = popen2io(prog2[0], prog2, rd, 1)) == -1)  { perror(prog2[0]); return 2; };

  // write side of the pipeline is opened in the prog1, which completion will break the pipeline
  // read side of the pipeline is opened in the parent process and then passed to the prog2
 
  // stop prog2 after 3 second
  sleep(3);
  kill(pid2, SIGHUP);
  printf("%s\n", "'wc -c' killed; pipe still open in parent");

  // prog1 is still running because of the pipeline is not broken yet as we have its read side still open in the parent
  // stop prog1 by breaking the pipe after more 3 seconds
  sleep(3);
  close(rd);
  printf("%s\n", "pipe closed; 'cat /dev/urandom' stopped on broken pipe");

  wait_all();


  printf("\n");
  printf("%s\n", "cat /dev/urandom | wc -c  # launched from right to left");
  
  /*** start pipeline parts from right to left ***/
  int wr = -1;
  if ((pid2 = popen2o(prog2[0], prog2, 1, &wr)) == -1)  { perror(prog2[0]); return 3; };
  if ((pid1 = popen2io(prog1[0], prog1, 0, wr)) == -1)  { perror(prog1[0]); return 4; };

  // write side of the pipeline is opened in the parent process and then passed to the prog1
  // read side of the pipeline is opened in the prog2, which completion will break the pipeline

  // stop prog1 after 3 second
  sleep(3);
  kill(pid1, SIGHUP);
  printf("%s\n", "'cat /dev/urandom' killed; pipe still open in parent");

  // prog2 is still running because of the pipeline is not broken yet as we have its write side still open in the parent
  // stop prog2 by breaking the pipe after more 3 seconds
  sleep(3);
  close(wr);
  printf("%s\n", "pipe closed; 'wc -c' finished at EOF");

  wait_all();

  
  return 0;
}
