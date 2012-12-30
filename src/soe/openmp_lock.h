///
/// @file   openmp_lock.h
/// @brief  The OmpInitLock and OmpLockGuard classes are RAII-style
///         wrappers for OpenMP locks.
///
/// Copyright (C) 2012 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the New BSD License. See the
/// LICENSE file in the top level directory.
///

#ifndef OPENMP_LOCK_H
#define OPENMP_LOCK_H

#include <omp.h>

namespace soe {

/// RAII-style wrapper for OpenMP locks.
/// Initialize lock -> destroy lock.
///
class OmpInitLock {
public:
  OmpInitLock(void**);
  ~OmpInitLock();
private:
  omp_lock_t lock_;
};

/// @param lockAdress  Allocate a new lock_ on the stack and store
///                    its address to lockAdress.
///
OmpInitLock::OmpInitLock(void** lockAddress)
{
  *lockAddress = static_cast<void*>(&lock_);
  omp_init_lock(&lock_);
}

OmpInitLock::~OmpInitLock()
{
  omp_destroy_lock(&lock_);
}

/// RAII-style wrapper for OpenMP locks.
/// Set lock -> unset lock.
///
class OmpLockGuard {
public:
  OmpLockGuard(omp_lock_t*, bool);
  ~OmpLockGuard();
  bool isSet() const;
private:
  omp_lock_t* lock_;
  bool isSet_;
};

/// @param waitForLock  If false do not block the current thread if
///                     the lock is not available.
///
OmpLockGuard::OmpLockGuard(omp_lock_t* lock, bool waitForLock = true) :
  lock_(lock)
{
  if (!waitForLock)
    isSet_ = (omp_test_lock(lock_) != 0);
  else {
    omp_set_lock(lock_);
    isSet_ = true;
  }
}

OmpLockGuard::~OmpLockGuard()
{
  if (isSet())
    omp_unset_lock(lock_);
}

bool OmpLockGuard::isSet() const
{
  return isSet_;
}

} // namespace soe

#endif
