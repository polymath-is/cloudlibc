// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_sandboxed, example) {
  ASSERT_TRUE(cap_sandboxed());
}
