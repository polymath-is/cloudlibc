// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "mqueue_impl.h"

int mq_open_anon(const struct mq_attr *attr, mqd_t *mqdes) {
  // Only allow O_NONBLOCK to be set. Maximum number of messages and
  // message size must be positive.
  if ((attr->mq_flags & ~O_NONBLOCK) != 0 || attr->mq_maxmsg <= 0 ||
      attr->mq_msgsize <= 0) {
    errno = EINVAL;
    return -1;
  }

  // Allocate and initialize a new message queue object.
  struct __mqd *mqd = malloc(sizeof(*mqd));
  if (mqd == NULL)
    return -1;
  if (pthread_mutex_init(&mqd->lock, NULL) != 0) {
    free(mqd);
    return -1;
  }
  if (pthread_cond_init(&mqd->cond, NULL) != 0) {
    pthread_mutex_destroy(&mqd->lock);
    free(mqd);
    return -1;
  }
  mqd->attr = *attr;
  mqd->attr.mq_curmsgs = 0;
  mqd->queue_receive = NULL;
  mqd->queue_send = NULL;
  mqdes->__mqd = mqd;
  return 0;
}
