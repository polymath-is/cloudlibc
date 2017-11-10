// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>
#include <testing.h>

TEST(ngettext, example) {
  ASSERT_STREQ("%d bottles of beer on the wall",
               ngettext("%d bottle of beer on the wall",
                        "%d bottles of beer on the wall", 0));
  ASSERT_STREQ("%d bottle of beer on the wall",
               ngettext("%d bottle of beer on the wall",
                        "%d bottles of beer on the wall", 1));
  ASSERT_STREQ("%d bottles of beer on the wall",
               ngettext("%d bottle of beer on the wall",
                        "%d bottles of beer on the wall", 2));
  ASSERT_STREQ("%d bottles of beer on the wall",
               ngettext("%d bottle of beer on the wall",
                        "%d bottles of beer on the wall", 3));
}
