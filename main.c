#include "main.h"

void usr_handler(int sigset){
    flag = 1;
}

void trd_handler(int sigset){

}

void* sender(sem_t *lock){ //создаем задачи для процессов
    srandom(time(NULL));
    while(1){
        usleep(random() & RND_TIME_MASK);
        book bk = {0};
        for(int i = 0; i < (random() & RND_LEN_MASK); i++){
            if(i >= STR_SIZE)
                break;
            bk.str[i] = 0x7F & (random() + 0x20);
        }
        sem_wait(lock);
        pushBack(&bk);
        sem_post(lock);
    }
}

void* receiver(struct thrd* th){
    list* tmp;
    char str[STR_SIZE];

    struct sigaction new_action;
    new_action.sa_handler = trd_handler;
    new_action.sa_flags = 0;
    if(sigemptyset(&new_action.sa_mask)){
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGUSR2, &new_action ,NULL) < 0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while(1){ 
        th->is_sleep = 0;
        th->is_sleep = sleep(__INT_MAX__);  //спим до сигнала. Можно "спать" на следующей строке.
        sem_wait(th->lk);                   //атомарно забираем экземпляр задачи
        if(tmp = getAt(0)){
            strcpy(str, tmp->bk.str);
            remove_at(0);
        }
        sem_post(th->lk);
        //выполнение задачи
        if(tmp){
            th->cnt++;
            printf("%lu: %s\n",th->thread, str);
            tmp = 0;
            usleep(RND_TIME_MASK); // нагрузка
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

    if(argc > 1){
        num_of_threads = atoi(argv[1]);
    }

    struct thrd *trd = calloc(sizeof(struct thrd), num_of_threads);

    pthread_t sndr;
    if(pthread_create(&sndr, NULL, sender, &lock)){
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < num_of_threads; i++){
        trd[i].lk = &lock;
        if(pthread_create(&trd[i].thread, NULL, receiver, &trd[i])){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    while(1){
        if(getAt(0)){ //пробуждаем потоки пока не выберем всю очередь
            for(int i = 0; i < num_of_threads; i++){
                if(!trd[i].is_sleep){
                    pthread_kill(trd[i].thread, SIGUSR2); //грубо скидываем sleep сигналом USR2
                    break;
                }
            }       
        }
        if(flag){ //обробатываем сигнал USR1
            int i;
            for(i = 0; getAt(i); i++);
            printf("queue len: %d \n", i);
            for(int i = 0; i < num_of_threads; i++){
                printf("thrd [%lu] has [%d] num\n", trd[i].thread, trd[i].cnt);
            }
            flag = 0;
        }
    }
    return 0;
}
