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
    pthread_t thread;   //ид процесса
    int cnt;            //счетчик сообщений
    sem_t* lk;          //указатель на семафор
    char is_sleep;      //флаг состояния сна
};

volatile char flag;
volatile int num_of_threads = 2;

void* sender(sem_t *lock);
void* receiver(struct thrd* th);
void trd_handler(int sigset);
void usr_handler(int sigset);

#endif