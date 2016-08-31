// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef ___ARCH_ARM_FENV_H_
#define ___ARCH_ARM_FENV_H_

// TODO(ed): Implement.

typedef struct {
  int __bogus;
} fenv_t;

typedef struct { int __bogus; } fexcept_t;

#define FE_INVALID 0x01
#define FE_DIVBYZERO 0x04
#define FE_OVERFLOW 0x08
#define FE_UNDERFLOW 0x10
#define FE_INEXACT 0x20

#define FE_ALL_EXCEPT                                                     \
  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)

#define FE_TONEAREST 0x0000
#define FE_DOWNWARD 0x2000
#define FE_UPWARD 0x4000
#define FE_TOWARDZERO 0x6000

#endif
