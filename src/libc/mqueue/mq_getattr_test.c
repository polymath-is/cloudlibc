// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fcntl.h>
#include <mqueue.h>
#include <testing.h>

TEST(mq_getattr, example) {
  // Create new queue.
  struct mq_attr attr1 = {
      .mq_flags = O_NONBLOCK,
      .mq_maxmsg = 100,
      .mq_msgsize = 200,
      .mq_curmsgs = -90210,
  };
  mqd_t mqd;
  ASSERT_EQ(0, mq_open_anon(&attr1, &mqd));

  // Fetch and validate attributes.
  struct mq_attr attr2;
  ASSERT_EQ(0, mq_getattr(mqd, &attr2));
  ASSERT_EQ(O_NONBLOCK, attr2.mq_flags);
  ASSERT_EQ(100, attr2.mq_maxmsg);
  ASSERT_EQ(200, attr2.mq_msgsize);
  ASSERT_EQ(0, attr2.mq_curmsgs);

  ASSERT_EQ(0, mq_close(mqd));
}
