/*
  Copyright (c) 2015 Mikhail Usenko <michaelus@tochka.ru>
*/

#ifndef FD__H
#define FD__H

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <new>


/* a small class wrapping the most frequent operations with file descriptors
   that makes client code to be a bit more clean and straightforward; class
   size is equal to sizeof(int) and its instances are interchangeable with file
   descriptor POD-values for most use cases */
class fd
{
private:
  int n_ = -1;

public:
  operator int& ()  { static_assert(sizeof(*this) == sizeof(n_), "");  return n_; }
  operator int () const  { static_assert(sizeof(*this) == sizeof(n_), "");  return n_; }

public:
  ~fd() = default;

  fd() = default;

  fd(const int _n) : n_(_n)  {}

  fd(const fd &_fd) : n_(_fd.n_)  {}

public:
  fd& operator = (const fd &_fd)
  {
    n_ = _fd.n_;
    return *this;
  }

  fd& operator = (const int _n)
  {
    n_ = _n;
    return *this;
  }

public:
  /* check if the fd variable has a valid and open file descriptor number */
  /* const specifier is very important here or else in some cases 'operator int () const' can be used instead */
  explicit operator bool () const  { return ::fcntl(n_, F_GETFD) != -1; }

public:
  /* invalidate fd variable */
  int inval()
  {
    int o = n_;
    n_ = -1;
    return o;
  }

  /* close the file descriptor number and invalidate fd variable */
  int close(const bool _force_inval = true)
  {
    int o = -1;
    while ((o = ::close(n_)) == -1 and errno == EINTR);
    if (o != -1 or _force_inval)  n_ = -1;
    return o;
  }

  /* duplicate file descriptor to a new number;
     optionally set file descriptor flags for the new file descriptor number */
  int dup(const int _F_SETFD = -1) const
  {
    int o = ::dup(n_);
    if (o != -1 and _F_SETFD != -1)  ::fcntl(o, F_SETFD, _F_SETFD);
    return o;
  }

  /* duplicate file descriptor to the specified number, closing the target file
     descriptor number first if necessary; optionally set file descriptor flags
     for the new file descriptor number */
  int dup2(const int _n, const int _F_SETFD = -1) const
  {
    int o = -1;
    while ((o = ::dup2(n_, _n)) == -1 and errno == EINTR);
    if (o != -1 and _F_SETFD != -1)  ::fcntl(o, F_SETFD, _F_SETFD);
    return o;
  }

  /* duplicate file descriptor to the specified number, closing the target
     file descriptor number first if necessary; set fd variable to the target
     file descriptor number, closing the old file descriptor number first;
     optionally set file descriptor flags for the new file descriptor number */
  int mov2(const int _n, const int _F_SETFD = -1)
  {
    int o = -1;
    while ((o = ::dup2(n_, _n)) == -1 and errno == EINTR);
    if (o != -1)
    {
      if (_F_SETFD != -1)  ::fcntl(o, F_SETFD, _F_SETFD);
      if (n_ != o)
      {
        while (::close(n_) == -1 and errno == EINTR);
        n_ = o;
      };
    };
    return o;
  }

public:
  /* functions to manipulate file descriptor flags and file status flags */
  int setfd(const int _flags)  { return ::fcntl(n_, F_SETFD, ::fcntl(n_, F_GETFD) | _flags); }
  int unsetfd(const int _flags)  { return ::fcntl(n_, F_SETFD, ::fcntl(n_, F_GETFD) & ~_flags); }
  int setfl(const int _flags)  { return ::fcntl(n_, F_SETFL, ::fcntl(n_, F_GETFL) | _flags); }
  int unsetfl(const int _flags)  { return ::fcntl(n_, F_SETFL, ::fcntl(n_, F_GETFL) & ~_flags); }

public:
  static void close_all(const int *_except = nullptr)
  {
    int n = -1;
    if ((n = ::snprintf(nullptr, 0, "/proc/%d/fd", ::getpid())) > 0)
    {
      char *p = 0;
      if ((p = new(std::nothrow) char[n+1]) != 0)
      {
        ::snprintf(p, n+1, "/proc/%d/fd", ::getpid());
        DIR *d = ::opendir(p);
        delete p;

        if (d)
        {
          struct dirent *e = 0;
          while ((e = ::readdir(d)) != 0)
          {
            if (e->d_name[0] == '.')  continue;

            int fd = 0;

            char *s = e->d_name;
            if (!*s)  continue;
            for (char c; *s; ++s)
            {
              c = *s - '0';
              if ((c < 0) || (9 < c))  break;
              fd = 10*fd + c;
            };
            if (*s)  continue;

            if (fd == ::dirfd(d))  continue;

            if (_except)
            {
              const int *x = _except;
              while (*x >= 0 and *x != fd)  ++x;
              if (*x >= 0)  continue;
            };

            ::close(fd);
          };

          ::closedir(d);
          return;
        };
      };
    };

    // fall back
    for (int fd = 0, m = ::sysconf(_SC_OPEN_MAX); fd < m; ++fd)
    {
      if (_except)
      {
        const int *x = _except;
        while (*x >= 0 and *x != fd)  ++x;
        if (*x >= 0)  continue;
      };
      ::close(fd);
    };

  }

};


#endif
