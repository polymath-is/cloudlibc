// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(fputs, eagain) {
  // Create a stream to non-blocking write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[1], "w");

  // Write data into the pipe until it fails.
  int ret;
  for (;;) {
    ret = fputs("Hello", fp);
    if (ret == EOF)
      break;
    ASSERT_LE(0, ret);
    ASSERT_FALSE(ferror(fp));
  }

  // Final write should have failed with EAGAIN.
  ASSERT_EQ(EOF, ret);
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);

  // Closing should try to flush the data again, but it will still fail
  // with EAGAIN.
  errno = 0;
  ASSERT_EQ(EOF, fclose(fp));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fputs, ebadf) {
  // Create a stream to the read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[0], "r");
  ASSERT_NE(NULL, fp);

  // Writing to the read side should fail.
  ASSERT_EQ(EOF, fputs("Hello", fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fputs, enospc) {
  // Create a very small temporary file.
  FILE *fp = fmemopen(NULL, 1, "w+");
  ASSERT_NE(NULL, fp);

  // Writing a multiple characters shall fail.
  ASSERT_EQ(EOF, fputs("Hello", fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(ENOSPC, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fputs, epipe) {
  // Create a stream to non-blocking write side of a pipe that has
  // already been closed.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_EQ(0, close(fds[0]));

  // Write data into the pipe until it fails.
  int ret;
  for (;;) {
    ret = fputs("Hello", fp);
    if (ret == EOF)
      break;
    ASSERT_LE(0, ret);
    ASSERT_FALSE(ferror(fp));
  }

  // Final write should have failed with EPIPE.
  ASSERT_EQ(EOF, ret);
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EPIPE, errno);

  // Closing should try to flush the data again, but it will still fail
  // with EPIPE.
  errno = 0;
  ASSERT_EQ(EOF, fclose(fp));
  ASSERT_EQ(EPIPE, errno);
}

TEST(fputs, success) {
  // Create a large enough temporary file.
  FILE *fp = fmemopen(NULL, 5, "w+");
  ASSERT_NE(NULL, fp);

  // Fill file with characters.
  ASSERT_LE(0, fputs("Hello", fp));
  ASSERT_LE(0, fputs("", fp));
  ASSERT_FALSE(ferror(fp));

  // File should contain three consecutive characters.
  rewind(fp);
  char buf[6];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("Hello", buf);
  ASSERT_EQ(0, fclose(fp));
}
