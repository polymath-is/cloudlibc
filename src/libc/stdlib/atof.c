// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef atof
#error "atof is supposed to be a macro as well"
#endif

// clang-format off
double (atof)(const char *str) {
  return atof(str);
}
