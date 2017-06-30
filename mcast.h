#ifndef MCAST_H
#define MCAST_H

#include <stdint.h>

#include <pthread.h>

#include "critbit.h"

typedef struct {

  uint64_t state;
  
  critbit0_tree *ls_entries;

  pthread_mutex_t readwrite_lock;
  
  int init : 1;
  
} mcast_t;

#endif
