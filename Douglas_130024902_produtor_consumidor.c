/**
 * UNIVERSIDADE DE BRASILIA
 * Programação Concorrente 2/2016
 * Douglas Shiro YOkoyama 13/0024902
 * Produtores e consumidores
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cons = PTHREAD_COND_INITIALIZER;

void * produtor(void * arg);
void * consumidor(void * arg);

double drand48(void);
void srand48(long int seedval);

#define PROD 10
#define CONS 10
#define N 10
#define TRUE 1

int b[N];
int cont = 0;
int prodpos = 0;
int conspos = 0;


int main(){
    pthread_t p[PROD];
    pthread_t c[CONS];

    srand48(time(NULL));
    int i;
    int *id = NULL;

    for(i = 0; i < PROD; i++){
        id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&p[i], NULL, produtor, (void*)(id));
    }
    for(i = 0; i < CONS; i++){
        id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&c[i], NULL, consumidor, (void*)(id));
    }
    pthread_join(p[0], NULL);
    return 0;
}

void * produtor(void * arg){
    int p = (int)(drand48()*1000);
    int i = *((int*) arg);

    while(TRUE){
        pthread_mutex_lock(&mp);
        while(cont == N){
            printf("-------------------------\n Esperar consumidor unidades de produtos lotadas!\n");
            pthread_cond_wait(&prod, &mp);
        }
        b[prodpos] = p;
        prodpos = (prodpos + 1)%N;
        cont++;
        if(cont == 1){
            printf("-------------------------\nproduzidos %d unidades, acordar consumidor!\n", p);
            pthread_cond_signal(&cons);
        }
        sleep(1);
        printf("-------------------------\nproduzindo[%d]... \nproduzidos: %d unidades\n", i, p);
        pthread_mutex_unlock(&mp);

        sleep(1);
    }
}

void * consumidor(void * arg){
    int i = *((int*) arg);
    while(TRUE){
        pthread_mutex_lock(&mp);
        while(cont == 0){
            printf("*************************\nEsperar produtos, sem unidades para conusmir!\n");
            pthread_cond_wait(&cons, &mp);
        }
        int c = b[conspos];
        conspos = (conspos + 1)%N;
        cont--;
        if(cont == (N -1)){
            printf("*************************\nAcabou produto!\n");
            pthread_cond_signal(&prod);
        }
        sleep(1);
        printf("*************************\nconsumindo[%d]... \nconsumidos: %d unidades\n", i, c);
        pthread_mutex_unlock(&mp);
        sleep(1);

    }
}
