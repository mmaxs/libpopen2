
#include "fd.h"
#include "popen2.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


void proc_fd(std::string &_p)
{
  _p.clear();

  int n = -1;
  if ((n = snprintf(nullptr, 0, "/proc/%d/fd", ::getpid())) > 0)
  {
    _p.assign(n, 0);
    snprintf(const_cast< char* >(_p.c_str()), n+1, "/proc/%d/fd", getpid());
  };
}


void ls(const int _out, const char* const  _p)
{
  if (!_p)  return;

  const char* cmd[] = { "ls", "-AFl", "--time-style=long-iso", "--color=auto", _p, 0 };
  popen2io(cmd[0], cmd, 0, _out);
}



int main(int _argc, char* const _argv[])
{
  std::string p;
  proc_fd(p);
  if (p.empty())
  {
    perror("proc_fd()");
    return 1;
  };

  pid_t cpid;
  int rd, wr;

  const char* args[] = { "cat", 0 };
  if ((cpid = popen2(args[0], args,  &rd, &wr)) == -1)
  {
    perror("popen2()");
    return 2;
  };

  fprintf(stderr, "pid=[%i] cpid=[%i] rd=[%i] wr=[%i]\n", getpid(), cpid, rd, wr);
  ls(fileno(stderr), p.c_str());
  getchar();

  int x[] = { 0, 2, wr, -1 };
  fd::close_all(x);

  ls(fileno(stderr), p.c_str());
  getchar();


  return 0;
}
