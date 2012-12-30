///
/// @file  EratSmall.h
///
/// Copyright (C) 2012 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the New BSD License. See the
/// LICENSE file in the top level directory.
///

#ifndef ERATSMALL_H
#define ERATSMALL_H

#include "config.h"
#include "WheelFactorization.h"

#include <stdint.h>
#include <list>

namespace soe {

/// EratSmall is an implementation of the segmented sieve of
/// Eratosthenes optimized for small sieving primes that have many
/// multiples per segment.
///
class EratSmall : public Modulo30Wheel_t {
public:
  EratSmall(uint64_t, uint_t, uint_t);
  uint_t getLimit() const { return limit_; }
  void crossOff(uint8_t*, uint8_t*);
private:
  typedef std::list<Bucket>::iterator BucketIterator_t;
  const uint_t limit_;
  /// List of buckets, holds the sieving primes
  std::list<Bucket> buckets_;
  void store(uint_t, uint_t, uint_t);
  static void crossOff(uint8_t*, uint8_t*, Bucket&);
  DISALLOW_COPY_AND_ASSIGN(EratSmall);
};

} // namespace soe

#endif
