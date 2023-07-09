#ifndef _MAIN_H
#define _MAIN_H 1

#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define RND_TIME_MASK   0xFFFF
#define RND_LEN_MASK    0xFF

struct thrd{
    pthread_t thread;
    int cnt;
    sem_t* lk;
};

volatile char flag;

#endif