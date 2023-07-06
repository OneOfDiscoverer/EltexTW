#include "main.h"

void usr_handler(int sigset){
    flag = 1;
}

void* sender(void* argc){
    srandom(time(NULL));
    while(1){
        usleep(random() & 0xFFFF);
        book bk = {0};
        for(int i = 0; i < (random() & 0xFF); i++){
            bk.str[i] = 0x7F & (random() + 0x20);
        }
        pushBack(&bk);
    }
}

void* receiver(struct thrd* th){
    list* tmp;
    char str[STR_SIZE];
    while(1){ 
        //ожидание задачи
        if(sem_wait(th->lk) < 0){
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        if(tmp = getAt(0)) {
            strcpy(str, tmp->bk.str);
            remove_at(0);
        }
        if(sem_post(th->lk) < 0){
            perror("sem_post");
            exit(EXIT_FAILURE);
        }
        //выполнение задачи
        if(tmp){
            th->cnt++;
            printf("%lu: %s\n",th->thread, str);
        }
    }
}


int main(int argc, char* argv[]){
    struct sigaction new_action;
    new_action.sa_handler = usr_handler;
    new_action.sa_flags = 0;
    if(sigemptyset(&new_action.sa_mask)){
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGUSR1, &new_action ,NULL) < 0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sem_t lock;
    if(sem_init(&lock, 0, 1) < 0){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    
    struct thrd trd[NUMOFTHREAD] = {0};
    pthread_t sndr;
    pthread_create(&sndr, NULL, sender, NULL);
    for(int i = 0; i < NUMOFTHREAD; i++){
        trd[i].lk = &lock;
        pthread_create(&trd[i].thread, NULL, receiver, &trd[i]);
    }

    while(1){
        if(flag){
            int i;
            for(i = 0; getAt(i); i++);
            printf("queue len: %d \n", i);
            for(int i = 0; i < NUMOFTHREAD; i++){
                printf("thrd [%lu] has [%d] num\n", trd[i].thread, trd[i].cnt);
            }
            flag = 0;
        }
    }
    return 0;
}
