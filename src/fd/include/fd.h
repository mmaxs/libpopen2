/*
  Copyright (c) 2015 Mikhail Usenko <michaelus@tochka.ru>
*/

#ifndef FD__H
#define FD__H

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


/* a small class wrapping the most frequent operations with file descriptors
   that makes client code to be a bit more clean and straightforward; class
   size is equal to sizeof(int) and its instances are interchangeable with file
   descriptor POD-values for most use cases */
class fd
{
private:
  int n_;

public:
  ~fd() = default;

  fd() : n_(-1)  { static_assert(sizeof(*this) == sizeof(n_), ""); }

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
  operator int& ()  { return n_; }

  /* check if the fd variable has a valid and open file descriptor number */
  explicit operator bool ()  { return ::fcntl(n_, F_GETFD) != -1; }

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
};


#endif
