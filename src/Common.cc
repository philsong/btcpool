/*
 The MIT License (MIT)

 Copyright (c) [2016] [BTC.COM]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */
#include "Common.h"
#include "bitcoin/bignum.h"

uint64 TargetToBdiff(uint256 &target) {
  CBigNum m, t;
  m.SetHex("0x00000000FFFF0000000000000000000000000000000000000000000000000000");
  t.setuint256(target);
  return strtoull((m / t).ToString().c_str(), NULL, 10);
}

uint64 TargetToBdiff(const string &str) {
  CBigNum m, t;
  m.SetHex("0x00000000FFFF0000000000000000000000000000000000000000000000000000");
  t.SetHex(str);
  return strtoull((m / t).ToString().c_str(), NULL, 10);
}

uint64 TargetToPdiff(uint256 &target) {
  CBigNum m, t;
  m.SetHex("0x00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  t.setuint256(target);
  return strtoull((m / t).ToString().c_str(), NULL, 10);
}

uint64 TargetToPdiff(const string &str) {
  CBigNum m, t;
  m.SetHex("0x00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  t.SetHex(str);
  return strtoull((m / t).ToString().c_str(), NULL, 10);
}

void BitsToTarget(uint32 bits, uint256 & target) {
  uint64 nbytes = (bits >> 24) & 0xff;
  target = bits & 0xffffffULL;
  target <<= (8 * ((uint8)nbytes - 3));
}

static const uint32 BITS_DIFF1 = 0x1d00ffff;
static uint32 _DiffToBits(uint64 diff) {
  uint64 nbytes = (BITS_DIFF1 >> 24) & 0xff;
  uint64 value  = BITS_DIFF1 & 0xffffffULL;

  if (diff == 0) {
    return 1;
  }

  while (diff % 256 == 0) {
    nbytes -= 1;
    diff /= 256;
  }

  if (value % diff == 0) {
    value /= diff;
  }
  else if ((value << 8) % diff == 0) {
    nbytes -= 1;
    value <<= 8;
    value /= diff;
  }
  else {
    return 1;
  }

  if (value > 0x00ffffffULL) {
    return 1;  // overflow... should not happen
  }
  return (uint32)(value | (nbytes << 24));
}

void DiffToTarget(uint64 diff, uint256 & target) {
  BitsToTarget(_DiffToBits(diff), target);
}

