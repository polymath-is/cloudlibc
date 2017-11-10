// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>
#include <testing.h>

TEST(gettext, example) {
  ASSERT_STREQ("Hello", gettext("Hello"));
}
