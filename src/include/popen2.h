/*
  Copyright (c) 2015 Mikhail Usenko <michaelus@tochka.ru>
*/

#ifndef POPEN2__H
#define POPEN2__H

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


extern "C"
{


/* the function creates a child process by forking and executing the _file with
   command line parameter list _argv[] using execvp(3), opens the pipes to
   child's process stdin and from its stdout; _oflags can be specified for
   created pipes as described for pipe2(2);

   on success, the PID value > 0 of the created child process is returned, *_rd
   is set to the file descriptor number referring to the read end of the pipe
   opened from child's stdout, *_wr is set to the file descriptor number
   referring to the write end of the pipe opened to child's stdin;

   on failure, -1 is returned to the caller, *_rd and *_wr are set to -1, no
   child process is created, errno is set to indicate the cause of the error */
pid_t popen2(const char *_file, const char *_argv[], int *_rd, int *_wr, const int _oflags = 0);


/* the function creates a child process by forking and executing the _file with
   command line parameter list _argv[] using execvp(3), duplicates the file
   descriptor referring by the number _in to child's process stdin, opens the
   pipe from child's process stdout; _oflags can be specified for created pipe
   as described for pipe2(2);

   on success, the PID value > 0 of the created child process is returned,
   child's process stdin is set to be referring the same file descriptor as for
   the number _in, *_rd is set to the file descriptor number referring to the
   read end of the pipe opened from child's stdout;

   on failure, -1 is returned to the caller, *_rd is set to -1, no
   child process is created, errno is set to indicate the cause of the error */
pid_t popen2i(const char *_file, const char *_argv[], const int _in, int *_rd, const int _oflags = 0);


/* the function creates a child process by forking and executing the _file with
   command line parameter list _argv[] using execvp(3), duplicates the file
   descriptor referring by the number _out to child's process stdout, opens the
   pipe to child's process stdin; _oflags can be specified for created pipe as
   described for pipe2(2);

   on success, the PID value > 0 of the created child process is returned,
   child's process stdout is set to be referring the same file descriptor as for
   the number _out, *_wr is set to the file descriptor number referring to the
   write end of the pipe opened to child's stdin;

   on failure, -1 is returned to the caller, *_wr is set to -1, no
   child process is created, errno is set to indicate the cause of the error */
pid_t popen2o(const char *_file, const char *_argv[], const int _out, int *_wr, const int _oflags = 0);


/* the function creates a child process by forking and executing the _file with
   command line parameter list _argv[] using execvp(3), duplicates the file
   descriptors referring by the numbers _in and _out to child's process stdin
   and stdout respectively;

   on success, the PID value > 0 of the created child process is returned,
   child's process stdin and stdout are set to be referring the same file
   descriptors as for the numbers _in and _out respectively;

   on failure, -1 is returned to the caller, no child process is created,
   errno is set to indicate the cause of the error */
pid_t popen2io(const char *_file, const char *_argv[], const int _in, const int _out);


}

#endif
