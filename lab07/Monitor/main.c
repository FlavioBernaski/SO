/*
Descrição:
   Programa multithread com uma solucao para o problema do PapaiNoel.
   Na linha 67 tem um while() comentado para que o programa funcione totalmente como especificado.
   Os elfos demoram alguns segundos para comecar a pedir ajuda, isto eh devido ao sleep()
Autores:
   Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
Datas:
   https://github.com/caiotheodoro/SO/commits/master/LAB7/ex1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "resource_monitor.h"

/* --- funcoes --- */
void* barbeiro();
void* cliente(void* thread_id);

/* --- main --- */
int main(int argc, char* argv[]){
   if(argc < 2) return printf("./$ <num_de_clientes>\n");
   int numClientes = atoi(argv[1]);

   printf("numClientes: %d \n",numClientes);
   // Identificadores para as Threads. (1 barbeiro + numClientes)
   int id[(1+numClientes)];
   
   for (int i=0; i < (1+numClientes); i++) id[i] = i;

   pthread_t barbeirod;
   pthread_t clientes[numClientes];

   initMonitor();

   //criar thread barbeiro
   pthread_create(&barbeirod, NULL, barbeiro, NULL);

   //criar threads clientes
   for(int i=0; i<numClientes; i++)
       pthread_create(&clientes[i], NULL, cliente, (void*) &id[i+1]);

   // Wait for the threads to finish
   pthread_join(barbeirod, NULL);
   for(int i=0; i<numClientes; i++) pthread_cancel(clientes[i]);

   destroyMonitor();

   printf("Programa finalizado.\n");
   pthread_exit(0);
}


/* --- funcoes ---*/
void* cliente(void* thread_id){
   int id = *((int*)thread_id);

   //seed do rand()
   srand(id);
   int tempo;

   while(1){
       // dormir por um tempo aleatorio
       tempo = 2;;
       sleep(tempo);

       // cliente chegou na barbearia
       int status = put(id);
       if(status == 0) printf("CLIENTE %d: Chegou para cortar cabelo.\n", id);
       fflush(stdout);
   }
}

void* barbeiro(){
    int descansando = 1;
   
   while(descansando == 1){    //parar o loop apos cortar todos os cabelos

       //Barbeiro espera o sinal
       descansando = take();
       
       if(descansando == 1 ){
           printf("BARBEIRO: cortou cabelo do cliente.\n");
        }
       fflush(stdout);
   }
}

