/*
  Copyright (c) 2015 Mikhail Usenko <michaelus@tochka.ru>
*/

#include "popen2.h"
#include "fd.h"


static void close_all_fds(const int *_exclude = nullptr)
{
  // TODO ...
}


pid_t popen2(const char *_file, const char *_argv[], int *_rd, int *_wr, const int _oflags)
{
  pid_t pid = -1;
  int e = 0;

  struct pipe_fd {
    // fd d[2] = { -1, -1 }, &rd = d[0], &wr = d[1];
    fd rd = -1, wr = -1;
    operator int* ()  { static_assert(sizeof(fd) == sizeof(int), ""); return (int*)&rd; }
    void close()  { rd.close(); wr.close(); }
  } downstream, upstream, child_status;

  if (::pipe2(downstream, _oflags) == -1)  { e = errno; goto quit_parent__; };
  if (::pipe2(upstream, _oflags) == -1)  { e = errno; goto quit_parent__; };
  if (::pipe2(child_status, O_CLOEXEC) == -1)  { e = errno; goto quit_parent__; };

  switch (pid = fork())
  {
    default:  downstream.wr.close(); upstream.rd.close();
              child_status.wr.close();
              while (::read(child_status.rd, &e, sizeof(e)) == -1)  if (errno != EINTR)  { e = errno; break /* while */; };
              child_status.rd.close();
        break;

    case  0:  child_status.rd.close();
              {
                pipe_fd &downstream_ = upstream, &upstream_ = downstream;

                downstream_.wr.close(); upstream_.rd.close();

                if (downstream_.rd.mov2(0) == -1)  goto quit_child__;
                if (upstream_.wr.mov2(1) == -1)  goto quit_child__;

                int x[] = { 0, 1, 2, child_status.wr, -1 };
                close_all_fds(x);
                
                execvp(_file, const_cast< char* const* >(_argv));
              }
              quit_child__:
              e = errno;
              while (::write(child_status.wr, &e, sizeof(e)) == -1 and errno == EINTR);
              child_status.wr.close();
              ::_exit(e);

    case -1:  e = errno;
        goto quit_parent__;
  };

quit_parent__:

  if (e != 0)
  {
    downstream.close(); upstream.close();
    pid = -1;
  };

  errno = e;
  *_rd = downstream.rd; *_wr = upstream.wr;
  return pid;
}


pid_t popen2i(const char *_file, const char *_argv[], const int _in, int *_rd, const int _oflags)
{
  pid_t pid = -1;
  int e = 0;

  struct pipe_fd {
    // fd d[2] = { -1, -1 }, &rd = d[0], &wr = d[1];
    fd rd = -1, wr = -1;
    operator int* ()  { static_assert(sizeof(fd) == sizeof(int), ""); return (int*)&rd; }
    void close()  { rd.close(); wr.close(); }
  } downstream, child_status;

  if (::pipe2(downstream, _oflags) == -1)  { e = errno; goto quit_parent__; };
  if (::pipe2(child_status, O_CLOEXEC) == -1)  { e = errno; goto quit_parent__; };

  switch (pid = fork())
  {
    default:  downstream.wr.close();
              child_status.wr.close();
              while (::read(child_status.rd, &e, sizeof(e)) == -1)  if (errno != EINTR)  { e = errno; break /* while */; };
              child_status.rd.close();
        break;

    case  0:  child_status.rd.close();
              {
                pipe_fd &upstream_ = downstream;
                fd in_ = _in;

                upstream_.rd.close();

                if (in_ == 0)  in_.unsetfd(FD_CLOEXEC); else  if (in_.mov2(0) == -1)  goto quit_child__;
                if (upstream_.wr.mov2(1) == -1)  goto quit_child__;

                int x[] = { 0, 1, 2, child_status.wr, -1 };
                close_all_fds(x);
                
                execvp(_file, const_cast< char* const* >(_argv));
              }
              quit_child__:
              e = errno;
              while (::write(child_status.wr, &e, sizeof(e)) == -1 and errno == EINTR);
              child_status.wr.close();
              ::_exit(e);

    case -1:  e = errno;
        goto quit_parent__;
  };

quit_parent__:

  if (e != 0)
  {
    downstream.close();
    pid = -1;
  };

  errno = e;
  *_rd = downstream.rd;
  return pid;
}


