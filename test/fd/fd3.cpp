
#include "fd.h"
#include <stdlib.h>
#include <stdio.h>



int main(int _argc, char* const _argv[])
{
  fd err = 2;
  printf("err [%d](%u): F_GETFD(0x%X) F_GETFL(0x%X)\n", (int)err, (bool)err, fcntl(err, F_GETFD), fcntl(err, F_GETFL));
  err.setfd(FD_CLOEXEC);
  printf("err [%d](%u): F_GETFD(0x%X) F_GETFL(0x%X)\n", (int)err, (bool)err, fcntl(err, F_GETFD), fcntl(err, F_GETFL));

  fd out = 1;
  printf("out [%d](%u): F_GETFD(0x%X) F_GETFL(0x%X)\n", (int)out, (bool)out, fcntl(out, F_GETFD), fcntl(out, F_GETFL));

  err.dup2(out);
  printf("out [%d](%u): F_GETFD(0x%X) F_GETFL(0x%X)\n", (int)out, (bool)out, fcntl(out, F_GETFD), fcntl(out, F_GETFL));
  
  out.dup2(err);
  printf("err [%d](%u): F_GETFD(0x%X) F_GETFL(0x%X)\n", (int)err, (bool)err, fcntl(err, F_GETFD), fcntl(err, F_GETFL));

  return 0;
}
