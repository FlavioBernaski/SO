#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

void leitor(void *threadId);
void escritor(void *threadId);

sem_t mutex, mutexLeitor;
int leitorLock = 1, leitores = 0;

int main(int argc, char *argv[]) {
    //Verificacao das informacoes necessarias
    if (argc < 3) return printf("./$ <N_Threads> <Proporcao>\n");
    int nThread = atoi(argv[1]);
    int chance = atoi(argv[2]);
    if (nThread < 2) nThread = 2;

    //Inicializador e identificador das Threads
    int id[nThread];
    pthread_t thread[nThread];
    for (int i = 0; i < nThread; ++i) id[i] = i;

    //Inicializador dos Semaforos
    sem_init(&mutex, 0, 1);
    sem_init(&mutexLeitor, 0, 1);

    //Criacao das Threads
    srand(time(NULL));
    pthread_create(&thread[0], NULL, (void*) &escritor, (void*) &id[0]);
    for (int i = 1; i < nThread; ++i) {
        int tmp = rand() % chance;
        if (tmp == 0) pthread_create(&thread[i], NULL, (void*) &escritor, (void*) &id[i]);
        else pthread_create(&thread[i], NULL, (void*) &leitor, (void*) &id[i]);
    }

    //Sincronizar
    for (int i=0; i < NUMTHREAD; i++) {
        pthread_join(thread[i], NULL);
    }
    sem_destroy(&mutexLeitor); /* destroy semaphore */
    sem_destroy(&mutex); /* destroy semaphore */
    pthread_exit(NULL);
    return 1;
}

void leitor(void *threadId) {
    int threadIdVar = *((int*) threadId);
    while(1) {
        if (!leitorLock) {
            sem_wait(&mutexLeitor);
            leitores++;
            sem_post(&mutexLeitor);

            int buffer;
            FILE *arq = fopen("arquivo.txt", "r");
            fscanf(arq, "%d", &buffer);
            fclose(arq);

            printf("Leitor ID %d leu o que o Escritor ID %d escreveu.\nTotal de leitores: %d\n", threadIdVar, buffer, leitores);

            sleep(1);

            sem_wait(&mutexLeitor);
            leitores--;
            sem_post(&mutexLeitor);
        } else {
            printf("Leitor bloqueado pelo bloqueador de leitura.\n");
        }
    }
}