pid_t popen2o(const char *_file, const char *_argv[], const int _out, int *_wr, const int _oflags)
{
  pid_t pid = -1;
  int e = 0;

  struct pipe_fd {
    // fd d[2] = { -1, -1 }, &rd = d[0], &wr = d[1];
    fd rd = -1, wr = -1;
    operator int* ()  { static_assert(sizeof(fd) == sizeof(int), ""); return (int*)&rd; }
    void close()  { rd.close(); wr.close(); }
  } upstream, child_status;

  if (::pipe2(upstream, _oflags) == -1)  { e = errno; goto quit_parent__; };
  if (::pipe2(child_status, O_CLOEXEC) == -1)  { e = errno; goto quit_parent__; };

  switch (pid = fork())
  {
    default:  upstream.rd.close();
              child_status.wr.close();
              while (::read(child_status.rd, &e, sizeof(e)) == -1)  if (errno != EINTR)  { e = errno; break /* while */; };
              child_status.rd.close();
        break;

    case  0:  child_status.rd.close();
              {
                pipe_fd &downstream_ = upstream;
                fd out_ = _out;

                downstream_.wr.close();

                if (downstream_.rd.mov2(0) == -1)  goto quit_child__;
                if (out_ == 1)  out_.unsetfd(FD_CLOEXEC); else  if (out_.mov2(1) == -1)  goto quit_child__;

                int x[] = { 0, 1, 2, child_status.wr, -1 };
                close_all_fds(x);
                
                execvp(_file, const_cast< char* const* >(_argv));
              }
              quit_child__:
              e = errno;
              while (::write(child_status.wr, &e, sizeof(e)) == -1 and errno == EINTR);
              child_status.wr.close();
              ::_exit(e);

    case -1:  e = errno;
        goto quit_parent__;
  };

quit_parent__:

  if (e != 0)
  {
    upstream.close();
    pid = -1;
  };

  errno = e;
  *_wr = upstream.wr;
  return pid;
}


pid_t popen2io(const char *_file, const char *_argv[], const int _in, const int _out)
{
  pid_t pid = -1;
  int e = 0;

  struct pipe_fd {
    // fd d[2] = { -1, -1 }, &rd = d[0], &wr = d[1];
    fd rd = -1, wr = -1;
    operator int* ()  { static_assert(sizeof(fd) == sizeof(int), ""); return (int*)&rd; }
    void close()  { rd.close(); wr.close(); }
  } child_status;

  if (::pipe2(child_status, O_CLOEXEC) == -1)  { e = errno; goto quit_parent__; };

  switch (pid = fork())
  {
    default:  child_status.wr.close();
              while (::read(child_status.rd, &e, sizeof(e)) == -1)  if (errno != EINTR)  { e = errno; break /* while */; };
              child_status.rd.close();
        break;

    case  0:  child_status.rd.close();
              {
                fd in_ = _in, out_ = _out;

                if (in_ == 0)  in_.unsetfd(FD_CLOEXEC); else  if (in_.mov2(0) == -1)  goto quit_child__;
                if (out_ == 1)  out_.unsetfd(FD_CLOEXEC); else  if (out_.mov2(1) == -1)  goto quit_child__;

                int x[] = { 0, 1, 2, child_status.wr, -1 };
                close_all_fds(x);
                
                execvp(_file, const_cast< char* const* >(_argv));
              }
              quit_child__:
              e = errno;
              while (::write(child_status.wr, &e, sizeof(e)) == -1 and errno == EINTR);
              child_status.wr.close();
              ::_exit(e);

    case -1:  e = errno;
        goto quit_parent__;
  };

quit_parent__:

  if (e != 0)
  {
    pid = -1;
  };
  
  errno = e;
  return pid;
}

