#include "main.h"

#define NUMOFTHREAD 10

struct thrd{
    pthread_t thread;
    int cnt;
} trd[NUMOFTHREAD] = {0};

sem_t lock;

void usr_handler(int sigset){
    for(int i = 0; i < NUMOFTHREAD; i++){
        printf("thrd [%lu] has [%d] num\n", trd[i].thread, trd[i].cnt);
    }
}

void* sender(void* argc){
    while(1){
        sleep(rand() & 0x000F);
        book bk = {0};
        for(int i = 0; i < (rand() & 0x00FF); i++){
            bk.str[i] = 0xFF & rand();
        }
        pushBack(&bk);
    }
}

void* receiver(int* cnt){
    list* tmp;
    while(1){ 
        sem_wait(&lock);
        if(tmp = getAt(0)) {
            *cnt = *cnt + 1;
            printf("%s\n", tmp->bk.str);
            remove_at(0);
        }
        sem_post(&lock);
    }
}


int main(int argc, char* argv[]){
    struct sigaction new_action, old_action;
    new_action.sa_handler = usr_handler;
    new_action.sa_flags = 0;
    sigemptyset(&new_action.sa_mask);

    sem_init(&lock, 0, 1);
    sigaction(SIGUSR1, &new_action ,NULL);

    pthread_t sndr;
    pthread_create(&sndr, NULL, sender, NULL);
    for(int i = 0; i < NUMOFTHREAD; i++){
        pthread_create(&trd[i].thread, NULL, receiver, &trd[i].cnt);
    }

    while(1){
        
    }

    return 0;
}

/*
"Необходимо реализовать многопоточное приложение на языке С. Первый поток - распасовщик. N потоков - обработчики. 
Распасовщик в случайный момент времени генерирует сообщение (например, набор символов) и предает его одному из свободных 
потоков-обработчиков. 
Поток-обработчик при получении сообщения выводит его на экран и инкрементирует счетчик обработанных сообщений. 
После этого поток-обработчик засыпает на какое-то время, чтобы дать возможность поработать соседним потокам.
Приложение должно уметь обрабатывать сигнал USR1, при котором будет выводиться статистика. 
Сколько сообщений обработал каждый поток-обработчик.

Работу выполнять с использованием системы контроля версий git – создать репозиторий на внутреннем Gitlab.

Необходимо покрыть тестами свое приложение. В качестве фреймворка использовать TAP и FFF.

В качестве системы сборки использовать Make или CMake."
*/